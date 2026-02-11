use std::collections::HashMap;
use std::fs;
use std::path::Path;

pub type AliasMap = HashMap<String, String>;

pub fn legacy_path_hex(path_bytes: &[u8]) -> String {
    let mut s = String::with_capacity(path_bytes.len() * 2);
    for b in path_bytes {
        s.push_str(&format!("{:02X}", b));
    }
    s
}

pub fn load_alias_tsv<P: AsRef<Path>>(path: P) -> std::io::Result<AliasMap> {
    let text = fs::read_to_string(path)?;
    let mut map = AliasMap::new();

    for (idx, line) in text.lines().enumerate() {
        if idx == 0 && line.starts_with("legacy_path_hex\t") {
            continue;
        }
        if line.trim().is_empty() {
            continue;
        }
        let mut it = line.split('\t');
        let legacy_hex = it.next().unwrap_or("").trim();
        let _legacy_cp949 = it.next().unwrap_or("");
        let normalized = it.next().unwrap_or("").trim();
        if !legacy_hex.is_empty() && !normalized.is_empty() {
            map.insert(legacy_hex.to_string(), normalized.to_string());
        }
    }

    Ok(map)
}

pub fn resolve_from_map<'a>(map: &'a AliasMap, legacy_path_bytes: &[u8]) -> Option<&'a str> {
    let key = legacy_path_hex(legacy_path_bytes);
    map.get(&key).map(String::as_str)
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::io::Write;

    #[test]
    fn test_hex() {
        assert_eq!(legacy_path_hex(b"objmon/talk"), "6F626A6D6F6E2F74616C6B");
    }

    #[test]
    fn test_load_and_resolve() {
        let mut f = tempfile::NamedTempFile::new().unwrap();
        writeln!(
            f,
            "legacy_path_hex\tlegacy_path_cp949\tnormalized_utf8_path\tblob_sha1\n6F626A\tobj\tobj_utf8\tdeadbeef"
        )
        .unwrap();

        let map = load_alias_tsv(f.path()).unwrap();
        assert_eq!(resolve_from_map(&map, b"obj"), Some("obj_utf8"));
        assert_eq!(resolve_from_map(&map, b"none"), None);
    }
}
