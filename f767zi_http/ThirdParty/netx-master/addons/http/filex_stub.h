#ifndef FILEX_STUB_H
#define FILEX_STUB_H

#include "main.h"

/* This is a stub routine for FileX, for UsbX applications that do not have FileX to rely on.  */
#ifndef FX_MAXIMUM_PATH
#define FX_MAXIMUM_PATH                        256
#endif

#ifndef FX_FAT_MAP_SIZE
#define FX_FAT_MAP_SIZE                        128  /* Minimum 1, maximum any. This represents how many 32-bit words used for the written FAT sector bit map. */
#endif

#define FX_MAX_SHORT_NAME_LEN                  13       /* Only allowed value is 13.  */

#ifndef FX_MAX_SECTOR_CACHE
#define FX_MAX_SECTOR_CACHE                    256      /* Maximum size of logical sector cache,
                                                           minimum value of 2 all other values must be a power of 2.*/
#endif

#ifndef FX_MAX_LONG_NAME_LEN
#define FX_MAX_LONG_NAME_LEN                   256      /* Minimum value is 13, Maximum value is 256.  */
#endif


#ifndef FX_MAX_LAST_NAME_LEN
#define FX_MAX_LAST_NAME_LEN                   256      /* Should be as large or larger than FX_MAX_LONG_NAME_LEN.  */
#endif

#ifndef FX_MAX_FAT_CACHE
#define FX_MAX_FAT_CACHE                       16   /* Minimum value is 8, all values must be a power of 2.  */
#endif
/* First, define basic data types.  */
typedef struct FX_CACHED_SECTOR_STRUCT
{

    /* Define the buffer pointer associated with this cache entry.  */
    UCHAR               *fx_cached_sector_memory_buffer;

    /* Define the sector number that is cached.  */
    ULONG64             fx_cached_sector;

    /* Define the flag that indicates whether or not the cached sector
       has been modified and needs to be written to the media.  */
    UCHAR               fx_cached_sector_buffer_dirty;

    /* Define the valid flag that indicates whether or not this entry is
       still valid.  */
    UCHAR               fx_cached_sector_valid;

    /* Define the sector type, which indicates what type of sector is present.  */
    UCHAR               fx_cached_sector_type;

    /* Define a reserved byte, reserved for future use.  */
    UCHAR               fx_cached_sector_reserved;

    /* Define the next cached sector pointer.  This is used to implement
       the "last used" algorithm when looking for cache entry to swap out to
       the physical media.  */
    struct FX_CACHED_SECTOR_STRUCT
                        *fx_cached_sector_next_used;

} FX_CACHED_SECTOR;

typedef struct FX_DIR_ENTRY_STRUCT
{

    CHAR   *fx_dir_entry_name;
    CHAR    fx_dir_entry_short_name[FX_MAX_SHORT_NAME_LEN];                 /* Short file name, if LFN is present                */
    UINT    fx_dir_entry_long_name_present;                                 /* 0 (default) => LFN not present; 1 => LFN present  */
    UINT    fx_dir_entry_long_name_shorted;                                 /* LFN too large, file name was made shorter         */
    UCHAR   fx_dir_entry_attributes;                                        /* Directory entry attributes                        */
    UCHAR   fx_dir_entry_reserved;                                          /* NT reserved, write 0 at create, otherwise ignore  */
    UCHAR   fx_dir_entry_created_time_ms;                                   /* Create time in milliseconds, always write 0       */
    UINT    fx_dir_entry_created_time;                                      /* Created time                                      */
    UINT    fx_dir_entry_created_date;                                      /* Created date                                      */
    UINT    fx_dir_entry_last_accessed_date;                                /* Last accessed date                                */
    UINT    fx_dir_entry_time;                                              /* Modified time                                     */
    UINT    fx_dir_entry_date;                                              /* Modified cluster                                  */
    ULONG   fx_dir_entry_cluster;                                           /* File/directory's starting cluster                 */
    ULONG64 fx_dir_entry_file_size;                                         /* Size of the file in bytes                         */
    ULONG64 fx_dir_entry_log_sector;                                        /* Logical sector of this directory entry            */
    ULONG   fx_dir_entry_byte_offset;                                       /* Offset in logical sector of this directory entry  */
    ULONG   fx_dir_entry_number;                                            /* Index into the directory                          */
    ULONG   fx_dir_entry_last_search_cluster;                               /* Last cluster searched                             */
    ULONG   fx_dir_entry_last_search_relative_cluster;                      /* Last relative cluster searched                    */
    ULONG64 fx_dir_entry_last_search_log_sector;                            /* Last logical sector searched                      */
    ULONG   fx_dir_entry_last_search_byte_offset;                           /* Last offset in logical sector searched            */
    ULONG64 fx_dir_entry_next_log_sector;

#ifdef FX_ENABLE_EXFAT
    /* for exFAT */
    CHAR    fx_dir_entry_dont_use_fat;                                      /* 0 bit - for current, 1st bit - for parent         */
    UCHAR   fx_dir_entry_type;
    ULONG64 fx_dir_entry_available_file_size;
    ULONG   fx_dir_entry_secondary_count;
#endif /* FX_ENABLE_EXFAT */
} FX_DIR_ENTRY;

