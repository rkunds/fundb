## FunDB!
This is a project of mine to build a key-value database from scratch using Log-Structured Merge Trees and Leveled Compaction. I had previously worked on [Venice](https://github.com/linkedin/venice) at LinkedIn, which was a source of inspiration to dig deeper into how the underlying database ([RocksDB](https://github.com/facebook/rocksdb)) works at a low level. 

### Completed / To Do
**To Do:**
-  Finish Merge Iterator for SST Files in Compaction
-  Multithreaded Compaction
-  Read Engine 
- probably some more... will update!
- Distributed KV Datastore


**Completed:**
- Threadsafe Memtable (Skiplist)
- Flush Memtable to L0
- Data to Sorted String Table on Disk writers
- Single SST Reader with SST Iterator
<!-- how to hyperlink -->
