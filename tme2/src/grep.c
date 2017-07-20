void extract_from_file(char*parse_expr,char*src——filename){
	char*line_buff = (char)malloc(LINE_MAX*sizeof(char));
	printf("opening%s\n",src_filename);
	FILE*src_fd;
	if((src_fd = fopen(src_filename,"r"))==NULL){
		perror("fopen");
		exit(1);	
	}
	while(fgets(line_buff,LINE_MAX,src_fd)!=NULL){
		if(strstr(line_buff,parse_expr)!=NULL){
			printf("%s %s",src_filename,line_buff);
		}	
	}
}

int browse_directory(char*parse_expr,char*dir_path){
	DIR*dir_ptr;
	struct dirent*dir_ent;
	int ch_nb=0;
	dir_ptr = opendir(dir_path);
	while((dir_ent = readdir(dir_ptr))!= NULL){
		if(S_ISREG(DTTOIF(dir_ent->d_type))){
			if(fork()==0){
				char*filepath = (char*)malloc(strlen(dir_path))+strlen(dir_ent->d_name)+1)*sizeof(char);
				sprintf(filepath,%s/%s,dir_path,dir_ent->d_name);
				extract_from_file(parse_expr,filepath);
				exit(0);			
			}else{
				ch_nb++;			
			}	
		}
		closedir(dir_ptr);
		printf("nb de fils = %d \n",ch_nb);
		return ch_nb;
	}

}