typedef struct FX_FAT_CACHE_ENTRY_STRUCT
{
    ULONG fx_fat_cache_entry_cluster;
    ULONG fx_fat_cache_entry_value;
    ULONG fx_fat_cache_entry_dirty;
} FX_FAT_CACHE_ENTRY;

typedef struct FX_PATH_STRUCT
{
    /* Define the path information.  */
    FX_DIR_ENTRY fx_path_directory;
    CHAR         fx_path_string[FX_MAXIMUM_PATH];
    CHAR         fx_path_name_buffer[FX_MAX_LONG_NAME_LEN];
    ULONG        fx_path_current_entry;
} FX_PATH;

typedef struct FX_MEDIA_STRUCT
{

    /* Define the media ID used for error checking.  */
    ULONG               fx_media_id;

    /* Define the media's name.  */
    CHAR                *fx_media_name;

    /* Remember the memory buffer area.  */
    UCHAR               *fx_media_memory_buffer;
    ULONG               fx_media_memory_size;

#ifdef FX_DISABLE_CACHE
    ULONG64             fx_media_memory_buffer_sector;
#else

    /* Define the flag that indicates whether the logical cache utilizes
       a hash function or is a linear search. If set, the logical cache
       is accessed via a hash function on the requested sector.  */
    UINT                fx_media_sector_cache_hashed;

    /* Define the number of sectors that can actually be cached based on the
       user supplied buffer at media open time.  */
    ULONG               fx_media_sector_cache_size;

    /* Define the end of the cache area.  This is used to determine
       if the I/O is for the internal memory of the media.  */
    UCHAR               *fx_media_sector_cache_end;

    /* Define the list head of the cached sector entries.  This
       pointer points to the most recently used cache sector.  */
    struct FX_CACHED_SECTOR_STRUCT
                        *fx_media_sector_cache_list_ptr;

    /* Define the bit map that represents the hashed cache sectors that are
       valid. This bit map will help optimize the invalidation of the hashed
       sector cache.  */
    ULONG               fx_media_sector_cache_hashed_sector_valid;

    /* Define the outstanding dirty sector counter. This is used to optimize
       the searching of sectors to flush to the media.  */
    ULONG               fx_media_sector_cache_dirty_count;
#endif /* FX_DISABLE_CACHE */

    /* Define the basic information about the associated media.  */
    UINT                fx_media_bytes_per_sector;
    UINT                fx_media_sectors_per_track;
    UINT                fx_media_heads;

    ULONG64             fx_media_total_sectors;
    ULONG               fx_media_total_clusters;

#ifdef FX_ENABLE_EXFAT
    /* Define exFAT media information.  */
    ULONG               fx_media_exfat_volume_serial_number;
    UINT                fx_media_exfat_file_system_revision;
    UINT                fx_media_exfat_volume_flag;
    USHORT              fx_media_exfat_drive_select;
    USHORT              fx_media_exfat_percent_in_use;
    UINT                fx_media_exfat_bytes_per_sector_shift;
    UINT                fx_media_exfat_sector_per_clusters_shift;

    /* exFAT: Bitmap cache */
    /* Pointer to Bitmap cache */
    UCHAR               fx_media_exfat_bitmap_cache[FX_EXFAT_BITMAP_CACHE_SIZE];

    /* Define beginning sector of Bitmap table.  */
    ULONG               fx_media_exfat_bitmap_start_sector;

    /* Define the cache size in sectors. Used for flash operation.  */
    ULONG               fx_media_exfat_bitmap_cache_size_in_sectors;

    /* Define the number of first cached cluster.  */
    ULONG               fx_media_exfat_bitmap_cache_start_cluster;

    /* Define the number of last cached cluster.  */
    ULONG               fx_media_exfat_bitmap_cache_end_cluster;

    /* Define how many clusters mapped in one sector.  */
    UINT                fx_media_exfat_bitmap_clusters_per_sector_shift;

    /* Define is Bitmap table was changed or not.  */
    UINT                fx_media_exfat_bitmap_cache_dirty;
#endif /* FX_ENABLE_EXFAT */

    UINT                fx_media_reserved_sectors;
    UINT                fx_media_root_sector_start;
    UINT                fx_media_root_sectors;
    UINT                fx_media_data_sector_start;
    UINT                fx_media_sectors_per_cluster;
    UINT                fx_media_sectors_per_FAT;
    UINT                fx_media_number_of_FATs;
    UINT                fx_media_12_bit_FAT;
    UINT                fx_media_32_bit_FAT;
    ULONG               fx_media_FAT32_additional_info_sector;
    UINT                fx_media_FAT32_additional_info_last_available;
#ifdef FX_DRIVER_USE_64BIT_LBA
    ULONG64             fx_media_hidden_sectors;
#else
    ULONG               fx_media_hidden_sectors;
#endif
    ULONG               fx_media_root_cluster_32;
    UINT                fx_media_root_directory_entries;
    ULONG               fx_media_available_clusters;
    ULONG               fx_media_cluster_search_start;

    /* Define the information pertinent to the I/O driver interface.  */

    VOID                *fx_media_driver_info;
    UINT                fx_media_driver_request;
    UINT                fx_media_driver_status;
    UCHAR               *fx_media_driver_buffer;
#ifdef FX_DRIVER_USE_64BIT_LBA
    ULONG64             fx_media_driver_logical_sector;
#else
    ULONG               fx_media_driver_logical_sector;
#endif
    ULONG               fx_media_driver_sectors;
    ULONG               fx_media_driver_physical_sector;
    UINT                fx_media_driver_physical_track;
    UINT                fx_media_driver_physical_head;
    UINT                fx_media_driver_write_protect;      /* The driver sets this to FX_TRUE when media is write protected.  */
    UINT                fx_media_driver_free_sector_update; /* The driver sets this to FX_TRUE when it needs to know freed clusters.  */
    UINT                fx_media_driver_system_write;
    UINT                fx_media_driver_data_sector_read;
    UINT                fx_media_driver_sector_type;

    /* Define the driver entry point.  */
    VOID                (*fx_media_driver_entry)(struct FX_MEDIA_STRUCT *);

    /* Define notify function called when media is open. */
    VOID                (*fx_media_open_notify)(struct FX_MEDIA_STRUCT *);

    /* Define notify function called when media is closed. */
    VOID                (*fx_media_close_notify)(struct FX_MEDIA_STRUCT *);

    /* Define the head pointer for the open files of this media.  */
    struct FX_FILE_STRUCT
                        *fx_media_opened_file_list;

    /* Define the counter for keeping track of how many open files are
       present.  */
    ULONG               fx_media_opened_file_count;

    /* Define the next and previous link pointers for the open media list.  */
    struct FX_MEDIA_STRUCT
                        *fx_media_opened_next,
                        *fx_media_opened_previous;

#ifndef FX_MEDIA_STATISTICS_DISABLE

    /* Define various statistics for this media instance. This information
       should be useful in performance tuning and optimizing the application.  */

    ULONG               fx_media_directory_attributes_reads;
    ULONG               fx_media_directory_attributes_sets;
    ULONG               fx_media_directory_creates;
    ULONG               fx_media_directory_default_gets;
    ULONG               fx_media_directory_default_sets;
    ULONG               fx_media_directory_deletes;
    ULONG               fx_media_directory_first_entry_finds;
    ULONG               fx_media_directory_first_full_entry_finds;
    ULONG               fx_media_directory_information_gets;
    ULONG               fx_media_directory_local_path_clears;
    ULONG               fx_media_directory_local_path_gets;
    ULONG               fx_media_directory_local_path_restores;
    ULONG               fx_media_directory_local_path_sets;
    ULONG               fx_media_directory_name_tests;
    ULONG               fx_media_directory_next_entry_finds;
    ULONG               fx_media_directory_next_full_entry_finds;
    ULONG               fx_media_directory_renames;
    ULONG               fx_media_file_allocates;
    ULONG               fx_media_file_attributes_reads;
    ULONG               fx_media_file_attributes_sets;
    ULONG               fx_media_file_best_effort_allocates;
    ULONG               fx_media_file_closes;
    ULONG               fx_media_file_creates;
    ULONG               fx_media_file_deletes;
    ULONG               fx_media_file_opens;
    ULONG               fx_media_file_reads;
    ULONG               fx_media_file_relative_seeks;
    ULONG               fx_media_file_renames;
    ULONG               fx_media_file_seeks;
    ULONG               fx_media_file_truncates;
    ULONG               fx_media_file_truncate_releases;
    ULONG               fx_media_file_writes;
    ULONG               fx_media_aborts;
    ULONG               fx_media_flushes;
    ULONG               fx_media_reads;
    ULONG               fx_media_writes;
    ULONG               fx_media_directory_entry_reads;
    ULONG               fx_media_directory_entry_writes;
    ULONG               fx_media_directory_searches;
    ULONG               fx_media_directory_free_searches;
    ULONG               fx_media_fat_entry_reads;
    ULONG               fx_media_fat_entry_writes;
    ULONG               fx_media_fat_entry_cache_read_hits;
    ULONG               fx_media_fat_entry_cache_read_misses;
    ULONG               fx_media_fat_entry_cache_write_hits;
    ULONG               fx_media_fat_entry_cache_write_misses;
    ULONG               fx_media_fat_cache_flushes;
    ULONG               fx_media_fat_sector_reads;
    ULONG               fx_media_fat_sector_writes;
    ULONG               fx_media_logical_sector_reads;
    ULONG               fx_media_logical_sector_writes;
    ULONG               fx_media_logical_sector_cache_read_hits;
    ULONG               fx_media_logical_sector_cache_read_misses;
    ULONG               fx_media_driver_read_requests;
    ULONG               fx_media_driver_write_requests;
    ULONG               fx_media_driver_boot_read_requests;
    ULONG               fx_media_driver_boot_write_requests;
    ULONG               fx_media_driver_release_sectors_requests;
    ULONG               fx_media_driver_flush_requests;
#ifndef FX_MEDIA_DISABLE_SEARCH_CACHE
    ULONG               fx_media_directory_search_cache_hits;
#endif
#endif

    /* Define the media's protection object, which is a ThreadX mutex.
       Only one thread is allowed to access any media or associated files
       at a time.  If FX_SINGLE_THREAD is defined, the FileX services are
       going to be called from only one thread, hence the protection is
       not needed.  */
#ifndef FX_SINGLE_THREAD
    TX_MUTEX            fx_media_protect;
#endif

#ifndef FX_MEDIA_DISABLE_SEARCH_CACHE

    /* Define the information used to remember the last directory entry found through
       searching or walking the directory via directory entry next. This information
       will be used to eliminate multiple searches for the same directory entry if
       the accesses are done sequentially.  */
    UINT                fx_media_last_found_directory_valid;
    FX_DIR_ENTRY        fx_media_last_found_directory;
    FX_DIR_ENTRY        fx_media_last_found_entry;
    CHAR                fx_media_last_found_file_name[FX_MAX_LONG_NAME_LEN];
    CHAR                fx_media_last_found_name[FX_MAX_LAST_NAME_LEN];
#endif

    /* Define the current directory information for the media.  */
    FX_PATH             fx_media_default_path;

    /* Define FAT entry cache and the variable used to index the cache.  */
    FX_FAT_CACHE_ENTRY  fx_media_fat_cache[FX_MAX_FAT_CACHE];

    /* Define the FAT secondary update map.  This will be used on flush and
       close to update sectors of any secondary FATs in the media.  */
    UCHAR               fx_media_fat_secondary_update_map[FX_FAT_MAP_SIZE];

    /* Define a variable for the application's use.  */
    ALIGN_TYPE          fx_media_reserved_for_user;

    /* Define an area to allocate long file names so that local storage on
       calling thread's stack is not used for long file names.  This helps
       reduce the amount of thread stack space needed when using FileX.  */
    CHAR                fx_media_name_buffer[4*FX_MAX_LONG_NAME_LEN];

#ifdef FX_RENAME_PATH_INHERIT

    /* Define the file and directory rename buffer that will be used to prepend
       paths when necessary to the target file name.  */
    CHAR                fx_media_rename_buffer[FX_MAXIMUM_PATH];
#endif

#ifndef FX_DISABLE_CACHE
    /* Define the sector cache control structures for this media.  */
    struct FX_CACHED_SECTOR_STRUCT fx_media_sector_cache[FX_MAX_SECTOR_CACHE];

    /* Define the sector cache hash mask so that the hash algorithm can be used with
       any power of 2 number of cache sectors.  */
    ULONG               fx_media_sector_cache_hash_mask;
#endif /* FX_DISABLE_CACHE */

    /* Define a variable to disable burst cache. This is used by the underlying
       driver.  */
    ULONG               fx_media_disable_burst_cache;

#ifdef FX_ENABLE_FAULT_TOLERANT

    /* Fault tolerant information */
    /* Indicate whether fault tolerant is enabled. */

    UCHAR               fx_media_fault_tolerant_enabled;

    /* State of fault tolerant operation. */
    UCHAR               fx_media_fault_tolerant_state;

    /* Transaction recursive count. */
    USHORT              fx_media_fault_tolerant_transaction_count;

    /* Start cluster of log. */
    ULONG               fx_media_fault_tolerant_start_cluster;

    /* Count of consecutive clusters of log. */
    ULONG               fx_media_fault_tolerant_clusters;

    /* Count of total logs. */
    ULONG               fx_media_fault_tolerant_total_logs;

    /* Pointer to the memory buffer area used for fault tolerant operations.  */
    UCHAR              *fx_media_fault_tolerant_memory_buffer;

    /* Size of memory buffer area used for fault tolerant operations. */
    ULONG               fx_media_fault_tolerant_memory_buffer_size;

    /* Size of log file. */
    ULONG               fx_media_fault_tolerant_file_size;

    /* Memory space used during the release of FAT list. */
    ULONG               fx_media_fault_tolerant_cache[FX_FAULT_TOLERANT_CACHE_SIZE >> 2];

    /* Sector number of cached FAT entries. */
    ULONG               fx_media_fault_tolerant_cached_FAT_sector;
#endif /* FX_ENABLE_FAULT_TOLERANT */

    /* Reserved value of FAT table. */
    ULONG               fx_media_fat_reserved;

    /* Last value of FAT table. */
    ULONG               fx_media_fat_last;

    /* Media geometry structure */
    UCHAR               fx_media_FAT_type;

    /* Define the module port extension in the media control block. This
       is typically defined to whitespace in fx_port.h.  */
//    FX_MEDIA_MODULE_EXTENSION

} FX_MEDIA;

