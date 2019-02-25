module.exports = {
  targets: {
	  BSVC: {
		  	cmd: "cd `git rev-parse --show-toplevel` && make PLATFORM=BSVC run",
			sh: true,
		    functionMatch: function (output) {
		      const enterDir = /^make\[\d+\]: Entering directory '([^']+)'$/;
		  	const error = /^([^:]+):(\d+):(\d+): error: (.+)$/;
		      // this is the list of error matches that atom-build will process
		      const array = [];
		      // stores the current directory
		      var dir = null;
		      // iterate over the output by lines
		      output.split(/\r?\n/).forEach(line => {
		        // update the current directory on lines with `Entering directory`
		        const dir_match = enterDir.exec(line);
		        if (dir_match) {
		          dir = dir_match[1];
		        } else {
		          // process possible error messages
		          const error_match = error.exec(line);
		          if (error_match) {
		            // map the regex match to the error object that atom-build expects
		            array.push({
		              file: error_match[1],
		              line: error_match[2],
		              col: error_match[3],
		              message: error_match[4]
		            });
		          }
		        }
		      });
		      return array;
		    }
	  },
	  Blitz: {
		  	cmd: "cd `git rev-parse --show-toplevel` && make PLATFORM=Blitz",
			sh: true,
		    functionMatch: function (output) {
		      const enterDir = /^make\[\d+\]: Entering directory '([^']+)'$/;
		  	const error = /^([^:]+):(\d+):(\d+): error: (.+)$/;
		      // this is the list of error matches that atom-build will process
		      const array = [];
		      // stores the current directory
		      var dir = null;
		      // iterate over the output by lines
		      output.split(/\r?\n/).forEach(line => {
		        // update the current directory on lines with `Entering directory`
		        const dir_match = enterDir.exec(line);
		        if (dir_match) {
		          dir = dir_match[1];
		        } else {
		          // process possible error messages
		          const error_match = error.exec(line);
		          if (error_match) {
		            // map the regex match to the error object that atom-build expects
		            array.push({
		              file: error_match[1],
		              line: error_match[2],
		              col: error_match[3],
		              message: error_match[4]
		            });
		          }
		        }
		      });
		      return array;
		    }
	  }
  },
};
