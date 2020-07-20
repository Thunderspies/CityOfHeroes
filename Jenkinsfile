pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                // "/m:n" where N is CPU count. Leaving N out uses the system's core count
                bat 'msbuild build/vs2019/master.sln /m /p:BuildInParallel=true /p:Configuration=Release /p:Platform=x86'
            }
        }

        /*stage('Documentation') {
            steps {
                bat 'doxygen'
            }
        }*/

        stage('Archive') {
            steps {
                // TODO: Change "Source" to use repository name instead, or even the Jenkins job being ran
                zip zipFile: "Source.${GIT_BRANCH}-${GIT_COMMIT}.zip", archive: true, dir: 'bin', glob: '*.exe, *.pdb, *.dll, config.txt, version.ini, etc/config.txt, data/server/db/*.cfg, data/server/db/*.db'
            }
        }
    }
}