typedef struct FX_FILE_STRUCT
{

    /* Define the file ID used for error checking.  */
    ULONG               fx_file_id;

    /* Define the file's name.  */
    CHAR                *fx_file_name;

    /* Define the open mode request.  */
    ULONG               fx_file_open_mode;

    /* Define the file modified field.  */
    UCHAR               fx_file_modified;

    /* Define the data storage parameters.  */
    ULONG               fx_file_total_clusters;
    ULONG               fx_file_first_physical_cluster;
    ULONG               fx_file_consecutive_cluster;
    ULONG               fx_file_last_physical_cluster;
    ULONG               fx_file_current_physical_cluster;
    ULONG64             fx_file_current_logical_sector;
    ULONG               fx_file_current_logical_offset;
    ULONG               fx_file_current_relative_cluster;
    ULONG               fx_file_current_relative_sector;
    ULONG64             fx_file_current_file_offset;
    ULONG64             fx_file_current_file_size;
    ULONG64             fx_file_current_available_size;
#ifdef FX_ENABLE_FAULT_TOLERANT
    ULONG64             fx_file_maximum_size_used;
#endif /* FX_ENABLE_FAULT_TOLERANT */

    /* Remember the media that is associated with this file. */
    FX_MEDIA            *fx_file_media_ptr;

    /* Define the pointers necessary to maintain the open file on
       the list of open files.  */
    struct FX_FILE_STRUCT
                        *fx_file_opened_next,
                        *fx_file_opened_previous;

    /* Define the complete directory entry structure.  */
    FX_DIR_ENTRY        fx_file_dir_entry;
    CHAR                fx_file_name_buffer[FX_MAX_LONG_NAME_LEN];

    /* Define a variable for the application's use */
    ULONG               fx_file_disable_burst_cache;

    /* Define a notify function called when file is written to. */
    VOID               (*fx_file_write_notify)(struct FX_FILE_STRUCT *);

    /* Define the module port extension in the file control block. This
       is typically defined to whitespace in fx_port.h.  */
//    FX_FILE_MODULE_EXTENSION

} FX_FILE;

