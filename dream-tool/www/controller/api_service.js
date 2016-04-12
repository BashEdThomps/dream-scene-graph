App.service('ApiService',
    ['$http',
    function($http) {
        this.runDreamProject = function(projectName, callback) {
			$http({
				url: "/run",
				method: "POST",
				data: projectName
			}).then(
                function successCallback() {
				    callback(true);
			     }, function errorCallback() {
				    callback(false);
			    }
            );
        };

        this.uploadResource = function(path,data,callback) {
            $http({
                url    : path,
                data   : {data: data},
                method : "POST"
            }).then(
                function successCallback() {
                    callback(true);
                },function errorCallback() {
                    callback(false);
                }
            );
        };

        this.createProjectDirectory = function(projectUUID,callback) {
            $http({
                url    : '/create',
                data   : {uuid : projectUUID},
                method : 'POST'
            }).then(function success() {
                callback(true);
            }, function error() {
                callback(false);
            });
        };

        this.saveProjectFile = function(projObj,callback) {
            $http({
                url    : '/save/'+projObj.uuid,
                method : 'POST',
                data   : { project : projObj }
            }).then(function success() {
                callback(true);
            }, function failure() {
                callback(false);
            });
        };

        this.saveProjectTarball = function(projObj,callback) {
            console.log("Saveing project as tarball...");
            $http({
                url    : '/compress/'+projObj.uuid,
                method : 'GET'
            }).then(function success(resp){
                callback(true,resp);
            },function failure(resp){
                callback(false,resp);
            });
        };

        this.readProjectList = function (callback) {
            $http({
                url    : "/projectlist",
                method : 'GET',
            }).then(function success(resp) {
                callback(resp.data);
            },function error(resp){
                callback(null);
            });
        };

        return this;
    }
]);
