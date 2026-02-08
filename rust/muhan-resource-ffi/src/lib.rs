use muhan_resource_index::{load_alias_tsv, resolve_from_map, AliasMap};
use std::ffi::CStr;
use std::os::raw::{c_char, c_int};
use std::ptr;
use std::sync::OnceLock;

static MAP: OnceLock<AliasMap> = OnceLock::new();

#[no_mangle]
pub extern "C" fn mr_manifest_load(manifest_path: *const c_char) -> c_int {
    if manifest_path.is_null() {
        return -1;
    }

    let path = unsafe { CStr::from_ptr(manifest_path) };
    let path = match path.to_str() {
        Ok(v) => v,
        Err(_) => return -1,
    };

    let loaded = match load_alias_tsv(path) {
        Ok(m) => m,
        Err(_) => return -1,
    };

    if MAP.set(loaded).is_err() {
        // already initialized; treat as success
        return 0;
    }

    0
}

#[no_mangle]
pub extern "C" fn mr_resolve_legacy_path(
    legacy_path: *const c_char,
    out: *mut c_char,
    out_len: usize,
) -> c_int {
    if legacy_path.is_null() || out.is_null() || out_len == 0 {
        return -1;
    }

    let map = match MAP.get() {
        Some(m) => m,
        None => return -1,
    };

    let path_bytes = unsafe { CStr::from_ptr(legacy_path) }.to_bytes();
    let normalized = match resolve_from_map(map, path_bytes) {
        Some(v) => v,
        None => return -1,
    };

    let bytes = normalized.as_bytes();
    if bytes.len() + 1 > out_len {
        return -1;
    }

    unsafe {
        ptr::copy_nonoverlapping(bytes.as_ptr(), out as *mut u8, bytes.len());
        *out.add(bytes.len()) = 0;
    }

    0
}