typedef struct FX_LOCAL_PATH_STRUCT
{

    /* This structure must be mapped to match application's file system.  */
    ULONG   tbd;
} FX_LOCAL_PATH;


/* Define basic constants.  */

#define FX_OPEN_FOR_READ            0
#define FX_OPEN_FOR_WRITE           1

#define FX_MAX_LONG_NAME_LEN        256

#define FX_SUCCESS                  0x00
#define FX_NULL                     0
#define FX_ACCESS_ERROR             0x06
#define FX_END_OF_FILE              0x09
#define FX_NOT_A_FILE               0x05
#define FX_NO_MORE_ENTRIES          0x0F
#define FX_DIRECTORY                0x10
#define FX_READ_ONLY                0x01


/* Next define the FileX prototypes used in some network server applications.  */

UINT        fx_directory_attributes_read(FX_MEDIA *media_ptr, CHAR *directory_name, UINT *attributes_ptr);
UINT        fx_directory_attributes_set(FX_MEDIA *media_ptr, CHAR *directory_name, UINT attributes);
UINT        fx_directory_create(FX_MEDIA *media_ptr, CHAR *directory_name);
UINT        fx_directory_delete(FX_MEDIA *media_ptr, CHAR *directory_name);
UINT        fx_directory_rename(FX_MEDIA *media_ptr, CHAR *old_directory_name, CHAR *new_directory_name);
UINT        fx_directory_first_entry_find(FX_MEDIA *media_ptr, CHAR *directory_name);
UINT        fx_directory_first_full_entry_find(FX_MEDIA *media_ptr, CHAR *directory_name, UINT *attributes,
                ULONG *size, UINT *year, UINT *month, UINT *day, UINT *hour, UINT *minute, UINT *second);
