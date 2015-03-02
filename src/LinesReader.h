#ifndef FASTREAD_LinesReader_H
#define FASTREAD_LinesReader_H

template <typename Source>
class LinesReader {
public:
  LinesReader( Source& source_ ) : source(source_){}

  Rcpp::CharacterVector read( int n ){
    if(n>0) return read_n_lines(n) ;
    return read_all_lines();
  }

private:
  Source& source ;

  Rcpp::CharacterVector read_n_lines(int n){
    Rcpp::CharacterVector res(n) ;
    for(int i=0; i<n; i++){
      bool more = source.ensure_full_line() ;
      res[i] = source.get_line() ;
      // TODO: perhaps with a warning
      if(!more) break ;
    }
    return res ;
  }

  Rcpp::CharacterVector read_all_lines(){
    int n=0 ;
    std::vector<SEXP> lines ; lines.reserve(1000) ;
    bool more=true ;
    while(more){
      more = source.ensure_full_line() ;
      lines.push_back( source.get_line() );
      n++ ;
    }
    // if the last line is empty, dump it.
    if( std::string("") == CHAR(lines[n-1]) ) n-- ;

    Rcpp::CharacterVector res(n) ;
    for( int i=0; i<n; i++){
      res[i] = lines[i] ;
    }
    return res ;
  }

} ;

#endif