UINT        fx_directory_next_entry_find(FX_MEDIA *media_ptr, CHAR *directory_name);
UINT        fx_directory_next_full_entry_find(FX_MEDIA *media_ptr, CHAR *directory_name, UINT *attributes,
                ULONG *size, UINT *year, UINT *month, UINT *day, UINT *hour, UINT *minute, UINT *second);
UINT        fx_directory_name_test(FX_MEDIA *media_ptr, CHAR *directory_name);
UINT        fx_directory_information_get(FX_MEDIA *media_ptr, CHAR *directory_name, UINT *attributes, ULONG *size,
                                        UINT *year, UINT *month, UINT *day, UINT *hour, UINT *minute, UINT *second);
UINT        fx_directory_default_set(FX_MEDIA *media_ptr, CHAR *new_path_name);
UINT        fx_directory_default_get(FX_MEDIA *media_ptr, CHAR **return_path_name);

UINT        fx_file_best_effort_allocate(FX_FILE *file_ptr, ULONG size, ULONG *actual_size_allocated);
UINT        fx_file_create(FX_MEDIA *media_ptr, CHAR *file_name);
UINT        fx_file_delete(FX_MEDIA *media_ptr, CHAR *file_name);
UINT        fx_file_rename(FX_MEDIA *media_ptr, CHAR *old_file_name, CHAR *new_file_name);
UINT        fx_file_attributes_set(FX_MEDIA *media_ptr, CHAR *file_name, UINT attributes);
UINT        fx_file_attributes_read(FX_MEDIA *media_ptr, CHAR *file_name, UINT *attributes_ptr);
UINT        fx_file_open(FX_MEDIA *media_ptr, FX_FILE *file_ptr, CHAR *file_name,
                UINT open_type);
UINT        fx_file_close(FX_FILE *file_ptr);
UINT        fx_file_read(FX_FILE *file_ptr, VOID *buffer_ptr, ULONG request_size, ULONG *actual_size);
UINT        fx_file_write(FX_FILE *file_ptr, VOID *buffer_ptr, ULONG size);
UINT        fx_file_allocate(FX_FILE *file_ptr, ULONG size);
UINT        fx_file_relative_seek(FX_FILE *file_ptr, ULONG byte_offset, UINT seek_from);
UINT        fx_file_seek(FX_FILE *file_ptr, ULONG byte_offset);
UINT        fx_file_truncate(FX_FILE *file_ptr, ULONG size);
UINT        fx_file_truncate_release(FX_FILE *file_ptr, ULONG size);
UINT        fx_directory_local_path_restore(FX_MEDIA *media_ptr, FX_LOCAL_PATH *local_path_ptr);
UINT        fx_directory_local_path_set(FX_MEDIA *media_ptr, FX_LOCAL_PATH *local_path_ptr, const CHAR *new_path_name);
UINT        fx_directory_local_path_get(FX_MEDIA *media_ptr, CHAR **return_path_name);

UINT        fx_media_close(FX_MEDIA *media_ptr);

#endif
