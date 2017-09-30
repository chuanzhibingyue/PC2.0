webpackJsonp([0],{

/***/ 0:
/***/ function(module, exports, __webpack_require__) {

	/* WEBPACK VAR INJECTION */(function(Util, gkClientInterface) {//入口文件
	__webpack_require__(97);
	
	__webpack_require__(98);
	__webpack_require__(106);
	__webpack_require__(112);
	
	__webpack_require__(5);
	__webpack_require__(63);
	
	//第三方依赖模块
	__webpack_require__(183);
	__webpack_require__(185);
	__webpack_require__(187);
	__webpack_require__(189);
	
	angular.module('goukuai.createLib', [
	        'ui.bootstrap',
	        'goukuai.common'
	    ])
	
	    .controller('initCtrl', ['$scope', '$q', 'gkApi', 'gkException', '$window', function ($scope, $q, gkApi, gkException, $window) {
	
	        //判断客户端数据加载完毕
	
	        var loadCompleteCallback = function () {
	            gkApi.entInfo().then(function (re) {
	                Util.Log.log('gkApi.entInfo:re', re);
	                var ents = re && angular.isArray(re.list) ? re.list : [];
	                if (!ents || !ents.length) {
	                    $scope.errorText = '你还没加入任何企业,不能创建库';
	                    return;
	                }
	
	                /*
	                 这里可能返回多个企业,全时这边只拿一个
	                 */
	                //$scope.ents = _.where(ents, {
	                //    enable_create_org: 1
	                //});
	                var ent = _.findWhere(ents, {
	                    enable_create_org: 1
	                });
	
	                if (!ent) {
	                    $scope.errorText = '你还没加入任何企业,不能创建库';
	                    return;
	                }
	
	                $scope.ents = [ent];
	
	                var promiseArr = [];
	                angular.forEach($scope.ents, function (ent) {
	                    promiseArr.push(gkApi.getStoragePoints(ent.ent_id));
	                });
	
	                var OnGKCloudLibCreateLoadComplete = top.window.uc.modules.cloud.CloudLibCreate.OnGKCloudLibCreateLoadComplete;
	
	                $q.all(promiseArr).then(function (ret) {
	                    angular.isFunction(OnGKCloudLibCreateLoadComplete) && OnGKCloudLibCreateLoadComplete();
	                    angular.forEach($scope.ents, function (ent, i) {
	                        ent.storagePoints = ret[i].list;
	                    });
	
	                    $scope.lib = {
	                        ent: $scope.ents[0],
	                        name: '',
	                        storagePoint: $scope.ents[0].storagePoints[0]
	                    };
	
	                    $scope.showStoragePoint = $scope.lib.ent && $scope.lib.ent.storagePoints && $scope.lib.ent.storagePoints.length > 1;
	                    $scope.showEnt = $scope.ents && $scope.ents.length > 1;
	
	                }, function () {
	                    angular.isFunction(OnGKCloudLibCreateLoadComplete) && OnGKCloudLibCreateLoadComplete();
	                    $scope.errorText = '无法获取企业存储节点数据';
	                });
	            });
	        };
	
	        gkClientInterface.needLoading(function (re) {
	            Util.Log.log('needLoading', re);
	            if (re == 0) {
	                loadCompleteCallback();
	            } else if (re == 1) {
	                loadCompleteCallback();
	            }
	        });
	
	        var mounts;
	        var checkMountNameAvaiable = function (name) {
	            var deferred = $q.defer();
	            var check = function () {
	                if(_.findWhere(mounts, {
	                        name: name
	                    })){
	                    deferred.reject();
	                }else{
	                    deferred.resolve();
	                }
	            };
	            if (angular.isDefined(mounts)) {
	                check();
	            } else {
	                gkApi.getMounts({
	                    sidetype:'org'
	                }).then(function (data) {
	                    mounts = data.list || [];
	                    check();
	                });
	            }
	            return deferred.promise;
	        };
	
	
	        $scope.createLoading = false;
	        $scope.createLib = function (lib) {
	            if ($scope.createLoading) {
	                return;
	            }
	            $scope.createLoading = true;
	            checkMountNameAvaiable(lib.name).then(function(){
	                gkApi.createLib(lib.ent.ent_id, lib.name,
	                    lib.storagePoint.storage_point, undefined
	                ).then(function (re) {
	                    Util.Log.log('gkApi.entInfo:success:re', re);
	                    var mount_id = re.mount_id;
	                    gkApi.notice({
	                        type: 'getOrg',
	                        'org_id': Number(re.org_id)
	                    }).then(function () {
	                        top.window.uc.modules.cloud.gkEventListener.OnGkEventListener("createlibSuccess", {"mountId": mount_id});
	                    });
	
	                }, function (request, textStatus, errorThrown) {
	                    Util.Log.log('gkApi.entInfo:error:errors', request, textStatus, errorThrown);
	                    $scope.createLoading = false;
	                    var error = gkException.getAjaxError(request, textStatus, errorThrown);
	                    window.top.uc.modules.component.PromptMessageBox.open({
	                            title: '提示',
	                            message: error.msg,
	                            buttonMode: "ok",
	                            OkText: '确定'
	                        },
	                        function (result) {
	                            this.close();
	                        });
	                });
	            },function(){
	                $scope.createLoading = false;
	                window.top.uc.modules.component.PromptMessageBox.open({
	                        title: '提示',
	                        message:'已存在相同名称文件库',
	                        buttonMode: "ok",
	                        OkText: '确定'
	                    },
	                    function (result) {
	                        this.close();
	                    });
	            });
	        };
	
	        $scope.disableCreateBtn = true;
	        $scope.$watch('lib.name', function (val) {
	            $scope.disableCreateBtn = !val || !val.length;
	        });
	
	        $scope.closeModal = function () {
	            top.window.uc.modules.cloud.gkEventListener.OnGkEventListener("cancel", null);
	        };
	
	    }])
	    .factory('gkModal', __webpack_require__(73))
	;
	/* WEBPACK VAR INJECTION */}.call(exports, __webpack_require__(1), __webpack_require__(15)))

/***/ },

/***/ 97:
/***/ function(module, exports) {

	
	module.exports = "createLib.html"

/***/ },

/***/ 112:
/***/ function(module, exports) {

	// removed by extract-text-webpack-plugin

/***/ },

/***/ 183:
/***/ function(module, exports, __webpack_require__) {

	/* WEBPACK VAR INJECTION */(function(global) {module.exports = global["_"] = __webpack_require__(184);
	/* WEBPACK VAR INJECTION */}.call(exports, (function() { return this; }())))

/***/ },

/***/ 184:
/***/ function(module, exports, __webpack_require__) {

	var __WEBPACK_AMD_DEFINE_ARRAY__, __WEBPACK_AMD_DEFINE_RESULT__;//     Underscore.js 1.8.3
	//     http://underscorejs.org
	//     (c) 2009-2015 Jeremy Ashkenas, DocumentCloud and Investigative Reporters & Editors
	//     Underscore may be freely distributed under the MIT license.
	
	(function() {
	
	  // Baseline setup
	  // --------------
	
	  // Establish the root object, `window` in the browser, or `exports` on the server.
	  var root = this;
	
	  // Save the previous value of the `_` variable.
	  var previousUnderscore = root._;
	
	  // Save bytes in the minified (but not gzipped) version:
	  var ArrayProto = Array.prototype, ObjProto = Object.prototype, FuncProto = Function.prototype;
	
	  // Create quick reference variables for speed access to core prototypes.
	  var
	    push             = ArrayProto.push,
	    slice            = ArrayProto.slice,
	    toString         = ObjProto.toString,
	    hasOwnProperty   = ObjProto.hasOwnProperty;
	
	  // All **ECMAScript 5** native function implementations that we hope to use
	  // are declared here.
	  var
	    nativeIsArray      = Array.isArray,
	    nativeKeys         = Object.keys,
	    nativeBind         = FuncProto.bind,
	    nativeCreate       = Object.create;
	
	  // Naked function reference for surrogate-prototype-swapping.
	  var Ctor = function(){};
	
	  // Create a safe reference to the Underscore object for use below.
	  var _ = function(obj) {
	    if (obj instanceof _) return obj;
	    if (!(this instanceof _)) return new _(obj);
	    this._wrapped = obj;
	  };
	
	  // Export the Underscore object for **Node.js**, with
	  // backwards-compatibility for the old `require()` API. If we're in
	  // the browser, add `_` as a global object.
	  if (true) {
	    if (typeof module !== 'undefined' && module.exports) {
	      exports = module.exports = _;
	    }
	    exports._ = _;
	  } else {
	    root._ = _;
	  }
	
	  // Current version.
	  _.VERSION = '1.8.3';
	
	  // Internal function that returns an efficient (for current engines) version
	  // of the passed-in callback, to be repeatedly applied in other Underscore
	  // functions.
	  var optimizeCb = function(func, context, argCount) {
	    if (context === void 0) return func;
	    switch (argCount == null ? 3 : argCount) {
	      case 1: return function(value) {
	        return func.call(context, value);
	      };
	      case 2: return function(value, other) {
	        return func.call(context, value, other);
	      };
	      case 3: return function(value, index, collection) {
	        return func.call(context, value, index, collection);
	      };
	      case 4: return function(accumulator, value, index, collection) {
	        return func.call(context, accumulator, value, index, collection);
	      };
	    }
	    return function() {
	      return func.apply(context, arguments);
	    };
	  };
	
	  // A mostly-internal function to generate callbacks that can be applied
	  // to each element in a collection, returning the desired result — either
	  // identity, an arbitrary callback, a property matcher, or a property accessor.
	  var cb = function(value, context, argCount) {
	    if (value == null) return _.identity;
	    if (_.isFunction(value)) return optimizeCb(value, context, argCount);
	    if (_.isObject(value)) return _.matcher(value);
	    return _.property(value);
	  };
	  _.iteratee = function(value, context) {
	    return cb(value, context, Infinity);
	  };
	
	  // An internal function for creating assigner functions.
	  var createAssigner = function(keysFunc, undefinedOnly) {
	    return function(obj) {
	      var length = arguments.length;
	      if (length < 2 || obj == null) return obj;
	      for (var index = 1; index < length; index++) {
	        var source = arguments[index],
	            keys = keysFunc(source),
	            l = keys.length;
	        for (var i = 0; i < l; i++) {
	          var key = keys[i];
	          if (!undefinedOnly || obj[key] === void 0) obj[key] = source[key];
	        }
	      }
	      return obj;
	    };
	  };
	
	  // An internal function for creating a new object that inherits from another.
	  var baseCreate = function(prototype) {
	    if (!_.isObject(prototype)) return {};
	    if (nativeCreate) return nativeCreate(prototype);
	    Ctor.prototype = prototype;
	    var result = new Ctor;
	    Ctor.prototype = null;
	    return result;
	  };
	
	  var property = function(key) {
	    return function(obj) {
	      return obj == null ? void 0 : obj[key];
	    };
	  };
	
	  // Helper for collection methods to determine whether a collection
	  // should be iterated as an array or as an object
	  // Related: http://people.mozilla.org/~jorendorff/es6-draft.html#sec-tolength
	  // Avoids a very nasty iOS 8 JIT bug on ARM-64. #2094
	  var MAX_ARRAY_INDEX = Math.pow(2, 53) - 1;
	  var getLength = property('length');
	  var isArrayLike = function(collection) {
	    var length = getLength(collection);
	    return typeof length == 'number' && length >= 0 && length <= MAX_ARRAY_INDEX;
	  };
	
	  // Collection Functions
	  // --------------------
	
	  // The cornerstone, an `each` implementation, aka `forEach`.
	  // Handles raw objects in addition to array-likes. Treats all
	  // sparse array-likes as if they were dense.
	  _.each = _.forEach = function(obj, iteratee, context) {
	    iteratee = optimizeCb(iteratee, context);
	    var i, length;
	    if (isArrayLike(obj)) {
	      for (i = 0, length = obj.length; i < length; i++) {
	        iteratee(obj[i], i, obj);
	      }
	    } else {
	      var keys = _.keys(obj);
	      for (i = 0, length = keys.length; i < length; i++) {
	        iteratee(obj[keys[i]], keys[i], obj);
	      }
	    }
	    return obj;
	  };
	
	  // Return the results of applying the iteratee to each element.
	  _.map = _.collect = function(obj, iteratee, context) {
	    iteratee = cb(iteratee, context);
	    var keys = !isArrayLike(obj) && _.keys(obj),
	        length = (keys || obj).length,
	        results = Array(length);
	    for (var index = 0; index < length; index++) {
	      var currentKey = keys ? keys[index] : index;
	      results[index] = iteratee(obj[currentKey], currentKey, obj);
	    }
	    return results;
	  };
	
	  // Create a reducing function iterating left or right.
	  function createReduce(dir) {
	    // Optimized iterator function as using arguments.length
	    // in the main function will deoptimize the, see #1991.
	    function iterator(obj, iteratee, memo, keys, index, length) {
	      for (; index >= 0 && index < length; index += dir) {
	        var currentKey = keys ? keys[index] : index;
	        memo = iteratee(memo, obj[currentKey], currentKey, obj);
	      }
	      return memo;
	    }
	
	    return function(obj, iteratee, memo, context) {
	      iteratee = optimizeCb(iteratee, context, 4);
	      var keys = !isArrayLike(obj) && _.keys(obj),
	          length = (keys || obj).length,
	          index = dir > 0 ? 0 : length - 1;
	      // Determine the initial value if none is provided.
	      if (arguments.length < 3) {
	        memo = obj[keys ? keys[index] : index];
	        index += dir;
	      }
	      return iterator(obj, iteratee, memo, keys, index, length);
	    };
	  }
	
	  // **Reduce** builds up a single result from a list of values, aka `inject`,
	  // or `foldl`.
	  _.reduce = _.foldl = _.inject = createReduce(1);
	
	  // The right-associative version of reduce, also known as `foldr`.
	  _.reduceRight = _.foldr = createReduce(-1);
	
	  // Return the first value which passes a truth test. Aliased as `detect`.
	  _.find = _.detect = function(obj, predicate, context) {
	    var key;
	    if (isArrayLike(obj)) {
	      key = _.findIndex(obj, predicate, context);
	    } else {
	      key = _.findKey(obj, predicate, context);
	    }
	    if (key !== void 0 && key !== -1) return obj[key];
	  };
	
	  // Return all the elements that pass a truth test.
	  // Aliased as `select`.
	  _.filter = _.select = function(obj, predicate, context) {
	    var results = [];
	    predicate = cb(predicate, context);
	    _.each(obj, function(value, index, list) {
	      if (predicate(value, index, list)) results.push(value);
	    });
	    return results;
	  };
	
	  // Return all the elements for which a truth test fails.
	  _.reject = function(obj, predicate, context) {
	    return _.filter(obj, _.negate(cb(predicate)), context);
	  };
	
	  // Determine whether all of the elements match a truth test.
	  // Aliased as `all`.
	  _.every = _.all = function(obj, predicate, context) {
	    predicate = cb(predicate, context);
	    var keys = !isArrayLike(obj) && _.keys(obj),
	        length = (keys || obj).length;
	    for (var index = 0; index < length; index++) {
	      var currentKey = keys ? keys[index] : index;
	      if (!predicate(obj[currentKey], currentKey, obj)) return false;
	    }
	    return true;
	  };
	
	  // Determine if at least one element in the object matches a truth test.
	  // Aliased as `any`.
	  _.some = _.any = function(obj, predicate, context) {
	    predicate = cb(predicate, context);
	    var keys = !isArrayLike(obj) && _.keys(obj),
	        length = (keys || obj).length;
	    for (var index = 0; index < length; index++) {
	      var currentKey = keys ? keys[index] : index;
	      if (predicate(obj[currentKey], currentKey, obj)) return true;
	    }
	    return false;
	  };
	
	  // Determine if the array or object contains a given item (using `===`).
	  // Aliased as `includes` and `include`.
	  _.contains = _.includes = _.include = function(obj, item, fromIndex, guard) {
	    if (!isArrayLike(obj)) obj = _.values(obj);
	    if (typeof fromIndex != 'number' || guard) fromIndex = 0;
	    return _.indexOf(obj, item, fromIndex) >= 0;
	  };
	
	  // Invoke a method (with arguments) on every item in a collection.
	  _.invoke = function(obj, method) {
	    var args = slice.call(arguments, 2);
	    var isFunc = _.isFunction(method);
	    return _.map(obj, function(value) {
	      var func = isFunc ? method : value[method];
	      return func == null ? func : func.apply(value, args);
	    });
	  };
	
	  // Convenience version of a common use case of `map`: fetching a property.
	  _.pluck = function(obj, key) {
	    return _.map(obj, _.property(key));
	  };
	
	  // Convenience version of a common use case of `filter`: selecting only objects
	  // containing specific `key:value` pairs.
	  _.where = function(obj, attrs) {
	    return _.filter(obj, _.matcher(attrs));
	  };
	
	  // Convenience version of a common use case of `find`: getting the first object
	  // containing specific `key:value` pairs.
	  _.findWhere = function(obj, attrs) {
	    return _.find(obj, _.matcher(attrs));
	  };
	
	  // Return the maximum element (or element-based computation).
	  _.max = function(obj, iteratee, context) {
	    var result = -Infinity, lastComputed = -Infinity,
	        value, computed;
	    if (iteratee == null && obj != null) {
	      obj = isArrayLike(obj) ? obj : _.values(obj);
	      for (var i = 0, length = obj.length; i < length; i++) {
	        value = obj[i];
	        if (value > result) {
	          result = value;
	        }
	      }
	    } else {
	      iteratee = cb(iteratee, context);
	      _.each(obj, function(value, index, list) {
	        computed = iteratee(value, index, list);
	        if (computed > lastComputed || computed === -Infinity && result === -Infinity) {
	          result = value;
	          lastComputed = computed;
	        }
	      });
	    }
	    return result;
	  };
	
	  // Return the minimum element (or element-based computation).
	  _.min = function(obj, iteratee, context) {
	    var result = Infinity, lastComputed = Infinity,
	        value, computed;
	    if (iteratee == null && obj != null) {
	      obj = isArrayLike(obj) ? obj : _.values(obj);
	      for (var i = 0, length = obj.length; i < length; i++) {
	        value = obj[i];
	        if (value < result) {
	          result = value;
	        }
	      }
	    } else {
	      iteratee = cb(iteratee, context);
	      _.each(obj, function(value, index, list) {
	        computed = iteratee(value, index, list);
	        if (computed < lastComputed || computed === Infinity && result === Infinity) {
	          result = value;
	          lastComputed = computed;
	        }
	      });
	    }
	    return result;
	  };
	
	  // Shuffle a collection, using the modern version of the
	  // [Fisher-Yates shuffle](http://en.wikipedia.org/wiki/Fisher–Yates_shuffle).
	  _.shuffle = function(obj) {
	    var set = isArrayLike(obj) ? obj : _.values(obj);
	    var length = set.length;
	    var shuffled = Array(length);
	    for (var index = 0, rand; index < length; index++) {
	      rand = _.random(0, index);
	      if (rand !== index) shuffled[index] = shuffled[rand];
	      shuffled[rand] = set[index];
	    }
	    return shuffled;
	  };
	
	  // Sample **n** random values from a collection.
	  // If **n** is not specified, returns a single random element.
	  // The internal `guard` argument allows it to work with `map`.
	  _.sample = function(obj, n, guard) {
	    if (n == null || guard) {
	      if (!isArrayLike(obj)) obj = _.values(obj);
	      return obj[_.random(obj.length - 1)];
	    }
	    return _.shuffle(obj).slice(0, Math.max(0, n));
	  };
	
	  // Sort the object's values by a criterion produced by an iteratee.
	  _.sortBy = function(obj, iteratee, context) {
	    iteratee = cb(iteratee, context);
	    return _.pluck(_.map(obj, function(value, index, list) {
	      return {
	        value: value,
	        index: index,
	        criteria: iteratee(value, index, list)
	      };
	    }).sort(function(left, right) {
	      var a = left.criteria;
	      var b = right.criteria;
	      if (a !== b) {
	        if (a > b || a === void 0) return 1;
	        if (a < b || b === void 0) return -1;
	      }
	      return left.index - right.index;
	    }), 'value');
	  };
	
	  // An internal function used for aggregate "group by" operations.
	  var group = function(behavior) {
	    return function(obj, iteratee, context) {
	      var result = {};
	      iteratee = cb(iteratee, context);
	      _.each(obj, function(value, index) {
	        var key = iteratee(value, index, obj);
	        behavior(result, value, key);
	      });
	      return result;
	    };
	  };
	
	  // Groups the object's values by a criterion. Pass either a string attribute
	  // to group by, or a function that returns the criterion.
	  _.groupBy = group(function(result, value, key) {
	    if (_.has(result, key)) result[key].push(value); else result[key] = [value];
	  });
	
	  // Indexes the object's values by a criterion, similar to `groupBy`, but for
	  // when you know that your index values will be unique.
	  _.indexBy = group(function(result, value, key) {
	    result[key] = value;
	  });
	
	  // Counts instances of an object that group by a certain criterion. Pass
	  // either a string attribute to count by, or a function that returns the
	  // criterion.
	  _.countBy = group(function(result, value, key) {
	    if (_.has(result, key)) result[key]++; else result[key] = 1;
	  });
	
	  // Safely create a real, live array from anything iterable.
	  _.toArray = function(obj) {
	    if (!obj) return [];
	    if (_.isArray(obj)) return slice.call(obj);
	    if (isArrayLike(obj)) return _.map(obj, _.identity);
	    return _.values(obj);
	  };
	
	  // Return the number of elements in an object.
	  _.size = function(obj) {
	    if (obj == null) return 0;
	    return isArrayLike(obj) ? obj.length : _.keys(obj).length;
	  };
	
	  // Split a collection into two arrays: one whose elements all satisfy the given
	  // predicate, and one whose elements all do not satisfy the predicate.
	  _.partition = function(obj, predicate, context) {
	    predicate = cb(predicate, context);
	    var pass = [], fail = [];
	    _.each(obj, function(value, key, obj) {
	      (predicate(value, key, obj) ? pass : fail).push(value);
	    });
	    return [pass, fail];
	  };
	
	  // Array Functions
	  // ---------------
	
	  // Get the first element of an array. Passing **n** will return the first N
	  // values in the array. Aliased as `head` and `take`. The **guard** check
	  // allows it to work with `_.map`.
	  _.first = _.head = _.take = function(array, n, guard) {
	    if (array == null) return void 0;
	    if (n == null || guard) return array[0];
	    return _.initial(array, array.length - n);
	  };
	
	  // Returns everything but the last entry of the array. Especially useful on
	  // the arguments object. Passing **n** will return all the values in
	  // the array, excluding the last N.
	  _.initial = function(array, n, guard) {
	    return slice.call(array, 0, Math.max(0, array.length - (n == null || guard ? 1 : n)));
	  };
	
	  // Get the last element of an array. Passing **n** will return the last N
	  // values in the array.
	  _.last = function(array, n, guard) {
	    if (array == null) return void 0;
	    if (n == null || guard) return array[array.length - 1];
	    return _.rest(array, Math.max(0, array.length - n));
	  };
	
	  // Returns everything but the first entry of the array. Aliased as `tail` and `drop`.
	  // Especially useful on the arguments object. Passing an **n** will return
	  // the rest N values in the array.
	  _.rest = _.tail = _.drop = function(array, n, guard) {
	    return slice.call(array, n == null || guard ? 1 : n);
	  };
	
	  // Trim out all falsy values from an array.
	  _.compact = function(array) {
	    return _.filter(array, _.identity);
	  };
	
	  // Internal implementation of a recursive `flatten` function.
	  var flatten = function(input, shallow, strict, startIndex) {
	    var output = [], idx = 0;
	    for (var i = startIndex || 0, length = getLength(input); i < length; i++) {
	      var value = input[i];
	      if (isArrayLike(value) && (_.isArray(value) || _.isArguments(value))) {
	        //flatten current level of array or arguments object
	        if (!shallow) value = flatten(value, shallow, strict);
	        var j = 0, len = value.length;
	        output.length += len;
	        while (j < len) {
	          output[idx++] = value[j++];
	        }
	      } else if (!strict) {
	        output[idx++] = value;
	      }
	    }
	    return output;
	  };
	
	  // Flatten out an array, either recursively (by default), or just one level.
	  _.flatten = function(array, shallow) {
	    return flatten(array, shallow, false);
	  };
	
	  // Return a version of the array that does not contain the specified value(s).
	  _.without = function(array) {
	    return _.difference(array, slice.call(arguments, 1));
	  };
	
	  // Produce a duplicate-free version of the array. If the array has already
	  // been sorted, you have the option of using a faster algorithm.
	  // Aliased as `unique`.
	  _.uniq = _.unique = function(array, isSorted, iteratee, context) {
	    if (!_.isBoolean(isSorted)) {
	      context = iteratee;
	      iteratee = isSorted;
	      isSorted = false;
	    }
	    if (iteratee != null) iteratee = cb(iteratee, context);
	    var result = [];
	    var seen = [];
	    for (var i = 0, length = getLength(array); i < length; i++) {
	      var value = array[i],
	          computed = iteratee ? iteratee(value, i, array) : value;
	      if (isSorted) {
	        if (!i || seen !== computed) result.push(value);
	        seen = computed;
	      } else if (iteratee) {
	        if (!_.contains(seen, computed)) {
	          seen.push(computed);
	          result.push(value);
	        }
	      } else if (!_.contains(result, value)) {
	        result.push(value);
	      }
	    }
	    return result;
	  };
	
	  // Produce an array that contains the union: each distinct element from all of
	  // the passed-in arrays.
	  _.union = function() {
	    return _.uniq(flatten(arguments, true, true));
	  };
	
	  // Produce an array that contains every item shared between all the
	  // passed-in arrays.
	  _.intersection = function(array) {
	    var result = [];
	    var argsLength = arguments.length;
	    for (var i = 0, length = getLength(array); i < length; i++) {
	      var item = array[i];
	      if (_.contains(result, item)) continue;
	      for (var j = 1; j < argsLength; j++) {
	        if (!_.contains(arguments[j], item)) break;
	      }
	      if (j === argsLength) result.push(item);
	    }
	    return result;
	  };
	
	  // Take the difference between one array and a number of other arrays.
	  // Only the elements present in just the first array will remain.
	  _.difference = function(array) {
	    var rest = flatten(arguments, true, true, 1);
	    return _.filter(array, function(value){
	      return !_.contains(rest, value);
	    });
	  };
	
	  // Zip together multiple lists into a single array -- elements that share
	  // an index go together.
	  _.zip = function() {
	    return _.unzip(arguments);
	  };
	
	  // Complement of _.zip. Unzip accepts an array of arrays and groups
	  // each array's elements on shared indices
	  _.unzip = function(array) {
	    var length = array && _.max(array, getLength).length || 0;
	    var result = Array(length);
	
	    for (var index = 0; index < length; index++) {
	      result[index] = _.pluck(array, index);
	    }
	    return result;
	  };
	
	  // Converts lists into objects. Pass either a single array of `[key, value]`
	  // pairs, or two parallel arrays of the same length -- one of keys, and one of
	  // the corresponding values.
	  _.object = function(list, values) {
	    var result = {};
	    for (var i = 0, length = getLength(list); i < length; i++) {
	      if (values) {
	        result[list[i]] = values[i];
	      } else {
	        result[list[i][0]] = list[i][1];
	      }
	    }
	    return result;
	  };
	
	  // Generator function to create the findIndex and findLastIndex functions
	  function createPredicateIndexFinder(dir) {
	    return function(array, predicate, context) {
	      predicate = cb(predicate, context);
	      var length = getLength(array);
	      var index = dir > 0 ? 0 : length - 1;
	      for (; index >= 0 && index < length; index += dir) {
	        if (predicate(array[index], index, array)) return index;
	      }
	      return -1;
	    };
	  }
	
	  // Returns the first index on an array-like that passes a predicate test
	  _.findIndex = createPredicateIndexFinder(1);
	  _.findLastIndex = createPredicateIndexFinder(-1);
	
	  // Use a comparator function to figure out the smallest index at which
	  // an object should be inserted so as to maintain order. Uses binary search.
	  _.sortedIndex = function(array, obj, iteratee, context) {
	    iteratee = cb(iteratee, context, 1);
	    var value = iteratee(obj);
	    var low = 0, high = getLength(array);
	    while (low < high) {
	      var mid = Math.floor((low + high) / 2);
	      if (iteratee(array[mid]) < value) low = mid + 1; else high = mid;
	    }
	    return low;
	  };
	
	  // Generator function to create the indexOf and lastIndexOf functions
	  function createIndexFinder(dir, predicateFind, sortedIndex) {
	    return function(array, item, idx) {
	      var i = 0, length = getLength(array);
	      if (typeof idx == 'number') {
	        if (dir > 0) {
	            i = idx >= 0 ? idx : Math.max(idx + length, i);
	        } else {
	            length = idx >= 0 ? Math.min(idx + 1, length) : idx + length + 1;
	        }
	      } else if (sortedIndex && idx && length) {
	        idx = sortedIndex(array, item);
	        return array[idx] === item ? idx : -1;
	      }
	      if (item !== item) {
	        idx = predicateFind(slice.call(array, i, length), _.isNaN);
	        return idx >= 0 ? idx + i : -1;
	      }
	      for (idx = dir > 0 ? i : length - 1; idx >= 0 && idx < length; idx += dir) {
	        if (array[idx] === item) return idx;
	      }
	      return -1;
	    };
	  }
	
	  // Return the position of the first occurrence of an item in an array,
	  // or -1 if the item is not included in the array.
	  // If the array is large and already in sort order, pass `true`
	  // for **isSorted** to use binary search.
	  _.indexOf = createIndexFinder(1, _.findIndex, _.sortedIndex);
	  _.lastIndexOf = createIndexFinder(-1, _.findLastIndex);
	
	  // Generate an integer Array containing an arithmetic progression. A port of
	  // the native Python `range()` function. See
	  // [the Python documentation](http://docs.python.org/library/functions.html#range).
	  _.range = function(start, stop, step) {
	    if (stop == null) {
	      stop = start || 0;
	      start = 0;
	    }
	    step = step || 1;
	
	    var length = Math.max(Math.ceil((stop - start) / step), 0);
	    var range = Array(length);
	
	    for (var idx = 0; idx < length; idx++, start += step) {
	      range[idx] = start;
	    }
	
	    return range;
	  };
	
	  // Function (ahem) Functions
	  // ------------------
	
	  // Determines whether to execute a function as a constructor
	  // or a normal function with the provided arguments
	  var executeBound = function(sourceFunc, boundFunc, context, callingContext, args) {
	    if (!(callingContext instanceof boundFunc)) return sourceFunc.apply(context, args);
	    var self = baseCreate(sourceFunc.prototype);
	    var result = sourceFunc.apply(self, args);
	    if (_.isObject(result)) return result;
	    return self;
	  };
	
	  // Create a function bound to a given object (assigning `this`, and arguments,
	  // optionally). Delegates to **ECMAScript 5**'s native `Function.bind` if
	  // available.
	  _.bind = function(func, context) {
	    if (nativeBind && func.bind === nativeBind) return nativeBind.apply(func, slice.call(arguments, 1));
	    if (!_.isFunction(func)) throw new TypeError('Bind must be called on a function');
	    var args = slice.call(arguments, 2);
	    var bound = function() {
	      return executeBound(func, bound, context, this, args.concat(slice.call(arguments)));
	    };
	    return bound;
	  };
	
	  // Partially apply a function by creating a version that has had some of its
	  // arguments pre-filled, without changing its dynamic `this` context. _ acts
	  // as a placeholder, allowing any combination of arguments to be pre-filled.
	  _.partial = function(func) {
	    var boundArgs = slice.call(arguments, 1);
	    var bound = function() {
	      var position = 0, length = boundArgs.length;
	      var args = Array(length);
	      for (var i = 0; i < length; i++) {
	        args[i] = boundArgs[i] === _ ? arguments[position++] : boundArgs[i];
	      }
	      while (position < arguments.length) args.push(arguments[position++]);
	      return executeBound(func, bound, this, this, args);
	    };
	    return bound;
	  };
	
	  // Bind a number of an object's methods to that object. Remaining arguments
	  // are the method names to be bound. Useful for ensuring that all callbacks
	  // defined on an object belong to it.
	  _.bindAll = function(obj) {
	    var i, length = arguments.length, key;
	    if (length <= 1) throw new Error('bindAll must be passed function names');
	    for (i = 1; i < length; i++) {
	      key = arguments[i];
	      obj[key] = _.bind(obj[key], obj);
	    }
	    return obj;
	  };
	
	  // Memoize an expensive function by storing its results.
	  _.memoize = function(func, hasher) {
	    var memoize = function(key) {
	      var cache = memoize.cache;
	      var address = '' + (hasher ? hasher.apply(this, arguments) : key);
	      if (!_.has(cache, address)) cache[address] = func.apply(this, arguments);
	      return cache[address];
	    };
	    memoize.cache = {};
	    return memoize;
	  };
	
	  // Delays a function for the given number of milliseconds, and then calls
	  // it with the arguments supplied.
	  _.delay = function(func, wait) {
	    var args = slice.call(arguments, 2);
	    return setTimeout(function(){
	      return func.apply(null, args);
	    }, wait);
	  };
	
	  // Defers a function, scheduling it to run after the current call stack has
	  // cleared.
	  _.defer = _.partial(_.delay, _, 1);
	
	  // Returns a function, that, when invoked, will only be triggered at most once
	  // during a given window of time. Normally, the throttled function will run
	  // as much as it can, without ever going more than once per `wait` duration;
	  // but if you'd like to disable the execution on the leading edge, pass
	  // `{leading: false}`. To disable execution on the trailing edge, ditto.
	  _.throttle = function(func, wait, options) {
	    var context, args, result;
	    var timeout = null;
	    var previous = 0;
	    if (!options) options = {};
	    var later = function() {
	      previous = options.leading === false ? 0 : _.now();
	      timeout = null;
	      result = func.apply(context, args);
	      if (!timeout) context = args = null;
	    };
	    return function() {
	      var now = _.now();
	      if (!previous && options.leading === false) previous = now;
	      var remaining = wait - (now - previous);
	      context = this;
	      args = arguments;
	      if (remaining <= 0 || remaining > wait) {
	        if (timeout) {
	          clearTimeout(timeout);
	          timeout = null;
	        }
	        previous = now;
	        result = func.apply(context, args);
	        if (!timeout) context = args = null;
	      } else if (!timeout && options.trailing !== false) {
	        timeout = setTimeout(later, remaining);
	      }
	      return result;
	    };
	  };
	
	  // Returns a function, that, as long as it continues to be invoked, will not
	  // be triggered. The function will be called after it stops being called for
	  // N milliseconds. If `immediate` is passed, trigger the function on the
	  // leading edge, instead of the trailing.
	  _.debounce = function(func, wait, immediate) {
	    var timeout, args, context, timestamp, result;
	
	    var later = function() {
	      var last = _.now() - timestamp;
	
	      if (last < wait && last >= 0) {
	        timeout = setTimeout(later, wait - last);
	      } else {
	        timeout = null;
	        if (!immediate) {
	          result = func.apply(context, args);
	          if (!timeout) context = args = null;
	        }
	      }
	    };
	
	    return function() {
	      context = this;
	      args = arguments;
	      timestamp = _.now();
	      var callNow = immediate && !timeout;
	      if (!timeout) timeout = setTimeout(later, wait);
	      if (callNow) {
	        result = func.apply(context, args);
	        context = args = null;
	      }
	
	      return result;
	    };
	  };
	
	  // Returns the first function passed as an argument to the second,
	  // allowing you to adjust arguments, run code before and after, and
	  // conditionally execute the original function.
	  _.wrap = function(func, wrapper) {
	    return _.partial(wrapper, func);
	  };
	
	  // Returns a negated version of the passed-in predicate.
	  _.negate = function(predicate) {
	    return function() {
	      return !predicate.apply(this, arguments);
	    };
	  };
	
	  // Returns a function that is the composition of a list of functions, each
	  // consuming the return value of the function that follows.
	  _.compose = function() {
	    var args = arguments;
	    var start = args.length - 1;
	    return function() {
	      var i = start;
	      var result = args[start].apply(this, arguments);
	      while (i--) result = args[i].call(this, result);
	      return result;
	    };
	  };
	
	  // Returns a function that will only be executed on and after the Nth call.
	  _.after = function(times, func) {
	    return function() {
	      if (--times < 1) {
	        return func.apply(this, arguments);
	      }
	    };
	  };
	
	  // Returns a function that will only be executed up to (but not including) the Nth call.
	  _.before = function(times, func) {
	    var memo;
	    return function() {
	      if (--times > 0) {
	        memo = func.apply(this, arguments);
	      }
	      if (times <= 1) func = null;
	      return memo;
	    };
	  };
	
	  // Returns a function that will be executed at most one time, no matter how
	  // often you call it. Useful for lazy initialization.
	  _.once = _.partial(_.before, 2);
	
	  // Object Functions
	  // ----------------
	
	  // Keys in IE < 9 that won't be iterated by `for key in ...` and thus missed.
	  var hasEnumBug = !{toString: null}.propertyIsEnumerable('toString');
	  var nonEnumerableProps = ['valueOf', 'isPrototypeOf', 'toString',
	                      'propertyIsEnumerable', 'hasOwnProperty', 'toLocaleString'];
	
	  function collectNonEnumProps(obj, keys) {
	    var nonEnumIdx = nonEnumerableProps.length;
	    var constructor = obj.constructor;
	    var proto = (_.isFunction(constructor) && constructor.prototype) || ObjProto;
	
	    // Constructor is a special case.
	    var prop = 'constructor';
	    if (_.has(obj, prop) && !_.contains(keys, prop)) keys.push(prop);
	
	    while (nonEnumIdx--) {
	      prop = nonEnumerableProps[nonEnumIdx];
	      if (prop in obj && obj[prop] !== proto[prop] && !_.contains(keys, prop)) {
	        keys.push(prop);
	      }
	    }
	  }
	
	  // Retrieve the names of an object's own properties.
	  // Delegates to **ECMAScript 5**'s native `Object.keys`
	  _.keys = function(obj) {
	    if (!_.isObject(obj)) return [];
	    if (nativeKeys) return nativeKeys(obj);
	    var keys = [];
	    for (var key in obj) if (_.has(obj, key)) keys.push(key);
	    // Ahem, IE < 9.
	    if (hasEnumBug) collectNonEnumProps(obj, keys);
	    return keys;
	  };
	
	  // Retrieve all the property names of an object.
	  _.allKeys = function(obj) {
	    if (!_.isObject(obj)) return [];
	    var keys = [];
	    for (var key in obj) keys.push(key);
	    // Ahem, IE < 9.
	    if (hasEnumBug) collectNonEnumProps(obj, keys);
	    return keys;
	  };
	
	  // Retrieve the values of an object's properties.
	  _.values = function(obj) {
	    var keys = _.keys(obj);
	    var length = keys.length;
	    var values = Array(length);
	    for (var i = 0; i < length; i++) {
	      values[i] = obj[keys[i]];
	    }
	    return values;
	  };
	
	  // Returns the results of applying the iteratee to each element of the object
	  // In contrast to _.map it returns an object
	  _.mapObject = function(obj, iteratee, context) {
	    iteratee = cb(iteratee, context);
	    var keys =  _.keys(obj),
	          length = keys.length,
	          results = {},
	          currentKey;
	      for (var index = 0; index < length; index++) {
	        currentKey = keys[index];
	        results[currentKey] = iteratee(obj[currentKey], currentKey, obj);
	      }
	      return results;
	  };
	
	  // Convert an object into a list of `[key, value]` pairs.
	  _.pairs = function(obj) {
	    var keys = _.keys(obj);
	    var length = keys.length;
	    var pairs = Array(length);
	    for (var i = 0; i < length; i++) {
	      pairs[i] = [keys[i], obj[keys[i]]];
	    }
	    return pairs;
	  };
	
	  // Invert the keys and values of an object. The values must be serializable.
	  _.invert = function(obj) {
	    var result = {};
	    var keys = _.keys(obj);
	    for (var i = 0, length = keys.length; i < length; i++) {
	      result[obj[keys[i]]] = keys[i];
	    }
	    return result;
	  };
	
	  // Return a sorted list of the function names available on the object.
	  // Aliased as `methods`
	  _.functions = _.methods = function(obj) {
	    var names = [];
	    for (var key in obj) {
	      if (_.isFunction(obj[key])) names.push(key);
	    }
	    return names.sort();
	  };
	
	  // Extend a given object with all the properties in passed-in object(s).
	  _.extend = createAssigner(_.allKeys);
	
	  // Assigns a given object with all the own properties in the passed-in object(s)
	  // (https://developer.mozilla.org/docs/Web/JavaScript/Reference/Global_Objects/Object/assign)
	  _.extendOwn = _.assign = createAssigner(_.keys);
	
	  // Returns the first key on an object that passes a predicate test
	  _.findKey = function(obj, predicate, context) {
	    predicate = cb(predicate, context);
	    var keys = _.keys(obj), key;
	    for (var i = 0, length = keys.length; i < length; i++) {
	      key = keys[i];
	      if (predicate(obj[key], key, obj)) return key;
	    }
	  };
	
	  // Return a copy of the object only containing the whitelisted properties.
	  _.pick = function(object, oiteratee, context) {
	    var result = {}, obj = object, iteratee, keys;
	    if (obj == null) return result;
	    if (_.isFunction(oiteratee)) {
	      keys = _.allKeys(obj);
	      iteratee = optimizeCb(oiteratee, context);
	    } else {
	      keys = flatten(arguments, false, false, 1);
	      iteratee = function(value, key, obj) { return key in obj; };
	      obj = Object(obj);
	    }
	    for (var i = 0, length = keys.length; i < length; i++) {
	      var key = keys[i];
	      var value = obj[key];
	      if (iteratee(value, key, obj)) result[key] = value;
	    }
	    return result;
	  };
	
	   // Return a copy of the object without the blacklisted properties.
	  _.omit = function(obj, iteratee, context) {
	    if (_.isFunction(iteratee)) {
	      iteratee = _.negate(iteratee);
	    } else {
	      var keys = _.map(flatten(arguments, false, false, 1), String);
	      iteratee = function(value, key) {
	        return !_.contains(keys, key);
	      };
	    }
	    return _.pick(obj, iteratee, context);
	  };
	
	  // Fill in a given object with default properties.
	  _.defaults = createAssigner(_.allKeys, true);
	
	  // Creates an object that inherits from the given prototype object.
	  // If additional properties are provided then they will be added to the
	  // created object.
	  _.create = function(prototype, props) {
	    var result = baseCreate(prototype);
	    if (props) _.extendOwn(result, props);
	    return result;
	  };
	
	  // Create a (shallow-cloned) duplicate of an object.
	  _.clone = function(obj) {
	    if (!_.isObject(obj)) return obj;
	    return _.isArray(obj) ? obj.slice() : _.extend({}, obj);
	  };
	
	  // Invokes interceptor with the obj, and then returns obj.
	  // The primary purpose of this method is to "tap into" a method chain, in
	  // order to perform operations on intermediate results within the chain.
	  _.tap = function(obj, interceptor) {
	    interceptor(obj);
	    return obj;
	  };
	
	  // Returns whether an object has a given set of `key:value` pairs.
	  _.isMatch = function(object, attrs) {
	    var keys = _.keys(attrs), length = keys.length;
	    if (object == null) return !length;
	    var obj = Object(object);
	    for (var i = 0; i < length; i++) {
	      var key = keys[i];
	      if (attrs[key] !== obj[key] || !(key in obj)) return false;
	    }
	    return true;
	  };
	
	
	  // Internal recursive comparison function for `isEqual`.
	  var eq = function(a, b, aStack, bStack) {
	    // Identical objects are equal. `0 === -0`, but they aren't identical.
	    // See the [Harmony `egal` proposal](http://wiki.ecmascript.org/doku.php?id=harmony:egal).
	    if (a === b) return a !== 0 || 1 / a === 1 / b;
	    // A strict comparison is necessary because `null == undefined`.
	    if (a == null || b == null) return a === b;
	    // Unwrap any wrapped objects.
	    if (a instanceof _) a = a._wrapped;
	    if (b instanceof _) b = b._wrapped;
	    // Compare `[[Class]]` names.
	    var className = toString.call(a);
	    if (className !== toString.call(b)) return false;
	    switch (className) {
	      // Strings, numbers, regular expressions, dates, and booleans are compared by value.
	      case '[object RegExp]':
	      // RegExps are coerced to strings for comparison (Note: '' + /a/i === '/a/i')
	      case '[object String]':
	        // Primitives and their corresponding object wrappers are equivalent; thus, `"5"` is
	        // equivalent to `new String("5")`.
	        return '' + a === '' + b;
	      case '[object Number]':
	        // `NaN`s are equivalent, but non-reflexive.
	        // Object(NaN) is equivalent to NaN
	        if (+a !== +a) return +b !== +b;
	        // An `egal` comparison is performed for other numeric values.
	        return +a === 0 ? 1 / +a === 1 / b : +a === +b;
	      case '[object Date]':
	      case '[object Boolean]':
	        // Coerce dates and booleans to numeric primitive values. Dates are compared by their
	        // millisecond representations. Note that invalid dates with millisecond representations
	        // of `NaN` are not equivalent.
	        return +a === +b;
	    }
	
	    var areArrays = className === '[object Array]';
	    if (!areArrays) {
	      if (typeof a != 'object' || typeof b != 'object') return false;
	
	      // Objects with different constructors are not equivalent, but `Object`s or `Array`s
	      // from different frames are.
	      var aCtor = a.constructor, bCtor = b.constructor;
	      if (aCtor !== bCtor && !(_.isFunction(aCtor) && aCtor instanceof aCtor &&
	                               _.isFunction(bCtor) && bCtor instanceof bCtor)
	                          && ('constructor' in a && 'constructor' in b)) {
	        return false;
	      }
	    }
	    // Assume equality for cyclic structures. The algorithm for detecting cyclic
	    // structures is adapted from ES 5.1 section 15.12.3, abstract operation `JO`.
	
	    // Initializing stack of traversed objects.
	    // It's done here since we only need them for objects and arrays comparison.
	    aStack = aStack || [];
	    bStack = bStack || [];
	    var length = aStack.length;
	    while (length--) {
	      // Linear search. Performance is inversely proportional to the number of
	      // unique nested structures.
	      if (aStack[length] === a) return bStack[length] === b;
	    }
	
	    // Add the first object to the stack of traversed objects.
	    aStack.push(a);
	    bStack.push(b);
	
	    // Recursively compare objects and arrays.
	    if (areArrays) {
	      // Compare array lengths to determine if a deep comparison is necessary.
	      length = a.length;
	      if (length !== b.length) return false;
	      // Deep compare the contents, ignoring non-numeric properties.
	      while (length--) {
	        if (!eq(a[length], b[length], aStack, bStack)) return false;
	      }
	    } else {
	      // Deep compare objects.
	      var keys = _.keys(a), key;
	      length = keys.length;
	      // Ensure that both objects contain the same number of properties before comparing deep equality.
	      if (_.keys(b).length !== length) return false;
	      while (length--) {
	        // Deep compare each member
	        key = keys[length];
	        if (!(_.has(b, key) && eq(a[key], b[key], aStack, bStack))) return false;
	      }
	    }
	    // Remove the first object from the stack of traversed objects.
	    aStack.pop();
	    bStack.pop();
	    return true;
	  };
	
	  // Perform a deep comparison to check if two objects are equal.
	  _.isEqual = function(a, b) {
	    return eq(a, b);
	  };
	
	  // Is a given array, string, or object empty?
	  // An "empty" object has no enumerable own-properties.
	  _.isEmpty = function(obj) {
	    if (obj == null) return true;
	    if (isArrayLike(obj) && (_.isArray(obj) || _.isString(obj) || _.isArguments(obj))) return obj.length === 0;
	    return _.keys(obj).length === 0;
	  };
	
	  // Is a given value a DOM element?
	  _.isElement = function(obj) {
	    return !!(obj && obj.nodeType === 1);
	  };
	
	  // Is a given value an array?
	  // Delegates to ECMA5's native Array.isArray
	  _.isArray = nativeIsArray || function(obj) {
	    return toString.call(obj) === '[object Array]';
	  };
	
	  // Is a given variable an object?
	  _.isObject = function(obj) {
	    var type = typeof obj;
	    return type === 'function' || type === 'object' && !!obj;
	  };
	
	  // Add some isType methods: isArguments, isFunction, isString, isNumber, isDate, isRegExp, isError.
	  _.each(['Arguments', 'Function', 'String', 'Number', 'Date', 'RegExp', 'Error'], function(name) {
	    _['is' + name] = function(obj) {
	      return toString.call(obj) === '[object ' + name + ']';
	    };
	  });
	
	  // Define a fallback version of the method in browsers (ahem, IE < 9), where
	  // there isn't any inspectable "Arguments" type.
	  if (!_.isArguments(arguments)) {
	    _.isArguments = function(obj) {
	      return _.has(obj, 'callee');
	    };
	  }
	
	  // Optimize `isFunction` if appropriate. Work around some typeof bugs in old v8,
	  // IE 11 (#1621), and in Safari 8 (#1929).
	  if (typeof /./ != 'function' && typeof Int8Array != 'object') {
	    _.isFunction = function(obj) {
	      return typeof obj == 'function' || false;
	    };
	  }
	
	  // Is a given object a finite number?
	  _.isFinite = function(obj) {
	    return isFinite(obj) && !isNaN(parseFloat(obj));
	  };
	
	  // Is the given value `NaN`? (NaN is the only number which does not equal itself).
	  _.isNaN = function(obj) {
	    return _.isNumber(obj) && obj !== +obj;
	  };
	
	  // Is a given value a boolean?
	  _.isBoolean = function(obj) {
	    return obj === true || obj === false || toString.call(obj) === '[object Boolean]';
	  };
	
	  // Is a given value equal to null?
	  _.isNull = function(obj) {
	    return obj === null;
	  };
	
	  // Is a given variable undefined?
	  _.isUndefined = function(obj) {
	    return obj === void 0;
	  };
	
	  // Shortcut function for checking if an object has a given property directly
	  // on itself (in other words, not on a prototype).
	  _.has = function(obj, key) {
	    return obj != null && hasOwnProperty.call(obj, key);
	  };
	
	  // Utility Functions
	  // -----------------
	
	  // Run Underscore.js in *noConflict* mode, returning the `_` variable to its
	  // previous owner. Returns a reference to the Underscore object.
	  _.noConflict = function() {
	    root._ = previousUnderscore;
	    return this;
	  };
	
	  // Keep the identity function around for default iteratees.
	  _.identity = function(value) {
	    return value;
	  };
	
	  // Predicate-generating functions. Often useful outside of Underscore.
	  _.constant = function(value) {
	    return function() {
	      return value;
	    };
	  };
	
	  _.noop = function(){};
	
	  _.property = property;
	
	  // Generates a function for a given object that returns a given property.
	  _.propertyOf = function(obj) {
	    return obj == null ? function(){} : function(key) {
	      return obj[key];
	    };
	  };
	
	  // Returns a predicate for checking whether an object has a given set of
	  // `key:value` pairs.
	  _.matcher = _.matches = function(attrs) {
	    attrs = _.extendOwn({}, attrs);
	    return function(obj) {
	      return _.isMatch(obj, attrs);
	    };
	  };
	
	  // Run a function **n** times.
	  _.times = function(n, iteratee, context) {
	    var accum = Array(Math.max(0, n));
	    iteratee = optimizeCb(iteratee, context, 1);
	    for (var i = 0; i < n; i++) accum[i] = iteratee(i);
	    return accum;
	  };
	
	  // Return a random integer between min and max (inclusive).
	  _.random = function(min, max) {
	    if (max == null) {
	      max = min;
	      min = 0;
	    }
	    return min + Math.floor(Math.random() * (max - min + 1));
	  };
	
	  // A (possibly faster) way to get the current timestamp as an integer.
	  _.now = Date.now || function() {
	    return new Date().getTime();
	  };
	
	   // List of HTML entities for escaping.
	  var escapeMap = {
	    '&': '&amp;',
	    '<': '&lt;',
	    '>': '&gt;',
	    '"': '&quot;',
	    "'": '&#x27;',
	    '`': '&#x60;'
	  };
	  var unescapeMap = _.invert(escapeMap);
	
	  // Functions for escaping and unescaping strings to/from HTML interpolation.
	  var createEscaper = function(map) {
	    var escaper = function(match) {
	      return map[match];
	    };
	    // Regexes for identifying a key that needs to be escaped
	    var source = '(?:' + _.keys(map).join('|') + ')';
	    var testRegexp = RegExp(source);
	    var replaceRegexp = RegExp(source, 'g');
	    return function(string) {
	      string = string == null ? '' : '' + string;
	      return testRegexp.test(string) ? string.replace(replaceRegexp, escaper) : string;
	    };
	  };
	  _.escape = createEscaper(escapeMap);
	  _.unescape = createEscaper(unescapeMap);
	
	  // If the value of the named `property` is a function then invoke it with the
	  // `object` as context; otherwise, return it.
	  _.result = function(object, property, fallback) {
	    var value = object == null ? void 0 : object[property];
	    if (value === void 0) {
	      value = fallback;
	    }
	    return _.isFunction(value) ? value.call(object) : value;
	  };
	
	  // Generate a unique integer id (unique within the entire client session).
	  // Useful for temporary DOM ids.
	  var idCounter = 0;
	  _.uniqueId = function(prefix) {
	    var id = ++idCounter + '';
	    return prefix ? prefix + id : id;
	  };
	
	  // By default, Underscore uses ERB-style template delimiters, change the
	  // following template settings to use alternative delimiters.
	  _.templateSettings = {
	    evaluate    : /<%([\s\S]+?)%>/g,
	    interpolate : /<%=([\s\S]+?)%>/g,
	    escape      : /<%-([\s\S]+?)%>/g
	  };
	
	  // When customizing `templateSettings`, if you don't want to define an
	  // interpolation, evaluation or escaping regex, we need one that is
	  // guaranteed not to match.
	  var noMatch = /(.)^/;
	
	  // Certain characters need to be escaped so that they can be put into a
	  // string literal.
	  var escapes = {
	    "'":      "'",
	    '\\':     '\\',
	    '\r':     'r',
	    '\n':     'n',
	    '\u2028': 'u2028',
	    '\u2029': 'u2029'
	  };
	
	  var escaper = /\\|'|\r|\n|\u2028|\u2029/g;
	
	  var escapeChar = function(match) {
	    return '\\' + escapes[match];
	  };
	
	  // JavaScript micro-templating, similar to John Resig's implementation.
	  // Underscore templating handles arbitrary delimiters, preserves whitespace,
	  // and correctly escapes quotes within interpolated code.
	  // NB: `oldSettings` only exists for backwards compatibility.
	  _.template = function(text, settings, oldSettings) {
	    if (!settings && oldSettings) settings = oldSettings;
	    settings = _.defaults({}, settings, _.templateSettings);
	
	    // Combine delimiters into one regular expression via alternation.
	    var matcher = RegExp([
	      (settings.escape || noMatch).source,
	      (settings.interpolate || noMatch).source,
	      (settings.evaluate || noMatch).source
	    ].join('|') + '|$', 'g');
	
	    // Compile the template source, escaping string literals appropriately.
	    var index = 0;
	    var source = "__p+='";
	    text.replace(matcher, function(match, escape, interpolate, evaluate, offset) {
	      source += text.slice(index, offset).replace(escaper, escapeChar);
	      index = offset + match.length;
	
	      if (escape) {
	        source += "'+\n((__t=(" + escape + "))==null?'':_.escape(__t))+\n'";
	      } else if (interpolate) {
	        source += "'+\n((__t=(" + interpolate + "))==null?'':__t)+\n'";
	      } else if (evaluate) {
	        source += "';\n" + evaluate + "\n__p+='";
	      }
	
	      // Adobe VMs need the match returned to produce the correct offest.
	      return match;
	    });
	    source += "';\n";
	
	    // If a variable is not specified, place data values in local scope.
	    if (!settings.variable) source = 'with(obj||{}){\n' + source + '}\n';
	
	    source = "var __t,__p='',__j=Array.prototype.join," +
	      "print=function(){__p+=__j.call(arguments,'');};\n" +
	      source + 'return __p;\n';
	
	    try {
	      var render = new Function(settings.variable || 'obj', '_', source);
	    } catch (e) {
	      e.source = source;
	      throw e;
	    }
	
	    var template = function(data) {
	      return render.call(this, data, _);
	    };
	
	    // Provide the compiled source as a convenience for precompilation.
	    var argument = settings.variable || 'obj';
	    template.source = 'function(' + argument + '){\n' + source + '}';
	
	    return template;
	  };
	
	  // Add a "chain" function. Start chaining a wrapped Underscore object.
	  _.chain = function(obj) {
	    var instance = _(obj);
	    instance._chain = true;
	    return instance;
	  };
	
	  // OOP
	  // ---------------
	  // If Underscore is called as a function, it returns a wrapped object that
	  // can be used OO-style. This wrapper holds altered versions of all the
	  // underscore functions. Wrapped objects may be chained.
	
	  // Helper function to continue chaining intermediate results.
	  var result = function(instance, obj) {
	    return instance._chain ? _(obj).chain() : obj;
	  };
	
	  // Add your own custom functions to the Underscore object.
	  _.mixin = function(obj) {
	    _.each(_.functions(obj), function(name) {
	      var func = _[name] = obj[name];
	      _.prototype[name] = function() {
	        var args = [this._wrapped];
	        push.apply(args, arguments);
	        return result(this, func.apply(_, args));
	      };
	    });
	  };
	
	  // Add all of the Underscore functions to the wrapper object.
	  _.mixin(_);
	
	  // Add all mutator Array functions to the wrapper.
	  _.each(['pop', 'push', 'reverse', 'shift', 'sort', 'splice', 'unshift'], function(name) {
	    var method = ArrayProto[name];
	    _.prototype[name] = function() {
	      var obj = this._wrapped;
	      method.apply(obj, arguments);
	      if ((name === 'shift' || name === 'splice') && obj.length === 0) delete obj[0];
	      return result(this, obj);
	    };
	  });
	
	  // Add all accessor Array functions to the wrapper.
	  _.each(['concat', 'join', 'slice'], function(name) {
	    var method = ArrayProto[name];
	    _.prototype[name] = function() {
	      return result(this, method.apply(this._wrapped, arguments));
	    };
	  });
	
	  // Extracts the result from a wrapped and chained object.
	  _.prototype.value = function() {
	    return this._wrapped;
	  };
	
	  // Provide unwrapping proxy for some methods used in engine operations
	  // such as arithmetic and JSON stringification.
	  _.prototype.valueOf = _.prototype.toJSON = _.prototype.value;
	
	  _.prototype.toString = function() {
	    return '' + this._wrapped;
	  };
	
	  // AMD registration happens at the end for compatibility with AMD loaders
	  // that may not enforce next-turn semantics on modules. Even though general
	  // practice for AMD registration is to be anonymous, underscore registers
	  // as a named module because, like jQuery, it is a base library that is
	  // popular enough to be bundled in a third party lib, but not be part of
	  // an AMD load request. Those cases could generate an error when an
	  // anonymous define() is called outside of a loader request.
	  if (true) {
	    !(__WEBPACK_AMD_DEFINE_ARRAY__ = [], __WEBPACK_AMD_DEFINE_RESULT__ = function() {
	      return _;
	    }.apply(exports, __WEBPACK_AMD_DEFINE_ARRAY__), __WEBPACK_AMD_DEFINE_RESULT__ !== undefined && (module.exports = __WEBPACK_AMD_DEFINE_RESULT__));
	  }
	}.call(this));


/***/ },

/***/ 185:
/***/ function(module, exports, __webpack_require__) {

	__webpack_require__(186);
	module.exports = 'ui.bootstrap';


/***/ },

/***/ 186:
/***/ function(module, exports) {

	/*
	 * angular-ui-bootstrap
	 * http://angular-ui.github.io/bootstrap/
	
	 * Version: 0.14.3 - 2015-10-23
	 * License: MIT
	 */
	angular.module("ui.bootstrap", ["ui.bootstrap.tpls", "ui.bootstrap.collapse","ui.bootstrap.accordion","ui.bootstrap.alert","ui.bootstrap.buttons","ui.bootstrap.carousel","ui.bootstrap.dateparser","ui.bootstrap.position","ui.bootstrap.datepicker","ui.bootstrap.dropdown","ui.bootstrap.stackedMap","ui.bootstrap.modal","ui.bootstrap.pagination","ui.bootstrap.tooltip","ui.bootstrap.popover","ui.bootstrap.progressbar","ui.bootstrap.rating","ui.bootstrap.tabs","ui.bootstrap.timepicker","ui.bootstrap.typeahead"]);
	angular.module("ui.bootstrap.tpls", ["template/accordion/accordion-group.html","template/accordion/accordion.html","template/alert/alert.html","template/carousel/carousel.html","template/carousel/slide.html","template/datepicker/datepicker.html","template/datepicker/day.html","template/datepicker/month.html","template/datepicker/popup.html","template/datepicker/year.html","template/modal/backdrop.html","template/modal/window.html","template/pagination/pager.html","template/pagination/pagination.html","template/tooltip/tooltip-html-popup.html","template/tooltip/tooltip-popup.html","template/tooltip/tooltip-template-popup.html","template/popover/popover-html.html","template/popover/popover-template.html","template/popover/popover.html","template/progressbar/bar.html","template/progressbar/progress.html","template/progressbar/progressbar.html","template/rating/rating.html","template/tabs/tab.html","template/tabs/tabset.html","template/timepicker/timepicker.html","template/typeahead/typeahead-match.html","template/typeahead/typeahead-popup.html"]);
	angular.module('ui.bootstrap.collapse', [])
	
	  .directive('uibCollapse', ['$animate', '$injector', function($animate, $injector) {
	    var $animateCss = $injector.has('$animateCss') ? $injector.get('$animateCss') : null;
	    return {
	      link: function(scope, element, attrs) {
	        function expand() {
	          element.removeClass('collapse')
	            .addClass('collapsing')
	            .attr('aria-expanded', true)
	            .attr('aria-hidden', false);
	
	          if ($animateCss) {
	            $animateCss(element, {
	              addClass: 'in',
	              easing: 'ease',
	              to: { height: element[0].scrollHeight + 'px' }
	            }).start().finally(expandDone);
	          } else {
	            $animate.addClass(element, 'in', {
	              to: { height: element[0].scrollHeight + 'px' }
	            }).then(expandDone);
	          }
	        }
	
	        function expandDone() {
	          element.removeClass('collapsing')
	            .addClass('collapse')
	            .css({height: 'auto'});
	        }
	
	        function collapse() {
	          if (!element.hasClass('collapse') && !element.hasClass('in')) {
	            return collapseDone();
	          }
	
	          element
	            // IMPORTANT: The height must be set before adding "collapsing" class.
	            // Otherwise, the browser attempts to animate from height 0 (in
	            // collapsing class) to the given height here.
	            .css({height: element[0].scrollHeight + 'px'})
	            // initially all panel collapse have the collapse class, this removal
	            // prevents the animation from jumping to collapsed state
	            .removeClass('collapse')
	            .addClass('collapsing')
	            .attr('aria-expanded', false)
	            .attr('aria-hidden', true);
	
	          if ($animateCss) {
	            $animateCss(element, {
	              removeClass: 'in',
	              to: {height: '0'}
	            }).start().finally(collapseDone);
	          } else {
	            $animate.removeClass(element, 'in', {
	              to: {height: '0'}
	            }).then(collapseDone);
	          }
	        }
	
	        function collapseDone() {
	          element.css({height: '0'}); // Required so that collapse works when animation is disabled
	          element.removeClass('collapsing')
	            .addClass('collapse');
	        }
	
	        scope.$watch(attrs.uibCollapse, function(shouldCollapse) {
	          if (shouldCollapse) {
	            collapse();
	          } else {
	            expand();
	          }
	        });
	      }
	    };
	  }]);
	
	/* Deprecated collapse below */
	
	angular.module('ui.bootstrap.collapse')
	
	  .value('$collapseSuppressWarning', false)
	
	  .directive('collapse', ['$animate', '$injector', '$log', '$collapseSuppressWarning', function($animate, $injector, $log, $collapseSuppressWarning) {
	    var $animateCss = $injector.has('$animateCss') ? $injector.get('$animateCss') : null;
	    return {
	      link: function(scope, element, attrs) {
	        if (!$collapseSuppressWarning) {
	          $log.warn('collapse is now deprecated. Use uib-collapse instead.');
	        }
	
	        function expand() {
	          element.removeClass('collapse')
	            .addClass('collapsing')
	            .attr('aria-expanded', true)
	            .attr('aria-hidden', false);
	
	          if ($animateCss) {
	            $animateCss(element, {
	              easing: 'ease',
	              to: { height: element[0].scrollHeight + 'px' }
	            }).start().done(expandDone);
	          } else {
	            $animate.animate(element, {}, {
	              height: element[0].scrollHeight + 'px'
	            }).then(expandDone);
	          }
	        }
	
	        function expandDone() {
	          element.removeClass('collapsing')
	            .addClass('collapse in')
	            .css({height: 'auto'});
	        }
	
	        function collapse() {
	          if (!element.hasClass('collapse') && !element.hasClass('in')) {
	            return collapseDone();
	          }
	
	          element
	            // IMPORTANT: The height must be set before adding "collapsing" class.
	            // Otherwise, the browser attempts to animate from height 0 (in
	            // collapsing class) to the given height here.
	            .css({height: element[0].scrollHeight + 'px'})
	            // initially all panel collapse have the collapse class, this removal
	            // prevents the animation from jumping to collapsed state
	            .removeClass('collapse in')
	            .addClass('collapsing')
	            .attr('aria-expanded', false)
	            .attr('aria-hidden', true);
	
	          if ($animateCss) {
	            $animateCss(element, {
	              to: {height: '0'}
	            }).start().done(collapseDone);
	          } else {
	            $animate.animate(element, {}, {
	              height: '0'
	            }).then(collapseDone);
	          }
	        }
	
	        function collapseDone() {
	          element.css({height: '0'}); // Required so that collapse works when animation is disabled
	          element.removeClass('collapsing')
	            .addClass('collapse');
	        }
	
	        scope.$watch(attrs.collapse, function(shouldCollapse) {
	          if (shouldCollapse) {
	            collapse();
	          } else {
	            expand();
	          }
	        });
	      }
	    };
	  }]);
	
	angular.module('ui.bootstrap.accordion', ['ui.bootstrap.collapse'])
	
	.constant('uibAccordionConfig', {
	  closeOthers: true
	})
	
	.controller('UibAccordionController', ['$scope', '$attrs', 'uibAccordionConfig', function($scope, $attrs, accordionConfig) {
	  // This array keeps track of the accordion groups
	  this.groups = [];
	
	  // Ensure that all the groups in this accordion are closed, unless close-others explicitly says not to
	  this.closeOthers = function(openGroup) {
	    var closeOthers = angular.isDefined($attrs.closeOthers) ?
	      $scope.$eval($attrs.closeOthers) : accordionConfig.closeOthers;
	    if (closeOthers) {
	      angular.forEach(this.groups, function(group) {
	        if (group !== openGroup) {
	          group.isOpen = false;
	        }
	      });
	    }
	  };
	
	  // This is called from the accordion-group directive to add itself to the accordion
	  this.addGroup = function(groupScope) {
	    var that = this;
	    this.groups.push(groupScope);
	
	    groupScope.$on('$destroy', function(event) {
	      that.removeGroup(groupScope);
	    });
	  };
	
	  // This is called from the accordion-group directive when to remove itself
	  this.removeGroup = function(group) {
	    var index = this.groups.indexOf(group);
	    if (index !== -1) {
	      this.groups.splice(index, 1);
	    }
	  };
	
	}])
	
	// The accordion directive simply sets up the directive controller
	// and adds an accordion CSS class to itself element.
	.directive('uibAccordion', function() {
	  return {
	    controller: 'UibAccordionController',
	    controllerAs: 'accordion',
	    transclude: true,
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/accordion/accordion.html';
	    }
	  };
	})
	
	// The accordion-group directive indicates a block of html that will expand and collapse in an accordion
	.directive('uibAccordionGroup', function() {
	  return {
	    require: '^uibAccordion',         // We need this directive to be inside an accordion
	    transclude: true,              // It transcludes the contents of the directive into the template
	    replace: true,                // The element containing the directive will be replaced with the template
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/accordion/accordion-group.html';
	    },
	    scope: {
	      heading: '@',               // Interpolate the heading attribute onto this scope
	      isOpen: '=?',
	      isDisabled: '=?'
	    },
	    controller: function() {
	      this.setHeading = function(element) {
	        this.heading = element;
	      };
	    },
	    link: function(scope, element, attrs, accordionCtrl) {
	      accordionCtrl.addGroup(scope);
	
	      scope.openClass = attrs.openClass || 'panel-open';
	      scope.panelClass = attrs.panelClass;
	      scope.$watch('isOpen', function(value) {
	        element.toggleClass(scope.openClass, !!value);
	        if (value) {
	          accordionCtrl.closeOthers(scope);
	        }
	      });
	
	      scope.toggleOpen = function($event) {
	        if (!scope.isDisabled) {
	          if (!$event || $event.which === 32) {
	            scope.isOpen = !scope.isOpen;
	          }
	        }
	      };
	    }
	  };
	})
	
	// Use accordion-heading below an accordion-group to provide a heading containing HTML
	.directive('uibAccordionHeading', function() {
	  return {
	    transclude: true,   // Grab the contents to be used as the heading
	    template: '',       // In effect remove this element!
	    replace: true,
	    require: '^uibAccordionGroup',
	    link: function(scope, element, attrs, accordionGroupCtrl, transclude) {
	      // Pass the heading to the accordion-group controller
	      // so that it can be transcluded into the right place in the template
	      // [The second parameter to transclude causes the elements to be cloned so that they work in ng-repeat]
	      accordionGroupCtrl.setHeading(transclude(scope, angular.noop));
	    }
	  };
	})
	
	// Use in the accordion-group template to indicate where you want the heading to be transcluded
	// You must provide the property on the accordion-group controller that will hold the transcluded element
	.directive('uibAccordionTransclude', function() {
	  return {
	    require: ['?^uibAccordionGroup', '?^accordionGroup'],
	    link: function(scope, element, attrs, controller) {
	      controller = controller[0] ? controller[0] : controller[1]; // Delete after we remove deprecation
	      scope.$watch(function() { return controller[attrs.uibAccordionTransclude]; }, function(heading) {
	        if (heading) {
	          element.find('span').html('');
	          element.find('span').append(heading);
	        }
	      });
	    }
	  };
	});
	
	/* Deprecated accordion below */
	
	angular.module('ui.bootstrap.accordion')
	
	  .value('$accordionSuppressWarning', false)
	
	  .controller('AccordionController', ['$scope', '$attrs', '$controller', '$log', '$accordionSuppressWarning', function($scope, $attrs, $controller, $log, $accordionSuppressWarning) {
	    if (!$accordionSuppressWarning) {
	      $log.warn('AccordionController is now deprecated. Use UibAccordionController instead.');
	    }
	
	    angular.extend(this, $controller('UibAccordionController', {
	      $scope: $scope,
	      $attrs: $attrs
	    }));
	  }])
	
	  .directive('accordion', ['$log', '$accordionSuppressWarning', function($log, $accordionSuppressWarning) {
	    return {
	      restrict: 'EA',
	      controller: 'AccordionController',
	      controllerAs: 'accordion',
	      transclude: true,
	      replace: false,
	      templateUrl: function(element, attrs) {
	        return attrs.templateUrl || 'template/accordion/accordion.html';
	      },
	      link: function() {
	        if (!$accordionSuppressWarning) {
	          $log.warn('accordion is now deprecated. Use uib-accordion instead.');
	        }
	      }
	    };
	  }])
	
	  .directive('accordionGroup', ['$log', '$accordionSuppressWarning', function($log, $accordionSuppressWarning) {
	    return {
	      require: '^accordion',         // We need this directive to be inside an accordion
	      restrict: 'EA',
	      transclude: true,              // It transcludes the contents of the directive into the template
	      replace: true,                // The element containing the directive will be replaced with the template
	      templateUrl: function(element, attrs) {
	        return attrs.templateUrl || 'template/accordion/accordion-group.html';
	      },
	      scope: {
	        heading: '@',               // Interpolate the heading attribute onto this scope
	        isOpen: '=?',
	        isDisabled: '=?'
	      },
	      controller: function() {
	        this.setHeading = function(element) {
	          this.heading = element;
	        };
	      },
	      link: function(scope, element, attrs, accordionCtrl) {
	        if (!$accordionSuppressWarning) {
	          $log.warn('accordion-group is now deprecated. Use uib-accordion-group instead.');
	        }
	
	        accordionCtrl.addGroup(scope);
	
	        scope.openClass = attrs.openClass || 'panel-open';
	        scope.panelClass = attrs.panelClass;
	        scope.$watch('isOpen', function(value) {
	          element.toggleClass(scope.openClass, !!value);
	          if (value) {
	            accordionCtrl.closeOthers(scope);
	          }
	        });
	
	        scope.toggleOpen = function($event) {
	          if (!scope.isDisabled) {
	            if (!$event || $event.which === 32) {
	              scope.isOpen = !scope.isOpen;
	            }
	          }
	        };
	      }
	    };
	  }])
	
	  .directive('accordionHeading', ['$log', '$accordionSuppressWarning', function($log, $accordionSuppressWarning) {
	    return {
	      restrict: 'EA',
	      transclude: true,   // Grab the contents to be used as the heading
	      template: '',       // In effect remove this element!
	      replace: true,
	      require: '^accordionGroup',
	      link: function(scope, element, attr, accordionGroupCtrl, transclude) {
	        if (!$accordionSuppressWarning) {
	          $log.warn('accordion-heading is now deprecated. Use uib-accordion-heading instead.');
	        }
	        // Pass the heading to the accordion-group controller
	        // so that it can be transcluded into the right place in the template
	        // [The second parameter to transclude causes the elements to be cloned so that they work in ng-repeat]
	        accordionGroupCtrl.setHeading(transclude(scope, angular.noop));
	      }
	    };
	  }])
	
	  .directive('accordionTransclude', ['$log', '$accordionSuppressWarning', function($log, $accordionSuppressWarning) {
	    return {
	      require: '^accordionGroup',
	      link: function(scope, element, attr, controller) {
	        if (!$accordionSuppressWarning) {
	          $log.warn('accordion-transclude is now deprecated. Use uib-accordion-transclude instead.');
	        }
	
	        scope.$watch(function() { return controller[attr.accordionTransclude]; }, function(heading) {
	          if (heading) {
	            element.find('span').html('');
	            element.find('span').append(heading);
	          }
	        });
	      }
	    };
	  }]);
	
	
	angular.module('ui.bootstrap.alert', [])
	
	.controller('UibAlertController', ['$scope', '$attrs', '$interpolate', '$timeout', function($scope, $attrs, $interpolate, $timeout) {
	  $scope.closeable = !!$attrs.close;
	
	  var dismissOnTimeout = angular.isDefined($attrs.dismissOnTimeout) ?
	    $interpolate($attrs.dismissOnTimeout)($scope.$parent) : null;
	
	  if (dismissOnTimeout) {
	    $timeout(function() {
	      $scope.close();
	    }, parseInt(dismissOnTimeout, 10));
	  }
	}])
	
	.directive('uibAlert', function() {
	  return {
	    controller: 'UibAlertController',
	    controllerAs: 'alert',
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/alert/alert.html';
	    },
	    transclude: true,
	    replace: true,
	    scope: {
	      type: '@',
	      close: '&'
	    }
	  };
	});
	
	/* Deprecated alert below */
	
	angular.module('ui.bootstrap.alert')
	
	  .value('$alertSuppressWarning', false)
	
	  .controller('AlertController', ['$scope', '$attrs', '$controller', '$log', '$alertSuppressWarning', function($scope, $attrs, $controller, $log, $alertSuppressWarning) {
	    if (!$alertSuppressWarning) {
	      $log.warn('AlertController is now deprecated. Use UibAlertController instead.');
	    }
	
	    angular.extend(this, $controller('UibAlertController', {
	      $scope: $scope,
	      $attrs: $attrs
	    }));
	  }])
	
	  .directive('alert', ['$log', '$alertSuppressWarning', function($log, $alertSuppressWarning) {
	    return {
	      controller: 'AlertController',
	      controllerAs: 'alert',
	      templateUrl: function(element, attrs) {
	        return attrs.templateUrl || 'template/alert/alert.html';
	      },
	      transclude: true,
	      replace: true,
	      scope: {
	        type: '@',
	        close: '&'
	      },
	      link: function() {
	        if (!$alertSuppressWarning) {
	          $log.warn('alert is now deprecated. Use uib-alert instead.');
	        }
	      }
	    };
	  }]);
	
	angular.module('ui.bootstrap.buttons', [])
	
	.constant('uibButtonConfig', {
	  activeClass: 'active',
	  toggleEvent: 'click'
	})
	
	.controller('UibButtonsController', ['uibButtonConfig', function(buttonConfig) {
	  this.activeClass = buttonConfig.activeClass || 'active';
	  this.toggleEvent = buttonConfig.toggleEvent || 'click';
	}])
	
	.directive('uibBtnRadio', function() {
	  return {
	    require: ['uibBtnRadio', 'ngModel'],
	    controller: 'UibButtonsController',
	    controllerAs: 'buttons',
	    link: function(scope, element, attrs, ctrls) {
	      var buttonsCtrl = ctrls[0], ngModelCtrl = ctrls[1];
	
	      element.find('input').css({display: 'none'});
	
	      //model -> UI
	      ngModelCtrl.$render = function() {
	        element.toggleClass(buttonsCtrl.activeClass, angular.equals(ngModelCtrl.$modelValue, scope.$eval(attrs.uibBtnRadio)));
	      };
	
	      //ui->model
	      element.on(buttonsCtrl.toggleEvent, function() {
	        if (attrs.disabled) {
	          return;
	        }
	
	        var isActive = element.hasClass(buttonsCtrl.activeClass);
	
	        if (!isActive || angular.isDefined(attrs.uncheckable)) {
	          scope.$apply(function() {
	            ngModelCtrl.$setViewValue(isActive ? null : scope.$eval(attrs.uibBtnRadio));
	            ngModelCtrl.$render();
	          });
	        }
	      });
	    }
	  };
	})
	
	.directive('uibBtnCheckbox', function() {
	  return {
	    require: ['uibBtnCheckbox', 'ngModel'],
	    controller: 'UibButtonsController',
	    controllerAs: 'button',
	    link: function(scope, element, attrs, ctrls) {
	      var buttonsCtrl = ctrls[0], ngModelCtrl = ctrls[1];
	
	      element.find('input').css({display: 'none'});
	
	      function getTrueValue() {
	        return getCheckboxValue(attrs.btnCheckboxTrue, true);
	      }
	
	      function getFalseValue() {
	        return getCheckboxValue(attrs.btnCheckboxFalse, false);
	      }
	
	      function getCheckboxValue(attribute, defaultValue) {
	        return angular.isDefined(attribute) ? scope.$eval(attribute) : defaultValue;
	      }
	
	      //model -> UI
	      ngModelCtrl.$render = function() {
	        element.toggleClass(buttonsCtrl.activeClass, angular.equals(ngModelCtrl.$modelValue, getTrueValue()));
	      };
	
	      //ui->model
	      element.on(buttonsCtrl.toggleEvent, function() {
	        if (attrs.disabled) {
	          return;
	        }
	
	        scope.$apply(function() {
	          ngModelCtrl.$setViewValue(element.hasClass(buttonsCtrl.activeClass) ? getFalseValue() : getTrueValue());
	          ngModelCtrl.$render();
	        });
	      });
	    }
	  };
	});
	
	/* Deprecated buttons below */
	
	angular.module('ui.bootstrap.buttons')
	
	  .value('$buttonsSuppressWarning', false)
	
	  .controller('ButtonsController', ['$controller', '$log', '$buttonsSuppressWarning', function($controller, $log, $buttonsSuppressWarning) {
	    if (!$buttonsSuppressWarning) {
	      $log.warn('ButtonsController is now deprecated. Use UibButtonsController instead.');
	    }
	
	    angular.extend(this, $controller('UibButtonsController'));
	  }])
	
	  .directive('btnRadio', ['$log', '$buttonsSuppressWarning', function($log, $buttonsSuppressWarning) {
	    return {
	      require: ['btnRadio', 'ngModel'],
	      controller: 'ButtonsController',
	      controllerAs: 'buttons',
	      link: function(scope, element, attrs, ctrls) {
	        if (!$buttonsSuppressWarning) {
	          $log.warn('btn-radio is now deprecated. Use uib-btn-radio instead.');
	        }
	
	        var buttonsCtrl = ctrls[0], ngModelCtrl = ctrls[1];
	
	        element.find('input').css({display: 'none'});
	
	        //model -> UI
	        ngModelCtrl.$render = function() {
	          element.toggleClass(buttonsCtrl.activeClass, angular.equals(ngModelCtrl.$modelValue, scope.$eval(attrs.btnRadio)));
	        };
	
	        //ui->model
	        element.bind(buttonsCtrl.toggleEvent, function() {
	          if (attrs.disabled) {
	            return;
	          }
	
	          var isActive = element.hasClass(buttonsCtrl.activeClass);
	
	          if (!isActive || angular.isDefined(attrs.uncheckable)) {
	            scope.$apply(function() {
	              ngModelCtrl.$setViewValue(isActive ? null : scope.$eval(attrs.btnRadio));
	              ngModelCtrl.$render();
	            });
	          }
	        });
	      }
	    };
	  }])
	
	  .directive('btnCheckbox', ['$document', '$log', '$buttonsSuppressWarning', function($document, $log, $buttonsSuppressWarning) {
	    return {
	      require: ['btnCheckbox', 'ngModel'],
	      controller: 'ButtonsController',
	      controllerAs: 'button',
	      link: function(scope, element, attrs, ctrls) {
	        if (!$buttonsSuppressWarning) {
	          $log.warn('btn-checkbox is now deprecated. Use uib-btn-checkbox instead.');
	        }
	
	        var buttonsCtrl = ctrls[0], ngModelCtrl = ctrls[1];
	
	        element.find('input').css({display: 'none'});
	
	        function getTrueValue() {
	          return getCheckboxValue(attrs.btnCheckboxTrue, true);
	        }
	
	        function getFalseValue() {
	          return getCheckboxValue(attrs.btnCheckboxFalse, false);
	        }
	
	        function getCheckboxValue(attributeValue, defaultValue) {
	          var val = scope.$eval(attributeValue);
	          return angular.isDefined(val) ? val : defaultValue;
	        }
	
	        //model -> UI
	        ngModelCtrl.$render = function() {
	          element.toggleClass(buttonsCtrl.activeClass, angular.equals(ngModelCtrl.$modelValue, getTrueValue()));
	        };
	
	        //ui->model
	        element.bind(buttonsCtrl.toggleEvent, function() {
	          if (attrs.disabled) {
	            return;
	          }
	
	          scope.$apply(function() {
	            ngModelCtrl.$setViewValue(element.hasClass(buttonsCtrl.activeClass) ? getFalseValue() : getTrueValue());
	            ngModelCtrl.$render();
	          });
	        });
	
	        //accessibility
	        element.on('keypress', function(e) {
	          if (attrs.disabled || e.which !== 32 || $document[0].activeElement !== element[0]) {
	            return;
	          }
	
	          scope.$apply(function() {
	            ngModelCtrl.$setViewValue(element.hasClass(buttonsCtrl.activeClass) ? getFalseValue() : getTrueValue());
	            ngModelCtrl.$render();
	          });
	        });
	      }
	    };
	  }]);
	
	
	/**
	 * @ngdoc overview
	 * @name ui.bootstrap.carousel
	 *
	 * @description
	 * AngularJS version of an image carousel.
	 *
	 */
	angular.module('ui.bootstrap.carousel', [])
	
	.controller('UibCarouselController', ['$scope', '$element', '$interval', '$animate', function($scope, $element, $interval, $animate) {
	  var self = this,
	    slides = self.slides = $scope.slides = [],
	    NEW_ANIMATE = angular.version.minor >= 4,
	    NO_TRANSITION = 'uib-noTransition',
	    SLIDE_DIRECTION = 'uib-slideDirection',
	    currentIndex = -1,
	    currentInterval, isPlaying;
	  self.currentSlide = null;
	
	  var destroyed = false;
	  /* direction: "prev" or "next" */
	  self.select = $scope.select = function(nextSlide, direction) {
	    var nextIndex = $scope.indexOfSlide(nextSlide);
	    //Decide direction if it's not given
	    if (direction === undefined) {
	      direction = nextIndex > self.getCurrentIndex() ? 'next' : 'prev';
	    }
	    //Prevent this user-triggered transition from occurring if there is already one in progress
	    if (nextSlide && nextSlide !== self.currentSlide && !$scope.$currentTransition) {
	      goNext(nextSlide, nextIndex, direction);
	    }
	  };
	
	  function goNext(slide, index, direction) {
	    // Scope has been destroyed, stop here.
	    if (destroyed) { return; }
	
	    angular.extend(slide, {direction: direction, active: true});
	    angular.extend(self.currentSlide || {}, {direction: direction, active: false});
	    if ($animate.enabled() && !$scope.noTransition && !$scope.$currentTransition &&
	      slide.$element && self.slides.length > 1) {
	      slide.$element.data(SLIDE_DIRECTION, slide.direction);
	      if (self.currentSlide && self.currentSlide.$element) {
	        self.currentSlide.$element.data(SLIDE_DIRECTION, slide.direction);
	      }
	
	      $scope.$currentTransition = true;
	      if (NEW_ANIMATE) {
	        $animate.on('addClass', slide.$element, function(element, phase) {
	          if (phase === 'close') {
	            $scope.$currentTransition = null;
	            $animate.off('addClass', element);
	          }
	        });
	      } else {
	        slide.$element.one('$animate:close', function closeFn() {
	          $scope.$currentTransition = null;
	        });
	      }
	    }
	
	    self.currentSlide = slide;
	    currentIndex = index;
	
	    //every time you change slides, reset the timer
	    restartTimer();
	  }
	
	  $scope.$on('$destroy', function() {
	    destroyed = true;
	  });
	
	  function getSlideByIndex(index) {
	    if (angular.isUndefined(slides[index].index)) {
	      return slides[index];
	    }
	    var i, len = slides.length;
	    for (i = 0; i < slides.length; ++i) {
	      if (slides[i].index == index) {
	        return slides[i];
	      }
	    }
	  }
	
	  self.getCurrentIndex = function() {
	    if (self.currentSlide && angular.isDefined(self.currentSlide.index)) {
	      return +self.currentSlide.index;
	    }
	    return currentIndex;
	  };
	
	  /* Allow outside people to call indexOf on slides array */
	  $scope.indexOfSlide = function(slide) {
	    return angular.isDefined(slide.index) ? +slide.index : slides.indexOf(slide);
	  };
	
	  $scope.next = function() {
	    var newIndex = (self.getCurrentIndex() + 1) % slides.length;
	
	    if (newIndex === 0 && $scope.noWrap()) {
	      $scope.pause();
	      return;
	    }
	
	    return self.select(getSlideByIndex(newIndex), 'next');
	  };
	
	  $scope.prev = function() {
	    var newIndex = self.getCurrentIndex() - 1 < 0 ? slides.length - 1 : self.getCurrentIndex() - 1;
	
	    if ($scope.noWrap() && newIndex === slides.length - 1) {
	      $scope.pause();
	      return;
	    }
	
	    return self.select(getSlideByIndex(newIndex), 'prev');
	  };
	
	  $scope.isActive = function(slide) {
	     return self.currentSlide === slide;
	  };
	
	  $scope.$watch('interval', restartTimer);
	  $scope.$watchCollection('slides', resetTransition);
	  $scope.$on('$destroy', resetTimer);
	
	  function restartTimer() {
	    resetTimer();
	    var interval = +$scope.interval;
	    if (!isNaN(interval) && interval > 0) {
	      currentInterval = $interval(timerFn, interval);
	    }
	  }
	
	  function resetTimer() {
	    if (currentInterval) {
	      $interval.cancel(currentInterval);
	      currentInterval = null;
	    }
	  }
	
	  function timerFn() {
	    var interval = +$scope.interval;
	    if (isPlaying && !isNaN(interval) && interval > 0 && slides.length) {
	      $scope.next();
	    } else {
	      $scope.pause();
	    }
	  }
	
	  function resetTransition(slides) {
	    if (!slides.length) {
	      $scope.$currentTransition = null;
	    }
	  }
	
	  $scope.play = function() {
	    if (!isPlaying) {
	      isPlaying = true;
	      restartTimer();
	    }
	  };
	  $scope.pause = function() {
	    if (!$scope.noPause) {
	      isPlaying = false;
	      resetTimer();
	    }
	  };
	
	  self.addSlide = function(slide, element) {
	    slide.$element = element;
	    slides.push(slide);
	    //if this is the first slide or the slide is set to active, select it
	    if (slides.length === 1 || slide.active) {
	      self.select(slides[slides.length - 1]);
	      if (slides.length === 1) {
	        $scope.play();
	      }
	    } else {
	      slide.active = false;
	    }
	  };
	
	  self.removeSlide = function(slide) {
	    if (angular.isDefined(slide.index)) {
	      slides.sort(function(a, b) {
	        return +a.index > +b.index;
	      });
	    }
	    //get the index of the slide inside the carousel
	    var index = slides.indexOf(slide);
	    slides.splice(index, 1);
	    if (slides.length > 0 && slide.active) {
	      if (index >= slides.length) {
	        self.select(slides[index - 1]);
	      } else {
	        self.select(slides[index]);
	      }
	    } else if (currentIndex > index) {
	      currentIndex--;
	    }
	
	    //clean the currentSlide when no more slide
	    if (slides.length === 0) {
	      self.currentSlide = null;
	    }
	  };
	
	  $scope.$watch('noTransition', function(noTransition) {
	    $element.data(NO_TRANSITION, noTransition);
	  });
	
	}])
	
	/**
	 * @ngdoc directive
	 * @name ui.bootstrap.carousel.directive:carousel
	 * @restrict EA
	 *
	 * @description
	 * Carousel is the outer container for a set of image 'slides' to showcase.
	 *
	 * @param {number=} interval The time, in milliseconds, that it will take the carousel to go to the next slide.
	 * @param {boolean=} noTransition Whether to disable transitions on the carousel.
	 * @param {boolean=} noPause Whether to disable pausing on the carousel (by default, the carousel interval pauses on hover).
	 *
	 * @example
	<example module="ui.bootstrap">
	  <file name="index.html">
	    <uib-carousel>
	      <uib-slide>
	        <img src="http://placekitten.com/150/150" style="margin:auto;">
	        <div class="carousel-caption">
	          <p>Beautiful!</p>
	        </div>
	      </uib-slide>
	      <uib-slide>
	        <img src="http://placekitten.com/100/150" style="margin:auto;">
	        <div class="carousel-caption">
	          <p>D'aww!</p>
	        </div>
	      </uib-slide>
	    </uib-carousel>
	  </file>
	  <file name="demo.css">
	    .carousel-indicators {
	      top: auto;
	      bottom: 15px;
	    }
	  </file>
	</example>
	 */
	.directive('uibCarousel', [function() {
	  return {
	    transclude: true,
	    replace: true,
	    controller: 'UibCarouselController',
	    controllerAs: 'carousel',
	    require: 'carousel',
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/carousel/carousel.html';
	    },
	    scope: {
	      interval: '=',
	      noTransition: '=',
	      noPause: '=',
	      noWrap: '&'
	    }
	  };
	}])
	
	/**
	 * @ngdoc directive
	 * @name ui.bootstrap.carousel.directive:slide
	 * @restrict EA
	 *
	 * @description
	 * Creates a slide inside a {@link ui.bootstrap.carousel.directive:carousel carousel}.  Must be placed as a child of a carousel element.
	 *
	 * @param {boolean=} active Model binding, whether or not this slide is currently active.
	 * @param {number=} index The index of the slide. The slides will be sorted by this parameter.
	 *
	 * @example
	<example module="ui.bootstrap">
	  <file name="index.html">
	<div ng-controller="CarouselDemoCtrl">
	  <uib-carousel>
	    <uib-slide ng-repeat="slide in slides" active="slide.active" index="$index">
	      <img ng-src="{{slide.image}}" style="margin:auto;">
	      <div class="carousel-caption">
	        <h4>Slide {{$index}}</h4>
	        <p>{{slide.text}}</p>
	      </div>
	    </uib-slide>
	  </uib-carousel>
	  Interval, in milliseconds: <input type="number" ng-model="myInterval">
	  <br />Enter a negative number to stop the interval.
	</div>
	  </file>
	  <file name="script.js">
	function CarouselDemoCtrl($scope) {
	  $scope.myInterval = 5000;
	}
	  </file>
	  <file name="demo.css">
	    .carousel-indicators {
	      top: auto;
	      bottom: 15px;
	    }
	  </file>
	</example>
	*/
	
	.directive('uibSlide', function() {
	  return {
	    require: '^uibCarousel',
	    restrict: 'EA',
	    transclude: true,
	    replace: true,
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/carousel/slide.html';
	    },
	    scope: {
	      active: '=?',
	      actual: '=?',
	      index: '=?'
	    },
	    link: function (scope, element, attrs, carouselCtrl) {
	      carouselCtrl.addSlide(scope, element);
	      //when the scope is destroyed then remove the slide from the current slides array
	      scope.$on('$destroy', function() {
	        carouselCtrl.removeSlide(scope);
	      });
	
	      scope.$watch('active', function(active) {
	        if (active) {
	          carouselCtrl.select(scope);
	        }
	      });
	    }
	  };
	})
	
	.animation('.item', [
	         '$injector', '$animate',
	function ($injector, $animate) {
	  var NO_TRANSITION = 'uib-noTransition',
	    SLIDE_DIRECTION = 'uib-slideDirection',
	    $animateCss = null;
	
	  if ($injector.has('$animateCss')) {
	    $animateCss = $injector.get('$animateCss');
	  }
	
	  function removeClass(element, className, callback) {
	    element.removeClass(className);
	    if (callback) {
	      callback();
	    }
	  }
	
	  return {
	    beforeAddClass: function(element, className, done) {
	      // Due to transclusion, noTransition property is on parent's scope
	      if (className == 'active' && element.parent() && element.parent().parent() &&
	          !element.parent().parent().data(NO_TRANSITION)) {
	        var stopped = false;
	        var direction = element.data(SLIDE_DIRECTION);
	        var directionClass = direction == 'next' ? 'left' : 'right';
	        var removeClassFn = removeClass.bind(this, element,
	          directionClass + ' ' + direction, done);
	        element.addClass(direction);
	
	        if ($animateCss) {
	          $animateCss(element, {addClass: directionClass})
	            .start()
	            .done(removeClassFn);
	        } else {
	          $animate.addClass(element, directionClass).then(function () {
	            if (!stopped) {
	              removeClassFn();
	            }
	            done();
	          });
	        }
	
	        return function () {
	          stopped = true;
	        };
	      }
	      done();
	    },
	    beforeRemoveClass: function (element, className, done) {
	      // Due to transclusion, noTransition property is on parent's scope
	      if (className === 'active' && element.parent() && element.parent().parent() &&
	          !element.parent().parent().data(NO_TRANSITION)) {
	        var stopped = false;
	        var direction = element.data(SLIDE_DIRECTION);
	        var directionClass = direction == 'next' ? 'left' : 'right';
	        var removeClassFn = removeClass.bind(this, element, directionClass, done);
	
	        if ($animateCss) {
	          $animateCss(element, {addClass: directionClass})
	            .start()
	            .done(removeClassFn);
	        } else {
	          $animate.addClass(element, directionClass).then(function() {
	            if (!stopped) {
	              removeClassFn();
	            }
	            done();
	          });
	        }
	        return function() {
	          stopped = true;
	        };
	      }
	      done();
	    }
	  };
	}]);
	
	/* deprecated carousel below */
	
	angular.module('ui.bootstrap.carousel')
	
	.value('$carouselSuppressWarning', false)
	
	.controller('CarouselController', ['$scope', '$element', '$controller', '$log', '$carouselSuppressWarning', function($scope, $element, $controller, $log, $carouselSuppressWarning) {
	  if (!$carouselSuppressWarning) {
	    $log.warn('CarouselController is now deprecated. Use UibCarouselController instead.');
	  }
	
	  angular.extend(this, $controller('UibCarouselController', {
	    $scope: $scope,
	    $element: $element
	  }));
	}])
	
	.directive('carousel', ['$log', '$carouselSuppressWarning', function($log, $carouselSuppressWarning) {
	  return {
	    transclude: true,
	    replace: true,
	    controller: 'CarouselController',
	    controllerAs: 'carousel',
	    require: 'carousel',
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/carousel/carousel.html';
	    },
	    scope: {
	      interval: '=',
	      noTransition: '=',
	      noPause: '=',
	      noWrap: '&'
	    },
	    link: function() {
	      if (!$carouselSuppressWarning) {
	        $log.warn('carousel is now deprecated. Use uib-carousel instead.');
	      }
	    }
	  };
	}])
	
	.directive('slide', ['$log', '$carouselSuppressWarning', function($log, $carouselSuppressWarning) {
	  return {
	    require: '^carousel',
	    transclude: true,
	    replace: true,
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/carousel/slide.html';
	    },
	    scope: {
	      active: '=?',
	      actual: '=?',
	      index: '=?'
	    },
	    link: function (scope, element, attrs, carouselCtrl) {
	      if (!$carouselSuppressWarning) {
	        $log.warn('slide is now deprecated. Use uib-slide instead.');
	      }
	
	      carouselCtrl.addSlide(scope, element);
	      //when the scope is destroyed then remove the slide from the current slides array
	      scope.$on('$destroy', function() {
	        carouselCtrl.removeSlide(scope);
	      });
	
	      scope.$watch('active', function(active) {
	        if (active) {
	          carouselCtrl.select(scope);
	        }
	      });
	    }
	  };
	}]);
	
	angular.module('ui.bootstrap.dateparser', [])
	
	.service('uibDateParser', ['$log', '$locale', 'orderByFilter', function($log, $locale, orderByFilter) {
	  // Pulled from https://github.com/mbostock/d3/blob/master/src/format/requote.js
	  var SPECIAL_CHARACTERS_REGEXP = /[\\\^\$\*\+\?\|\[\]\(\)\.\{\}]/g;
	
	  var localeId;
	  var formatCodeToRegex;
	
	  this.init = function() {
	    localeId = $locale.id;
	
	    this.parsers = {};
	
	    formatCodeToRegex = {
	      'yyyy': {
	        regex: '\\d{4}',
	        apply: function(value) { this.year = +value; }
	      },
	      'yy': {
	        regex: '\\d{2}',
	        apply: function(value) { this.year = +value + 2000; }
	      },
	      'y': {
	        regex: '\\d{1,4}',
	        apply: function(value) { this.year = +value; }
	      },
	      'MMMM': {
	        regex: $locale.DATETIME_FORMATS.MONTH.join('|'),
	        apply: function(value) { this.month = $locale.DATETIME_FORMATS.MONTH.indexOf(value); }
	      },
	      'MMM': {
	        regex: $locale.DATETIME_FORMATS.SHORTMONTH.join('|'),
	        apply: function(value) { this.month = $locale.DATETIME_FORMATS.SHORTMONTH.indexOf(value); }
	      },
	      'MM': {
	        regex: '0[1-9]|1[0-2]',
	        apply: function(value) { this.month = value - 1; }
	      },
	      'M': {
	        regex: '[1-9]|1[0-2]',
	        apply: function(value) { this.month = value - 1; }
	      },
	      'dd': {
	        regex: '[0-2][0-9]{1}|3[0-1]{1}',
	        apply: function(value) { this.date = +value; }
	      },
	      'd': {
	        regex: '[1-2]?[0-9]{1}|3[0-1]{1}',
	        apply: function(value) { this.date = +value; }
	      },
	      'EEEE': {
	        regex: $locale.DATETIME_FORMATS.DAY.join('|')
	      },
	      'EEE': {
	        regex: $locale.DATETIME_FORMATS.SHORTDAY.join('|')
	      },
	      'HH': {
	        regex: '(?:0|1)[0-9]|2[0-3]',
	        apply: function(value) { this.hours = +value; }
	      },
	      'hh': {
	        regex: '0[0-9]|1[0-2]',
	        apply: function(value) { this.hours = +value; }
	      },
	      'H': {
	        regex: '1?[0-9]|2[0-3]',
	        apply: function(value) { this.hours = +value; }
	      },
	      'h': {
	        regex: '[0-9]|1[0-2]',
	        apply: function(value) { this.hours = +value; }
	      },
	      'mm': {
	        regex: '[0-5][0-9]',
	        apply: function(value) { this.minutes = +value; }
	      },
	      'm': {
	        regex: '[0-9]|[1-5][0-9]',
	        apply: function(value) { this.minutes = +value; }
	      },
	      'sss': {
	        regex: '[0-9][0-9][0-9]',
	        apply: function(value) { this.milliseconds = +value; }
	      },
	      'ss': {
	        regex: '[0-5][0-9]',
	        apply: function(value) { this.seconds = +value; }
	      },
	      's': {
	        regex: '[0-9]|[1-5][0-9]',
	        apply: function(value) { this.seconds = +value; }
	      },
	      'a': {
	        regex: $locale.DATETIME_FORMATS.AMPMS.join('|'),
	        apply: function(value) {
	          if (this.hours === 12) {
	            this.hours = 0;
	          }
	
	          if (value === 'PM') {
	            this.hours += 12;
	          }
	        }
	      }
	    };
	  };
	
	  this.init();
	
	  function createParser(format) {
	    var map = [], regex = format.split('');
	
	    angular.forEach(formatCodeToRegex, function(data, code) {
	      var index = format.indexOf(code);
	
	      if (index > -1) {
	        format = format.split('');
	
	        regex[index] = '(' + data.regex + ')';
	        format[index] = '$'; // Custom symbol to define consumed part of format
	        for (var i = index + 1, n = index + code.length; i < n; i++) {
	          regex[i] = '';
	          format[i] = '$';
	        }
	        format = format.join('');
	
	        map.push({ index: index, apply: data.apply });
	      }
	    });
	
	    return {
	      regex: new RegExp('^' + regex.join('') + '$'),
	      map: orderByFilter(map, 'index')
	    };
	  }
	
	  this.parse = function(input, format, baseDate) {
	    if (!angular.isString(input) || !format) {
	      return input;
	    }
	
	    format = $locale.DATETIME_FORMATS[format] || format;
	    format = format.replace(SPECIAL_CHARACTERS_REGEXP, '\\$&');
	
	    if ($locale.id !== localeId) {
	      this.init();
	    }
	
	    if (!this.parsers[format]) {
	      this.parsers[format] = createParser(format);
	    }
	
	    var parser = this.parsers[format],
	        regex = parser.regex,
	        map = parser.map,
	        results = input.match(regex);
	
	    if (results && results.length) {
	      var fields, dt;
	      if (angular.isDate(baseDate) && !isNaN(baseDate.getTime())) {
	        fields = {
	          year: baseDate.getFullYear(),
	          month: baseDate.getMonth(),
	          date: baseDate.getDate(),
	          hours: baseDate.getHours(),
	          minutes: baseDate.getMinutes(),
	          seconds: baseDate.getSeconds(),
	          milliseconds: baseDate.getMilliseconds()
	        };
	      } else {
	        if (baseDate) {
	          $log.warn('dateparser:', 'baseDate is not a valid date');
	        }
	        fields = { year: 1900, month: 0, date: 1, hours: 0, minutes: 0, seconds: 0, milliseconds: 0 };
	      }
	
	      for (var i = 1, n = results.length; i < n; i++) {
	        var mapper = map[i-1];
	        if (mapper.apply) {
	          mapper.apply.call(fields, results[i]);
	        }
	      }
	
	      if (isValid(fields.year, fields.month, fields.date)) {
	        if (angular.isDate(baseDate) && !isNaN(baseDate.getTime())) {
	          dt = new Date(baseDate);
	          dt.setFullYear(fields.year, fields.month, fields.date,
	            fields.hours, fields.minutes, fields.seconds,
	            fields.milliseconds || 0);
	        } else {
	          dt = new Date(fields.year, fields.month, fields.date,
	            fields.hours, fields.minutes, fields.seconds,
	            fields.milliseconds || 0);
	        }
	      }
	
	      return dt;
	    }
	  };
	
	  // Check if date is valid for specific month (and year for February).
	  // Month: 0 = Jan, 1 = Feb, etc
	  function isValid(year, month, date) {
	    if (date < 1) {
	      return false;
	    }
	
	    if (month === 1 && date > 28) {
	      return date === 29 && ((year % 4 === 0 && year % 100 !== 0) || year % 400 === 0);
	    }
	
	    if (month === 3 || month === 5 || month === 8 || month === 10) {
	      return date < 31;
	    }
	
	    return true;
	  }
	}]);
	
	/* Deprecated dateparser below */
	
	angular.module('ui.bootstrap.dateparser')
	
	.value('$dateParserSuppressWarning', false)
	
	.service('dateParser', ['$log', '$dateParserSuppressWarning', 'uibDateParser', function($log, $dateParserSuppressWarning, uibDateParser) {
	  if (!$dateParserSuppressWarning) {
	    $log.warn('dateParser is now deprecated. Use uibDateParser instead.');
	  }
	
	  angular.extend(this, uibDateParser);
	}]);
	
	angular.module('ui.bootstrap.position', [])
	
	/**
	 * A set of utility methods that can be use to retrieve position of DOM elements.
	 * It is meant to be used where we need to absolute-position DOM elements in
	 * relation to other, existing elements (this is the case for tooltips, popovers,
	 * typeahead suggestions etc.).
	 */
	  .factory('$uibPosition', ['$document', '$window', function($document, $window) {
	    function getStyle(el, cssprop) {
	      if (el.currentStyle) { //IE
	        return el.currentStyle[cssprop];
	      } else if ($window.getComputedStyle) {
	        return $window.getComputedStyle(el)[cssprop];
	      }
	      // finally try and get inline style
	      return el.style[cssprop];
	    }
	
	    /**
	     * Checks if a given element is statically positioned
	     * @param element - raw DOM element
	     */
	    function isStaticPositioned(element) {
	      return (getStyle(element, 'position') || 'static' ) === 'static';
	    }
	
	    /**
	     * returns the closest, non-statically positioned parentOffset of a given element
	     * @param element
	     */
	    var parentOffsetEl = function(element) {
	      var docDomEl = $document[0];
	      var offsetParent = element.offsetParent || docDomEl;
	      while (offsetParent && offsetParent !== docDomEl && isStaticPositioned(offsetParent) ) {
	        offsetParent = offsetParent.offsetParent;
	      }
	      return offsetParent || docDomEl;
	    };
	
	    return {
	      /**
	       * Provides read-only equivalent of jQuery's position function:
	       * http://api.jquery.com/position/
	       */
	      position: function(element) {
	        var elBCR = this.offset(element);
	        var offsetParentBCR = { top: 0, left: 0 };
	        var offsetParentEl = parentOffsetEl(element[0]);
	        if (offsetParentEl != $document[0]) {
	          offsetParentBCR = this.offset(angular.element(offsetParentEl));
	          offsetParentBCR.top += offsetParentEl.clientTop - offsetParentEl.scrollTop;
	          offsetParentBCR.left += offsetParentEl.clientLeft - offsetParentEl.scrollLeft;
	        }
	
	        var boundingClientRect = element[0].getBoundingClientRect();
	        return {
	          width: boundingClientRect.width || element.prop('offsetWidth'),
	          height: boundingClientRect.height || element.prop('offsetHeight'),
	          top: elBCR.top - offsetParentBCR.top,
	          left: elBCR.left - offsetParentBCR.left
	        };
	      },
	
	      /**
	       * Provides read-only equivalent of jQuery's offset function:
	       * http://api.jquery.com/offset/
	       */
	      offset: function(element) {
	        var boundingClientRect = element[0].getBoundingClientRect();
	        return {
	          width: boundingClientRect.width || element.prop('offsetWidth'),
	          height: boundingClientRect.height || element.prop('offsetHeight'),
	          top: boundingClientRect.top + ($window.pageYOffset || $document[0].documentElement.scrollTop),
	          left: boundingClientRect.left + ($window.pageXOffset || $document[0].documentElement.scrollLeft)
	        };
	      },
	
	      /**
	       * Provides coordinates for the targetEl in relation to hostEl
	       */
	      positionElements: function(hostEl, targetEl, positionStr, appendToBody) {
	        var positionStrParts = positionStr.split('-');
	        var pos0 = positionStrParts[0], pos1 = positionStrParts[1] || 'center';
	
	        var hostElPos,
	          targetElWidth,
	          targetElHeight,
	          targetElPos;
	
	        hostElPos = appendToBody ? this.offset(hostEl) : this.position(hostEl);
	
	        targetElWidth = targetEl.prop('offsetWidth');
	        targetElHeight = targetEl.prop('offsetHeight');
	
	        var shiftWidth = {
	          center: function() {
	            return hostElPos.left + hostElPos.width / 2 - targetElWidth / 2;
	          },
	          left: function() {
	            return hostElPos.left;
	          },
	          right: function() {
	            return hostElPos.left + hostElPos.width;
	          }
	        };
	
	        var shiftHeight = {
	          center: function() {
	            return hostElPos.top + hostElPos.height / 2 - targetElHeight / 2;
	          },
	          top: function() {
	            return hostElPos.top;
	          },
	          bottom: function() {
	            return hostElPos.top + hostElPos.height;
	          }
	        };
	
	        switch (pos0) {
	          case 'right':
	            targetElPos = {
	              top: shiftHeight[pos1](),
	              left: shiftWidth[pos0]()
	            };
	            break;
	          case 'left':
	            targetElPos = {
	              top: shiftHeight[pos1](),
	              left: hostElPos.left - targetElWidth
	            };
	            break;
	          case 'bottom':
	            targetElPos = {
	              top: shiftHeight[pos0](),
	              left: shiftWidth[pos1]()
	            };
	            break;
	          default:
	            targetElPos = {
	              top: hostElPos.top - targetElHeight,
	              left: shiftWidth[pos1]()
	            };
	            break;
	        }
	
	        return targetElPos;
	      }
	    };
	  }]);
	
	/* Deprecated position below */
	
	angular.module('ui.bootstrap.position')
	
	.value('$positionSuppressWarning', false)
	
	.service('$position', ['$log', '$positionSuppressWarning', '$uibPosition', function($log, $positionSuppressWarning, $uibPosition) {
	  if (!$positionSuppressWarning) {
	    $log.warn('$position is now deprecated. Use $uibPosition instead.');
	  }
	
	  angular.extend(this, $uibPosition);
	}]);
	
	angular.module('ui.bootstrap.datepicker', ['ui.bootstrap.dateparser', 'ui.bootstrap.position'])
	
	.value('$datepickerSuppressError', false)
	
	.constant('uibDatepickerConfig', {
	  formatDay: 'dd',
	  formatMonth: 'MMMM',
	  formatYear: 'yyyy',
	  formatDayHeader: 'EEE',
	  formatDayTitle: 'MMMM yyyy',
	  formatMonthTitle: 'yyyy',
	  datepickerMode: 'day',
	  minMode: 'day',
	  maxMode: 'year',
	  showWeeks: true,
	  startingDay: 0,
	  yearRange: 20,
	  minDate: null,
	  maxDate: null,
	  shortcutPropagation: false
	})
	
	.controller('UibDatepickerController', ['$scope', '$attrs', '$parse', '$interpolate', '$log', 'dateFilter', 'uibDatepickerConfig', '$datepickerSuppressError', function($scope, $attrs, $parse, $interpolate, $log, dateFilter, datepickerConfig, $datepickerSuppressError) {
	  var self = this,
	      ngModelCtrl = { $setViewValue: angular.noop }; // nullModelCtrl;
	
	  // Modes chain
	  this.modes = ['day', 'month', 'year'];
	
	  // Configuration attributes
	  angular.forEach(['formatDay', 'formatMonth', 'formatYear', 'formatDayHeader', 'formatDayTitle', 'formatMonthTitle',
	                   'showWeeks', 'startingDay', 'yearRange', 'shortcutPropagation'], function(key, index) {
	    self[key] = angular.isDefined($attrs[key]) ? (index < 6 ? $interpolate($attrs[key])($scope.$parent) : $scope.$parent.$eval($attrs[key])) : datepickerConfig[key];
	  });
	
	  // Watchable date attributes
	  angular.forEach(['minDate', 'maxDate'], function(key) {
	    if ($attrs[key]) {
	      $scope.$parent.$watch($parse($attrs[key]), function(value) {
	        self[key] = value ? new Date(value) : null;
	        self.refreshView();
	      });
	    } else {
	      self[key] = datepickerConfig[key] ? new Date(datepickerConfig[key]) : null;
	    }
	  });
	
	  angular.forEach(['minMode', 'maxMode'], function(key) {
	    if ($attrs[key]) {
	      $scope.$parent.$watch($parse($attrs[key]), function(value) {
	        self[key] = angular.isDefined(value) ? value : $attrs[key];
	        $scope[key] = self[key];
	        if ((key == 'minMode' && self.modes.indexOf($scope.datepickerMode) < self.modes.indexOf(self[key])) || (key == 'maxMode' && self.modes.indexOf($scope.datepickerMode) > self.modes.indexOf(self[key]))) {
	          $scope.datepickerMode = self[key];
	        }
	      });
	    } else {
	      self[key] = datepickerConfig[key] || null;
	      $scope[key] = self[key];
	    }
	  });
	
	  $scope.datepickerMode = $scope.datepickerMode || datepickerConfig.datepickerMode;
	  $scope.uniqueId = 'datepicker-' + $scope.$id + '-' + Math.floor(Math.random() * 10000);
	
	  if (angular.isDefined($attrs.initDate)) {
	    this.activeDate = $scope.$parent.$eval($attrs.initDate) || new Date();
	    $scope.$parent.$watch($attrs.initDate, function(initDate) {
	      if (initDate && (ngModelCtrl.$isEmpty(ngModelCtrl.$modelValue) || ngModelCtrl.$invalid)) {
	        self.activeDate = initDate;
	        self.refreshView();
	      }
	    });
	  } else {
	    this.activeDate = new Date();
	  }
	
	  $scope.isActive = function(dateObject) {
	    if (self.compare(dateObject.date, self.activeDate) === 0) {
	      $scope.activeDateId = dateObject.uid;
	      return true;
	    }
	    return false;
	  };
	
	  this.init = function(ngModelCtrl_) {
	    ngModelCtrl = ngModelCtrl_;
	
	    ngModelCtrl.$render = function() {
	      self.render();
	    };
	  };
	
	  this.render = function() {
	    if (ngModelCtrl.$viewValue) {
	      var date = new Date(ngModelCtrl.$viewValue),
	          isValid = !isNaN(date);
	
	      if (isValid) {
	        this.activeDate = date;
	      } else if (!$datepickerSuppressError) {
	        $log.error('Datepicker directive: "ng-model" value must be a Date object, a number of milliseconds since 01.01.1970 or a string representing an RFC2822 or ISO 8601 date.');
	      }
	    }
	    this.refreshView();
	  };
	
	  this.refreshView = function() {
	    if (this.element) {
	      this._refreshView();
	
	      var date = ngModelCtrl.$viewValue ? new Date(ngModelCtrl.$viewValue) : null;
	      ngModelCtrl.$setValidity('dateDisabled', !date || (this.element && !this.isDisabled(date)));
	    }
	  };
	
	  this.createDateObject = function(date, format) {
	    var model = ngModelCtrl.$viewValue ? new Date(ngModelCtrl.$viewValue) : null;
	    return {
	      date: date,
	      label: dateFilter(date, format),
	      selected: model && this.compare(date, model) === 0,
	      disabled: this.isDisabled(date),
	      current: this.compare(date, new Date()) === 0,
	      customClass: this.customClass(date)
	    };
	  };
	
	  this.isDisabled = function(date) {
	    return ((this.minDate && this.compare(date, this.minDate) < 0) || (this.maxDate && this.compare(date, this.maxDate) > 0) || ($attrs.dateDisabled && $scope.dateDisabled({date: date, mode: $scope.datepickerMode})));
	  };
	
	  this.customClass = function(date) {
	    return $scope.customClass({date: date, mode: $scope.datepickerMode});
	  };
	
	  // Split array into smaller arrays
	  this.split = function(arr, size) {
	    var arrays = [];
	    while (arr.length > 0) {
	      arrays.push(arr.splice(0, size));
	    }
	    return arrays;
	  };
	
	  $scope.select = function(date) {
	    if ($scope.datepickerMode === self.minMode) {
	      var dt = ngModelCtrl.$viewValue ? new Date(ngModelCtrl.$viewValue) : new Date(0, 0, 0, 0, 0, 0, 0);
	      dt.setFullYear(date.getFullYear(), date.getMonth(), date.getDate());
	      ngModelCtrl.$setViewValue(dt);
	      ngModelCtrl.$render();
	    } else {
	      self.activeDate = date;
	      $scope.datepickerMode = self.modes[self.modes.indexOf($scope.datepickerMode) - 1];
	    }
	  };
	
	  $scope.move = function(direction) {
	    var year = self.activeDate.getFullYear() + direction * (self.step.years || 0),
	        month = self.activeDate.getMonth() + direction * (self.step.months || 0);
	    self.activeDate.setFullYear(year, month, 1);
	    self.refreshView();
	  };
	
	  $scope.toggleMode = function(direction) {
	    direction = direction || 1;
	
	    if (($scope.datepickerMode === self.maxMode && direction === 1) || ($scope.datepickerMode === self.minMode && direction === -1)) {
	      return;
	    }
	
	    $scope.datepickerMode = self.modes[self.modes.indexOf($scope.datepickerMode) + direction];
	  };
	
	  // Key event mapper
	  $scope.keys = { 13: 'enter', 32: 'space', 33: 'pageup', 34: 'pagedown', 35: 'end', 36: 'home', 37: 'left', 38: 'up', 39: 'right', 40: 'down' };
	
	  var focusElement = function() {
	    self.element[0].focus();
	  };
	
	  // Listen for focus requests from popup directive
	  $scope.$on('uib:datepicker.focus', focusElement);
	
	  $scope.keydown = function(evt) {
	    var key = $scope.keys[evt.which];
	
	    if (!key || evt.shiftKey || evt.altKey) {
	      return;
	    }
	
	    evt.preventDefault();
	    if (!self.shortcutPropagation) {
	      evt.stopPropagation();
	    }
	
	    if (key === 'enter' || key === 'space') {
	      if (self.isDisabled(self.activeDate)) {
	        return; // do nothing
	      }
	      $scope.select(self.activeDate);
	    } else if (evt.ctrlKey && (key === 'up' || key === 'down')) {
	      $scope.toggleMode(key === 'up' ? 1 : -1);
	    } else {
	      self.handleKeyDown(key, evt);
	      self.refreshView();
	    }
	  };
	}])
	
	.controller('UibDaypickerController', ['$scope', '$element', 'dateFilter', function(scope, $element, dateFilter) {
	  var DAYS_IN_MONTH = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
	
	  this.step = { months: 1 };
	  this.element = $element;
	  function getDaysInMonth(year, month) {
	    return ((month === 1) && (year % 4 === 0) && ((year % 100 !== 0) || (year % 400 === 0))) ? 29 : DAYS_IN_MONTH[month];
	  }
	
	  this.init = function(ctrl) {
	    angular.extend(ctrl, this);
	    scope.showWeeks = ctrl.showWeeks;
	    ctrl.refreshView();
	  };
	
	  this.getDates = function(startDate, n) {
	    var dates = new Array(n), current = new Date(startDate), i = 0, date;
	    while (i < n) {
	      date = new Date(current);
	      dates[i++] = date;
	      current.setDate(current.getDate() + 1);
	    }
	    return dates;
	  };
	
	  this._refreshView = function() {
	    var year = this.activeDate.getFullYear(),
	      month = this.activeDate.getMonth(),
	      firstDayOfMonth = new Date(this.activeDate);
	
	    firstDayOfMonth.setFullYear(year, month, 1);
	
	    var difference = this.startingDay - firstDayOfMonth.getDay(),
	      numDisplayedFromPreviousMonth = (difference > 0) ? 7 - difference : - difference,
	      firstDate = new Date(firstDayOfMonth);
	
	    if (numDisplayedFromPreviousMonth > 0) {
	      firstDate.setDate(-numDisplayedFromPreviousMonth + 1);
	    }
	
	    // 42 is the number of days on a six-month calendar
	    var days = this.getDates(firstDate, 42);
	    for (var i = 0; i < 42; i ++) {
	      days[i] = angular.extend(this.createDateObject(days[i], this.formatDay), {
	        secondary: days[i].getMonth() !== month,
	        uid: scope.uniqueId + '-' + i
	      });
	    }
	
	    scope.labels = new Array(7);
	    for (var j = 0; j < 7; j++) {
	      scope.labels[j] = {
	        abbr: dateFilter(days[j].date, this.formatDayHeader),
	        full: dateFilter(days[j].date, 'EEEE')
	      };
	    }
	
	    scope.title = dateFilter(this.activeDate, this.formatDayTitle);
	    scope.rows = this.split(days, 7);
	
	    if (scope.showWeeks) {
	      scope.weekNumbers = [];
	      var thursdayIndex = (4 + 7 - this.startingDay) % 7,
	          numWeeks = scope.rows.length;
	      for (var curWeek = 0; curWeek < numWeeks; curWeek++) {
	        scope.weekNumbers.push(
	          getISO8601WeekNumber(scope.rows[curWeek][thursdayIndex].date));
	      }
	    }
	  };
	
	  this.compare = function(date1, date2) {
	    return (new Date(date1.getFullYear(), date1.getMonth(), date1.getDate()) - new Date(date2.getFullYear(), date2.getMonth(), date2.getDate()));
	  };
	
	  function getISO8601WeekNumber(date) {
	    var checkDate = new Date(date);
	    checkDate.setDate(checkDate.getDate() + 4 - (checkDate.getDay() || 7)); // Thursday
	    var time = checkDate.getTime();
	    checkDate.setMonth(0); // Compare with Jan 1
	    checkDate.setDate(1);
	    return Math.floor(Math.round((time - checkDate) / 86400000) / 7) + 1;
	  }
	
	  this.handleKeyDown = function(key, evt) {
	    var date = this.activeDate.getDate();
	
	    if (key === 'left') {
	      date = date - 1;   // up
	    } else if (key === 'up') {
	      date = date - 7;   // down
	    } else if (key === 'right') {
	      date = date + 1;   // down
	    } else if (key === 'down') {
	      date = date + 7;
	    } else if (key === 'pageup' || key === 'pagedown') {
	      var month = this.activeDate.getMonth() + (key === 'pageup' ? - 1 : 1);
	      this.activeDate.setMonth(month, 1);
	      date = Math.min(getDaysInMonth(this.activeDate.getFullYear(), this.activeDate.getMonth()), date);
	    } else if (key === 'home') {
	      date = 1;
	    } else if (key === 'end') {
	      date = getDaysInMonth(this.activeDate.getFullYear(), this.activeDate.getMonth());
	    }
	    this.activeDate.setDate(date);
	  };
	}])
	
	.controller('UibMonthpickerController', ['$scope', '$element', 'dateFilter', function(scope, $element, dateFilter) {
	  this.step = { years: 1 };
	  this.element = $element;
	
	  this.init = function(ctrl) {
	    angular.extend(ctrl, this);
	    ctrl.refreshView();
	  };
	
	  this._refreshView = function() {
	    var months = new Array(12),
	        year = this.activeDate.getFullYear(),
	        date;
	
	    for (var i = 0; i < 12; i++) {
	      date = new Date(this.activeDate);
	      date.setFullYear(year, i, 1);
	      months[i] = angular.extend(this.createDateObject(date, this.formatMonth), {
	        uid: scope.uniqueId + '-' + i
	      });
	    }
	
	    scope.title = dateFilter(this.activeDate, this.formatMonthTitle);
	    scope.rows = this.split(months, 3);
	  };
	
	  this.compare = function(date1, date2) {
	    return new Date(date1.getFullYear(), date1.getMonth()) - new Date(date2.getFullYear(), date2.getMonth());
	  };
	
	  this.handleKeyDown = function(key, evt) {
	    var date = this.activeDate.getMonth();
	
	    if (key === 'left') {
	      date = date - 1;   // up
	    } else if (key === 'up') {
	      date = date - 3;   // down
	    } else if (key === 'right') {
	      date = date + 1;   // down
	    } else if (key === 'down') {
	      date = date + 3;
	    } else if (key === 'pageup' || key === 'pagedown') {
	      var year = this.activeDate.getFullYear() + (key === 'pageup' ? - 1 : 1);
	      this.activeDate.setFullYear(year);
	    } else if (key === 'home') {
	      date = 0;
	    } else if (key === 'end') {
	      date = 11;
	    }
	    this.activeDate.setMonth(date);
	  };
	}])
	
	.controller('UibYearpickerController', ['$scope', '$element', 'dateFilter', function(scope, $element, dateFilter) {
	  var range;
	  this.element = $element;
	
	  function getStartingYear(year) {
	    return parseInt((year - 1) / range, 10) * range + 1;
	  }
	
	  this.yearpickerInit = function() {
	    range = this.yearRange;
	    this.step = { years: range };
	  };
	
	  this._refreshView = function() {
	    var years = new Array(range), date;
	
	    for (var i = 0, start = getStartingYear(this.activeDate.getFullYear()); i < range; i++) {
	      date = new Date(this.activeDate);
	      date.setFullYear(start + i, 0, 1);
	      years[i] = angular.extend(this.createDateObject(date, this.formatYear), {
	        uid: scope.uniqueId + '-' + i
	      });
	    }
	
	    scope.title = [years[0].label, years[range - 1].label].join(' - ');
	    scope.rows = this.split(years, 5);
	  };
	
	  this.compare = function(date1, date2) {
	    return date1.getFullYear() - date2.getFullYear();
	  };
	
	  this.handleKeyDown = function(key, evt) {
	    var date = this.activeDate.getFullYear();
	
	    if (key === 'left') {
	      date = date - 1;   // up
	    } else if (key === 'up') {
	      date = date - 5;   // down
	    } else if (key === 'right') {
	      date = date + 1;   // down
	    } else if (key === 'down') {
	      date = date + 5;
	    } else if (key === 'pageup' || key === 'pagedown') {
	      date += (key === 'pageup' ? - 1 : 1) * this.step.years;
	    } else if (key === 'home') {
	      date = getStartingYear(this.activeDate.getFullYear());
	    } else if (key === 'end') {
	      date = getStartingYear(this.activeDate.getFullYear()) + range - 1;
	    }
	    this.activeDate.setFullYear(date);
	  };
	}])
	
	.directive('uibDatepicker', function() {
	  return {
	    replace: true,
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/datepicker/datepicker.html';
	    },
	    scope: {
	      datepickerMode: '=?',
	      dateDisabled: '&',
	      customClass: '&',
	      shortcutPropagation: '&?'
	    },
	    require: ['uibDatepicker', '^ngModel'],
	    controller: 'UibDatepickerController',
	    controllerAs: 'datepicker',
	    link: function(scope, element, attrs, ctrls) {
	      var datepickerCtrl = ctrls[0], ngModelCtrl = ctrls[1];
	
	      datepickerCtrl.init(ngModelCtrl);
	    }
	  };
	})
	
	.directive('uibDaypicker', function() {
	  return {
	    replace: true,
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/datepicker/day.html';
	    },
	    require: ['^?uibDatepicker', 'uibDaypicker', '^?datepicker'],
	    controller: 'UibDaypickerController',
	    link: function(scope, element, attrs, ctrls) {
	      var datepickerCtrl = ctrls[0] || ctrls[2],
	        daypickerCtrl = ctrls[1];
	
	      daypickerCtrl.init(datepickerCtrl);
	    }
	  };
	})
	
	.directive('uibMonthpicker', function() {
	  return {
	    replace: true,
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/datepicker/month.html';
	    },
	    require: ['^?uibDatepicker', 'uibMonthpicker', '^?datepicker'],
	    controller: 'UibMonthpickerController',
	    link: function(scope, element, attrs, ctrls) {
	      var datepickerCtrl = ctrls[0] || ctrls[2],
	        monthpickerCtrl = ctrls[1];
	
	      monthpickerCtrl.init(datepickerCtrl);
	    }
	  };
	})
	
	.directive('uibYearpicker', function() {
	  return {
	    replace: true,
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/datepicker/year.html';
	    },
	    require: ['^?uibDatepicker', 'uibYearpicker', '^?datepicker'],
	    controller: 'UibYearpickerController',
	    link: function(scope, element, attrs, ctrls) {
	      var ctrl = ctrls[0] || ctrls[2];
	      angular.extend(ctrl, ctrls[1]);
	      ctrl.yearpickerInit();
	
	      ctrl.refreshView();
	    }
	  };
	})
	
	.constant('uibDatepickerPopupConfig', {
	  datepickerPopup: 'yyyy-MM-dd',
	  datepickerPopupTemplateUrl: 'template/datepicker/popup.html',
	  datepickerTemplateUrl: 'template/datepicker/datepicker.html',
	  html5Types: {
	    date: 'yyyy-MM-dd',
	    'datetime-local': 'yyyy-MM-ddTHH:mm:ss.sss',
	    'month': 'yyyy-MM'
	  },
	  currentText: 'Today',
	  clearText: 'Clear',
	  closeText: 'Done',
	  closeOnDateSelection: true,
	  appendToBody: false,
	  showButtonBar: true,
	  onOpenFocus: true
	})
	
	.controller('UibDatepickerPopupController', ['$scope', '$element', '$attrs', '$compile', '$parse', '$document', '$rootScope', '$uibPosition', 'dateFilter', 'uibDateParser', 'uibDatepickerPopupConfig', '$timeout',
	function(scope, element, attrs, $compile, $parse, $document, $rootScope, $position, dateFilter, dateParser, datepickerPopupConfig, $timeout) {
	  var self = this;
	  var cache = {},
	    isHtml5DateInput = false;
	  var dateFormat, closeOnDateSelection, appendToBody, onOpenFocus,
	    datepickerPopupTemplateUrl, datepickerTemplateUrl, popupEl, datepickerEl,
	    ngModel, $popup;
	
	  scope.watchData = {};
	
	  this.init = function(_ngModel_) {
	    ngModel = _ngModel_;
	    closeOnDateSelection = angular.isDefined(attrs.closeOnDateSelection) ? scope.$parent.$eval(attrs.closeOnDateSelection) : datepickerPopupConfig.closeOnDateSelection;
	    appendToBody = angular.isDefined(attrs.datepickerAppendToBody) ? scope.$parent.$eval(attrs.datepickerAppendToBody) : datepickerPopupConfig.appendToBody;
	    onOpenFocus = angular.isDefined(attrs.onOpenFocus) ? scope.$parent.$eval(attrs.onOpenFocus) : datepickerPopupConfig.onOpenFocus;
	    datepickerPopupTemplateUrl = angular.isDefined(attrs.datepickerPopupTemplateUrl) ? attrs.datepickerPopupTemplateUrl : datepickerPopupConfig.datepickerPopupTemplateUrl;
	    datepickerTemplateUrl = angular.isDefined(attrs.datepickerTemplateUrl) ? attrs.datepickerTemplateUrl : datepickerPopupConfig.datepickerTemplateUrl;
	
	    scope.showButtonBar = angular.isDefined(attrs.showButtonBar) ? scope.$parent.$eval(attrs.showButtonBar) : datepickerPopupConfig.showButtonBar;
	
	    if (datepickerPopupConfig.html5Types[attrs.type]) {
	      dateFormat = datepickerPopupConfig.html5Types[attrs.type];
	      isHtml5DateInput = true;
	    } else {
	      dateFormat = attrs.datepickerPopup || attrs.uibDatepickerPopup || datepickerPopupConfig.datepickerPopup;
	      attrs.$observe('uibDatepickerPopup', function(value, oldValue) {
	          var newDateFormat = value || datepickerPopupConfig.datepickerPopup;
	          // Invalidate the $modelValue to ensure that formatters re-run
	          // FIXME: Refactor when PR is merged: https://github.com/angular/angular.js/pull/10764
	          if (newDateFormat !== dateFormat) {
	            dateFormat = newDateFormat;
	            ngModel.$modelValue = null;
	
	            if (!dateFormat) {
	              throw new Error('uibDatepickerPopup must have a date format specified.');
	            }
	          }
	      });
	    }
	
	    if (!dateFormat) {
	      throw new Error('uibDatepickerPopup must have a date format specified.');
	    }
	
	    if (isHtml5DateInput && attrs.datepickerPopup) {
	      throw new Error('HTML5 date input types do not support custom formats.');
	    }
	
	    // popup element used to display calendar
	    popupEl = angular.element('<div uib-datepicker-popup-wrap><div uib-datepicker></div></div>');
	    popupEl.attr({
	      'ng-model': 'date',
	      'ng-change': 'dateSelection(date)',
	      'template-url': datepickerPopupTemplateUrl
	    });
	
	    // datepicker element
	    datepickerEl = angular.element(popupEl.children()[0]);
	    datepickerEl.attr('template-url', datepickerTemplateUrl);
	
	    if (isHtml5DateInput) {
	      if (attrs.type === 'month') {
	        datepickerEl.attr('datepicker-mode', '"month"');
	        datepickerEl.attr('min-mode', 'month');
	      }
	    }
	
	    if (attrs.datepickerOptions) {
	      var options = scope.$parent.$eval(attrs.datepickerOptions);
	      if (options && options.initDate) {
	        scope.initDate = options.initDate;
	        datepickerEl.attr('init-date', 'initDate');
	        delete options.initDate;
	      }
	      angular.forEach(options, function(value, option) {
	        datepickerEl.attr(cameltoDash(option), value);
	      });
	    }
	
	    angular.forEach(['minMode', 'maxMode', 'minDate', 'maxDate', 'datepickerMode', 'initDate', 'shortcutPropagation'], function(key) {
	      if (attrs[key]) {
	        var getAttribute = $parse(attrs[key]);
	        scope.$parent.$watch(getAttribute, function(value) {
	          scope.watchData[key] = value;
	          if (key === 'minDate' || key === 'maxDate') {
	            cache[key] = new Date(value);
	          }
	        });
	        datepickerEl.attr(cameltoDash(key), 'watchData.' + key);
	
	        // Propagate changes from datepicker to outside
	        if (key === 'datepickerMode') {
	          var setAttribute = getAttribute.assign;
	          scope.$watch('watchData.' + key, function(value, oldvalue) {
	            if (angular.isFunction(setAttribute) && value !== oldvalue) {
	              setAttribute(scope.$parent, value);
	            }
	          });
	        }
	      }
	    });
	    if (attrs.dateDisabled) {
	      datepickerEl.attr('date-disabled', 'dateDisabled({ date: date, mode: mode })');
	    }
	
	    if (attrs.showWeeks) {
	      datepickerEl.attr('show-weeks', attrs.showWeeks);
	    }
	
	    if (attrs.customClass) {
	      datepickerEl.attr('custom-class', 'customClass({ date: date, mode: mode })');
	    }
	
	    if (!isHtml5DateInput) {
	      // Internal API to maintain the correct ng-invalid-[key] class
	      ngModel.$$parserName = 'date';
	      ngModel.$validators.date = validator;
	      ngModel.$parsers.unshift(parseDate);
	      ngModel.$formatters.push(function(value) {
	        scope.date = value;
	        return ngModel.$isEmpty(value) ? value : dateFilter(value, dateFormat);
	      });
	    } else {
	      ngModel.$formatters.push(function(value) {
	        scope.date = value;
	        return value;
	      });
	    }
	
	    // Detect changes in the view from the text box
	    ngModel.$viewChangeListeners.push(function() {
	      scope.date = dateParser.parse(ngModel.$viewValue, dateFormat, scope.date);
	    });
	
	    element.bind('keydown', inputKeydownBind);
	
	    $popup = $compile(popupEl)(scope);
	    // Prevent jQuery cache memory leak (template is now redundant after linking)
	    popupEl.remove();
	
	    if (appendToBody) {
	      $document.find('body').append($popup);
	    } else {
	      element.after($popup);
	    }
	
	    scope.$on('$destroy', function() {
	      if (scope.isOpen === true) {
	        if (!$rootScope.$$phase) {
	          scope.$apply(function() {
	            scope.isOpen = false;
	          });
	        }
	      }
	
	      $popup.remove();
	      element.unbind('keydown', inputKeydownBind);
	      $document.unbind('click', documentClickBind);
	    });
	  };
	
	  scope.getText = function(key) {
	    return scope[key + 'Text'] || datepickerPopupConfig[key + 'Text'];
	  };
	
	  scope.isDisabled = function(date) {
	    if (date === 'today') {
	      date = new Date();
	    }
	
	    return ((scope.watchData.minDate && scope.compare(date, cache.minDate) < 0) ||
	      (scope.watchData.maxDate && scope.compare(date, cache.maxDate) > 0));
	  };
	
	  scope.compare = function(date1, date2) {
	    return (new Date(date1.getFullYear(), date1.getMonth(), date1.getDate()) - new Date(date2.getFullYear(), date2.getMonth(), date2.getDate()));
	  };
	
	  // Inner change
	  scope.dateSelection = function(dt) {
	    if (angular.isDefined(dt)) {
	      scope.date = dt;
	    }
	    var date = scope.date ? dateFilter(scope.date, dateFormat) : null; // Setting to NULL is necessary for form validators to function
	    element.val(date);
	    ngModel.$setViewValue(date);
	
	    if (closeOnDateSelection) {
	      scope.isOpen = false;
	      element[0].focus();
	    }
	  };
	
	  scope.keydown = function(evt) {
	    if (evt.which === 27) {
	      scope.isOpen = false;
	      element[0].focus();
	    }
	  };
	
	  scope.select = function(date) {
	    if (date === 'today') {
	      var today = new Date();
	      if (angular.isDate(scope.date)) {
	        date = new Date(scope.date);
	        date.setFullYear(today.getFullYear(), today.getMonth(), today.getDate());
	      } else {
	        date = new Date(today.setHours(0, 0, 0, 0));
	      }
	    }
	    scope.dateSelection(date);
	  };
	
	  scope.close = function() {
	    scope.isOpen = false;
	    element[0].focus();
	  };
	
	  scope.$watch('isOpen', function(value) {
	    if (value) {
	      scope.position = appendToBody ? $position.offset(element) : $position.position(element);
	      scope.position.top = scope.position.top + element.prop('offsetHeight');
	
	      $timeout(function() {
	        if (onOpenFocus) {
	          scope.$broadcast('uib:datepicker.focus');
	        }
	        $document.bind('click', documentClickBind);
	      }, 0, false);
	    } else {
	      $document.unbind('click', documentClickBind);
	    }
	  });
	
	  function cameltoDash(string) {
	    return string.replace(/([A-Z])/g, function($1) { return '-' + $1.toLowerCase(); });
	  }
	
	  function parseDate(viewValue) {
	    if (angular.isNumber(viewValue)) {
	      // presumably timestamp to date object
	      viewValue = new Date(viewValue);
	    }
	
	    if (!viewValue) {
	      return null;
	    } else if (angular.isDate(viewValue) && !isNaN(viewValue)) {
	      return viewValue;
	    } else if (angular.isString(viewValue)) {
	      var date = dateParser.parse(viewValue, dateFormat, scope.date);
	      if (isNaN(date)) {
	        return undefined;
	      } else {
	        return date;
	      }
	    } else {
	      return undefined;
	    }
	  }
	
	  function validator(modelValue, viewValue) {
	    var value = modelValue || viewValue;
	
	    if (!attrs.ngRequired && !value) {
	      return true;
	    }
	
	    if (angular.isNumber(value)) {
	      value = new Date(value);
	    }
	    if (!value) {
	      return true;
	    } else if (angular.isDate(value) && !isNaN(value)) {
	      return true;
	    } else if (angular.isString(value)) {
	      var date = dateParser.parse(value, dateFormat);
	      return !isNaN(date);
	    } else {
	      return false;
	    }
	  }
	
	  function documentClickBind(event) {
	    var popup = $popup[0];
	    var dpContainsTarget = element[0].contains(event.target);
	    // The popup node may not be an element node
	    // In some browsers (IE) only element nodes have the 'contains' function
	    var popupContainsTarget = popup.contains !== undefined && popup.contains(event.target);
	    if (scope.isOpen && !(dpContainsTarget || popupContainsTarget)) {
	      scope.$apply(function() {
	        scope.isOpen = false;
	      });
	    }
	  }
	
	  function inputKeydownBind(evt) {
	    if (evt.which === 27 && scope.isOpen) {
	      evt.preventDefault();
	      evt.stopPropagation();
	      scope.$apply(function() {
	        scope.isOpen = false;
	      });
	      element[0].focus();
	    } else if (evt.which === 40 && !scope.isOpen) {
	      evt.preventDefault();
	      evt.stopPropagation();
	      scope.$apply(function() {
	        scope.isOpen = true;
	      });
	    }
	  }
	}])
	
	.directive('uibDatepickerPopup', function() {
	  return {
	    require: ['ngModel', 'uibDatepickerPopup'],
	    controller: 'UibDatepickerPopupController',
	    scope: {
	      isOpen: '=?',
	      currentText: '@',
	      clearText: '@',
	      closeText: '@',
	      dateDisabled: '&',
	      customClass: '&'
	    },
	    link: function(scope, element, attrs, ctrls) {
	      var ngModel = ctrls[0],
	        ctrl = ctrls[1];
	
	      ctrl.init(ngModel);
	    }
	  };
	})
	
	.directive('uibDatepickerPopupWrap', function() {
	  return {
	    replace: true,
	    transclude: true,
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/datepicker/popup.html';
	    }
	  };
	});
	
	/* Deprecated datepicker below */
	
	angular.module('ui.bootstrap.datepicker')
	
	.value('$datepickerSuppressWarning', false)
	
	.controller('DatepickerController', ['$scope', '$attrs', '$parse', '$interpolate', '$log', 'dateFilter', 'uibDatepickerConfig', '$datepickerSuppressError', '$datepickerSuppressWarning', function($scope, $attrs, $parse, $interpolate, $log, dateFilter, datepickerConfig, $datepickerSuppressError, $datepickerSuppressWarning) {
	  if (!$datepickerSuppressWarning) {
	    $log.warn('DatepickerController is now deprecated. Use UibDatepickerController instead.');
	  }
	
	  var self = this,
	    ngModelCtrl = { $setViewValue: angular.noop }; // nullModelCtrl;
	
	  this.modes = ['day', 'month', 'year'];
	
	  angular.forEach(['formatDay', 'formatMonth', 'formatYear', 'formatDayHeader', 'formatDayTitle', 'formatMonthTitle',
	    'showWeeks', 'startingDay', 'yearRange', 'shortcutPropagation'], function(key, index) {
	    self[key] = angular.isDefined($attrs[key]) ? (index < 6 ? $interpolate($attrs[key])($scope.$parent) : $scope.$parent.$eval($attrs[key])) : datepickerConfig[key];
	  });
	
	  angular.forEach(['minDate', 'maxDate'], function(key) {
	    if ($attrs[key]) {
	      $scope.$parent.$watch($parse($attrs[key]), function(value) {
	        self[key] = value ? new Date(value) : null;
	        self.refreshView();
	      });
	    } else {
	      self[key] = datepickerConfig[key] ? new Date(datepickerConfig[key]) : null;
	    }
	  });
	
	  angular.forEach(['minMode', 'maxMode'], function(key) {
	    if ($attrs[key]) {
	      $scope.$parent.$watch($parse($attrs[key]), function(value) {
	        self[key] = angular.isDefined(value) ? value : $attrs[key];
	        $scope[key] = self[key];
	        if ((key == 'minMode' && self.modes.indexOf($scope.datepickerMode) < self.modes.indexOf(self[key])) || (key == 'maxMode' && self.modes.indexOf($scope.datepickerMode) > self.modes.indexOf(self[key]))) {
	          $scope.datepickerMode = self[key];
	        }
	      });
	    } else {
	      self[key] = datepickerConfig[key] || null;
	      $scope[key] = self[key];
	    }
	  });
	
	  $scope.datepickerMode = $scope.datepickerMode || datepickerConfig.datepickerMode;
	  $scope.uniqueId = 'datepicker-' + $scope.$id + '-' + Math.floor(Math.random() * 10000);
	
	  if (angular.isDefined($attrs.initDate)) {
	    this.activeDate = $scope.$parent.$eval($attrs.initDate) || new Date();
	    $scope.$parent.$watch($attrs.initDate, function(initDate) {
	      if (initDate && (ngModelCtrl.$isEmpty(ngModelCtrl.$modelValue) || ngModelCtrl.$invalid)) {
	        self.activeDate = initDate;
	        self.refreshView();
	      }
	    });
	  } else {
	    this.activeDate = new Date();
	  }
	
	  $scope.isActive = function(dateObject) {
	    if (self.compare(dateObject.date, self.activeDate) === 0) {
	      $scope.activeDateId = dateObject.uid;
	      return true;
	    }
	    return false;
	  };
	
	  this.init = function(ngModelCtrl_) {
	    ngModelCtrl = ngModelCtrl_;
	
	    ngModelCtrl.$render = function() {
	      self.render();
	    };
	  };
	
	  this.render = function() {
	    if (ngModelCtrl.$viewValue) {
	      var date = new Date(ngModelCtrl.$viewValue),
	        isValid = !isNaN(date);
	
	      if (isValid) {
	        this.activeDate = date;
	      } else if (!$datepickerSuppressError) {
	        $log.error('Datepicker directive: "ng-model" value must be a Date object, a number of milliseconds since 01.01.1970 or a string representing an RFC2822 or ISO 8601 date.');
	      }
	    }
	    this.refreshView();
	  };
	
	  this.refreshView = function() {
	    if (this.element) {
	      this._refreshView();
	
	      var date = ngModelCtrl.$viewValue ? new Date(ngModelCtrl.$viewValue) : null;
	      ngModelCtrl.$setValidity('dateDisabled', !date || (this.element && !this.isDisabled(date)));
	    }
	  };
	
	  this.createDateObject = function(date, format) {
	    var model = ngModelCtrl.$viewValue ? new Date(ngModelCtrl.$viewValue) : null;
	    return {
	      date: date,
	      label: dateFilter(date, format),
	      selected: model && this.compare(date, model) === 0,
	      disabled: this.isDisabled(date),
	      current: this.compare(date, new Date()) === 0,
	      customClass: this.customClass(date)
	    };
	  };
	
	  this.isDisabled = function(date) {
	    return ((this.minDate && this.compare(date, this.minDate) < 0) || (this.maxDate && this.compare(date, this.maxDate) > 0) || ($attrs.dateDisabled && $scope.dateDisabled({date: date, mode: $scope.datepickerMode})));
	  };
	
	  this.customClass = function(date) {
	    return $scope.customClass({date: date, mode: $scope.datepickerMode});
	  };
	
	  // Split array into smaller arrays
	  this.split = function(arr, size) {
	    var arrays = [];
	    while (arr.length > 0) {
	      arrays.push(arr.splice(0, size));
	    }
	    return arrays;
	  };
	
	  this.fixTimeZone = function(date) {
	    var hours = date.getHours();
	    date.setHours(hours === 23 ? hours + 2 : 0);
	  };
	
	  $scope.select = function(date) {
	    if ($scope.datepickerMode === self.minMode) {
	      var dt = ngModelCtrl.$viewValue ? new Date(ngModelCtrl.$viewValue) : new Date(0, 0, 0, 0, 0, 0, 0);
	      dt.setFullYear(date.getFullYear(), date.getMonth(), date.getDate());
	      ngModelCtrl.$setViewValue(dt);
	      ngModelCtrl.$render();
	    } else {
	      self.activeDate = date;
	      $scope.datepickerMode = self.modes[self.modes.indexOf($scope.datepickerMode) - 1];
	    }
	  };
	
	  $scope.move = function(direction) {
	    var year = self.activeDate.getFullYear() + direction * (self.step.years || 0),
	      month = self.activeDate.getMonth() + direction * (self.step.months || 0);
	    self.activeDate.setFullYear(year, month, 1);
	    self.refreshView();
	  };
	
	  $scope.toggleMode = function(direction) {
	    direction = direction || 1;
	
	    if (($scope.datepickerMode === self.maxMode && direction === 1) || ($scope.datepickerMode === self.minMode && direction === -1)) {
	      return;
	    }
	
	    $scope.datepickerMode = self.modes[self.modes.indexOf($scope.datepickerMode) + direction];
	  };
	
	  // Key event mapper
	  $scope.keys = { 13: 'enter', 32: 'space', 33: 'pageup', 34: 'pagedown', 35: 'end', 36: 'home', 37: 'left', 38: 'up', 39: 'right', 40: 'down' };
	
	  var focusElement = function() {
	    self.element[0].focus();
	  };
	
	  $scope.$on('uib:datepicker.focus', focusElement);
	
	  $scope.keydown = function(evt) {
	    var key = $scope.keys[evt.which];
	
	    if (!key || evt.shiftKey || evt.altKey) {
	      return;
	    }
	
	    evt.preventDefault();
	    if (!self.shortcutPropagation) {
	      evt.stopPropagation();
	    }
	
	    if (key === 'enter' || key === 'space') {
	      if (self.isDisabled(self.activeDate)) {
	        return; // do nothing
	      }
	      $scope.select(self.activeDate);
	    } else if (evt.ctrlKey && (key === 'up' || key === 'down')) {
	      $scope.toggleMode(key === 'up' ? 1 : -1);
	    } else {
	      self.handleKeyDown(key, evt);
	      self.refreshView();
	    }
	  };
	}])
	
	.directive('datepicker', ['$log', '$datepickerSuppressWarning', function($log, $datepickerSuppressWarning) {
	  return {
	    replace: true,
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/datepicker/datepicker.html';
	    },
	    scope: {
	      datepickerMode: '=?',
	      dateDisabled: '&',
	      customClass: '&',
	      shortcutPropagation: '&?'
	    },
	    require: ['datepicker', '^ngModel'],
	    controller: 'DatepickerController',
	    controllerAs: 'datepicker',
	    link: function(scope, element, attrs, ctrls) {
	      if (!$datepickerSuppressWarning) {
	        $log.warn('datepicker is now deprecated. Use uib-datepicker instead.');
	      }
	
	      var datepickerCtrl = ctrls[0], ngModelCtrl = ctrls[1];
	
	      datepickerCtrl.init(ngModelCtrl);
	    }
	  };
	}])
	
	.directive('daypicker', ['$log', '$datepickerSuppressWarning', function($log, $datepickerSuppressWarning) {
	  return {
	    replace: true,
	    templateUrl: 'template/datepicker/day.html',
	    require: ['^datepicker', 'daypicker'],
	    controller: 'UibDaypickerController',
	    link: function(scope, element, attrs, ctrls) {
	      if (!$datepickerSuppressWarning) {
	        $log.warn('daypicker is now deprecated. Use uib-daypicker instead.');
	      }
	
	      var datepickerCtrl = ctrls[0],
	        daypickerCtrl = ctrls[1];
	
	      daypickerCtrl.init(datepickerCtrl);
	    }
	  };
	}])
	
	.directive('monthpicker', ['$log', '$datepickerSuppressWarning', function($log, $datepickerSuppressWarning) {
	  return {
	    replace: true,
	    templateUrl: 'template/datepicker/month.html',
	    require: ['^datepicker', 'monthpicker'],
	    controller: 'UibMonthpickerController',
	    link: function(scope, element, attrs, ctrls) {
	      if (!$datepickerSuppressWarning) {
	        $log.warn('monthpicker is now deprecated. Use uib-monthpicker instead.');
	      }
	
	      var datepickerCtrl = ctrls[0],
	        monthpickerCtrl = ctrls[1];
	
	      monthpickerCtrl.init(datepickerCtrl);
	    }
	  };
	}])
	
	.directive('yearpicker', ['$log', '$datepickerSuppressWarning', function($log, $datepickerSuppressWarning) {
	  return {
	    replace: true,
	    templateUrl: 'template/datepicker/year.html',
	    require: ['^datepicker', 'yearpicker'],
	    controller: 'UibYearpickerController',
	    link: function(scope, element, attrs, ctrls) {
	      if (!$datepickerSuppressWarning) {
	        $log.warn('yearpicker is now deprecated. Use uib-yearpicker instead.');
	      }
	
	      var ctrl = ctrls[0];
	      angular.extend(ctrl, ctrls[1]);
	      ctrl.yearpickerInit();
	
	      ctrl.refreshView();
	    }
	  };
	}])
	
	.directive('datepickerPopup', ['$log', '$datepickerSuppressWarning', function($log, $datepickerSuppressWarning) {
	  return {
	    require: ['ngModel', 'datepickerPopup'],
	    controller: 'UibDatepickerPopupController',
	    scope: {
	      isOpen: '=?',
	      currentText: '@',
	      clearText: '@',
	      closeText: '@',
	      dateDisabled: '&',
	      customClass: '&'
	    },
	    link: function(scope, element, attrs, ctrls) {
	      if (!$datepickerSuppressWarning) {
	        $log.warn('datepicker-popup is now deprecated. Use uib-datepicker-popup instead.');
	      }
	
	      var ngModel = ctrls[0],
	        ctrl = ctrls[1];
	
	      ctrl.init(ngModel);
	    }
	  };
	}])
	
	.directive('datepickerPopupWrap', ['$log', '$datepickerSuppressWarning', function($log, $datepickerSuppressWarning) {
	  return {
	    replace: true,
	    transclude: true,
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/datepicker/popup.html';
	    },
	    link: function() {
	      if (!$datepickerSuppressWarning) {
	        $log.warn('datepicker-popup-wrap is now deprecated. Use uib-datepicker-popup-wrap instead.');
	      }
	    }
	  };
	}]);
	
	angular.module('ui.bootstrap.dropdown', ['ui.bootstrap.position'])
	
	.constant('uibDropdownConfig', {
	  openClass: 'open'
	})
	
	.service('uibDropdownService', ['$document', '$rootScope', function($document, $rootScope) {
	  var openScope = null;
	
	  this.open = function(dropdownScope) {
	    if (!openScope) {
	      $document.bind('click', closeDropdown);
	      $document.bind('keydown', keybindFilter);
	    }
	
	    if (openScope && openScope !== dropdownScope) {
	      openScope.isOpen = false;
	    }
	
	    openScope = dropdownScope;
	  };
	
	  this.close = function(dropdownScope) {
	    if (openScope === dropdownScope) {
	      openScope = null;
	      $document.unbind('click', closeDropdown);
	      $document.unbind('keydown', keybindFilter);
	    }
	  };
	
	  var closeDropdown = function(evt) {
	    // This method may still be called during the same mouse event that
	    // unbound this event handler. So check openScope before proceeding.
	    if (!openScope) { return; }
	
	    if (evt && openScope.getAutoClose() === 'disabled')  { return ; }
	
	    var toggleElement = openScope.getToggleElement();
	    if (evt && toggleElement && toggleElement[0].contains(evt.target)) {
	      return;
	    }
	
	    var dropdownElement = openScope.getDropdownElement();
	    if (evt && openScope.getAutoClose() === 'outsideClick' &&
	      dropdownElement && dropdownElement[0].contains(evt.target)) {
	      return;
	    }
	
	    openScope.isOpen = false;
	
	    if (!$rootScope.$$phase) {
	      openScope.$apply();
	    }
	  };
	
	  var keybindFilter = function(evt) {
	    if (evt.which === 27) {
	      openScope.focusToggleElement();
	      closeDropdown();
	    } else if (openScope.isKeynavEnabled() && /(38|40)/.test(evt.which) && openScope.isOpen) {
	      evt.preventDefault();
	      evt.stopPropagation();
	      openScope.focusDropdownEntry(evt.which);
	    }
	  };
	}])
	
	.controller('UibDropdownController', ['$scope', '$element', '$attrs', '$parse', 'uibDropdownConfig', 'uibDropdownService', '$animate', '$uibPosition', '$document', '$compile', '$templateRequest', function($scope, $element, $attrs, $parse, dropdownConfig, uibDropdownService, $animate, $position, $document, $compile, $templateRequest) {
	  var self = this,
	    scope = $scope.$new(), // create a child scope so we are not polluting original one
	    templateScope,
	    openClass = dropdownConfig.openClass,
	    getIsOpen,
	    setIsOpen = angular.noop,
	    toggleInvoker = $attrs.onToggle ? $parse($attrs.onToggle) : angular.noop,
	    appendToBody = false,
	    keynavEnabled =false,
	    selectedOption = null;
	
	
	  $element.addClass('dropdown');
	
	  this.init = function() {
	    if ($attrs.isOpen) {
	      getIsOpen = $parse($attrs.isOpen);
	      setIsOpen = getIsOpen.assign;
	
	      $scope.$watch(getIsOpen, function(value) {
	        scope.isOpen = !!value;
	      });
	    }
	
	    appendToBody = angular.isDefined($attrs.dropdownAppendToBody);
	    keynavEnabled = angular.isDefined($attrs.uibKeyboardNav);
	
	    if (appendToBody && self.dropdownMenu) {
	      $document.find('body').append(self.dropdownMenu);
	      $element.on('$destroy', function handleDestroyEvent() {
	        self.dropdownMenu.remove();
	      });
	    }
	  };
	
	  this.toggle = function(open) {
	    return scope.isOpen = arguments.length ? !!open : !scope.isOpen;
	  };
	
	  // Allow other directives to watch status
	  this.isOpen = function() {
	    return scope.isOpen;
	  };
	
	  scope.getToggleElement = function() {
	    return self.toggleElement;
	  };
	
	  scope.getAutoClose = function() {
	    return $attrs.autoClose || 'always'; //or 'outsideClick' or 'disabled'
	  };
	
	  scope.getElement = function() {
	    return $element;
	  };
	
	  scope.isKeynavEnabled = function() {
	    return keynavEnabled;
	  };
	
	  scope.focusDropdownEntry = function(keyCode) {
	    var elems = self.dropdownMenu ? //If append to body is used.
	      (angular.element(self.dropdownMenu).find('a')) :
	      (angular.element($element).find('ul').eq(0).find('a'));
	
	    switch (keyCode) {
	      case (40): {
	        if (!angular.isNumber(self.selectedOption)) {
	          self.selectedOption = 0;
	        } else {
	          self.selectedOption = (self.selectedOption === elems.length - 1 ?
	            self.selectedOption :
	            self.selectedOption + 1);
	        }
	        break;
	      }
	      case (38): {
	        if (!angular.isNumber(self.selectedOption)) {
	          self.selectedOption = elems.length - 1;
	        } else {
	          self.selectedOption = self.selectedOption === 0 ?
	            0 : self.selectedOption - 1;
	        }
	        break;
	      }
	    }
	    elems[self.selectedOption].focus();
	  };
	
	  scope.getDropdownElement = function() {
	    return self.dropdownMenu;
	  };
	
	  scope.focusToggleElement = function() {
	    if (self.toggleElement) {
	      self.toggleElement[0].focus();
	    }
	  };
	
	  scope.$watch('isOpen', function(isOpen, wasOpen) {
	    if (appendToBody && self.dropdownMenu) {
	      var pos = $position.positionElements($element, self.dropdownMenu, 'bottom-left', true);
	      var css = {
	        top: pos.top + 'px',
	        display: isOpen ? 'block' : 'none'
	      };
	
	      var rightalign = self.dropdownMenu.hasClass('dropdown-menu-right');
	      if (!rightalign) {
	        css.left = pos.left + 'px';
	        css.right = 'auto';
	      } else {
	        css.left = 'auto';
	        css.right = (window.innerWidth - (pos.left + $element.prop('offsetWidth'))) + 'px';
	      }
	
	      self.dropdownMenu.css(css);
	    }
	
	    $animate[isOpen ? 'addClass' : 'removeClass']($element, openClass).then(function() {
	      if (angular.isDefined(isOpen) && isOpen !== wasOpen) {
	        toggleInvoker($scope, { open: !!isOpen });
	      }
	    });
	
	    if (isOpen) {
	      if (self.dropdownMenuTemplateUrl) {
	        $templateRequest(self.dropdownMenuTemplateUrl).then(function(tplContent) {
	          templateScope = scope.$new();
	          $compile(tplContent.trim())(templateScope, function(dropdownElement) {
	            var newEl = dropdownElement;
	            self.dropdownMenu.replaceWith(newEl);
	            self.dropdownMenu = newEl;
	          });
	        });
	      }
	
	      scope.focusToggleElement();
	      uibDropdownService.open(scope);
	    } else {
	      if (self.dropdownMenuTemplateUrl) {
	        if (templateScope) {
	          templateScope.$destroy();
	        }
	        var newEl = angular.element('<ul class="dropdown-menu"></ul>');
	        self.dropdownMenu.replaceWith(newEl);
	        self.dropdownMenu = newEl;
	      }
	
	      uibDropdownService.close(scope);
	      self.selectedOption = null;
	    }
	
	    if (angular.isFunction(setIsOpen)) {
	      setIsOpen($scope, isOpen);
	    }
	  });
	
	  $scope.$on('$locationChangeSuccess', function() {
	    if (scope.getAutoClose() !== 'disabled') {
	      scope.isOpen = false;
	    }
	  });
	
	  var offDestroy = $scope.$on('$destroy', function() {
	    scope.$destroy();
	  });
	  scope.$on('$destroy', offDestroy);
	}])
	
	.directive('uibDropdown', function() {
	  return {
	    controller: 'UibDropdownController',
	    link: function(scope, element, attrs, dropdownCtrl) {
	      dropdownCtrl.init();
	    }
	  };
	})
	
	.directive('uibDropdownMenu', function() {
	  return {
	    restrict: 'AC',
	    require: '?^uibDropdown',
	    link: function(scope, element, attrs, dropdownCtrl) {
	      if (!dropdownCtrl || angular.isDefined(attrs.dropdownNested)) {
	        return;
	      }
	
	      element.addClass('dropdown-menu');
	
	      var tplUrl = attrs.templateUrl;
	      if (tplUrl) {
	        dropdownCtrl.dropdownMenuTemplateUrl = tplUrl;
	      }
	
	      if (!dropdownCtrl.dropdownMenu) {
	        dropdownCtrl.dropdownMenu = element;
	      }
	    }
	  };
	})
	
	.directive('uibKeyboardNav', function() {
	  return {
	    restrict: 'A',
	    require: '?^uibDropdown',
	    link: function(scope, element, attrs, dropdownCtrl) {
	      element.bind('keydown', function(e) {
	        if ([38, 40].indexOf(e.which) !== -1) {
	          e.preventDefault();
	          e.stopPropagation();
	
	          var elems = dropdownCtrl.dropdownMenu.find('a');
	
	          switch (e.which) {
	            case (40): { // Down
	              if (!angular.isNumber(dropdownCtrl.selectedOption)) {
	                dropdownCtrl.selectedOption = 0;
	              } else {
	                dropdownCtrl.selectedOption = dropdownCtrl.selectedOption === elems.length -1 ?
	                  dropdownCtrl.selectedOption : dropdownCtrl.selectedOption + 1;
	              }
	              break;
	            }
	            case (38): { // Up
	              if (!angular.isNumber(dropdownCtrl.selectedOption)) {
	                dropdownCtrl.selectedOption = elems.length - 1;
	              } else {
	                dropdownCtrl.selectedOption = dropdownCtrl.selectedOption === 0 ?
	                  0 : dropdownCtrl.selectedOption - 1;
	              }
	              break;
	            }
	          }
	          elems[dropdownCtrl.selectedOption].focus();
	        }
	      });
	    }
	  };
	})
	
	.directive('uibDropdownToggle', function() {
	  return {
	    require: '?^uibDropdown',
	    link: function(scope, element, attrs, dropdownCtrl) {
	      if (!dropdownCtrl) {
	        return;
	      }
	
	      element.addClass('dropdown-toggle');
	
	      dropdownCtrl.toggleElement = element;
	
	      var toggleDropdown = function(event) {
	        event.preventDefault();
	
	        if (!element.hasClass('disabled') && !attrs.disabled) {
	          scope.$apply(function() {
	            dropdownCtrl.toggle();
	          });
	        }
	      };
	
	      element.bind('click', toggleDropdown);
	
	      // WAI-ARIA
	      element.attr({ 'aria-haspopup': true, 'aria-expanded': false });
	      scope.$watch(dropdownCtrl.isOpen, function(isOpen) {
	        element.attr('aria-expanded', !!isOpen);
	      });
	
	      scope.$on('$destroy', function() {
	        element.unbind('click', toggleDropdown);
	      });
	    }
	  };
	});
	
	/* Deprecated dropdown below */
	
	angular.module('ui.bootstrap.dropdown')
	
	.value('$dropdownSuppressWarning', false)
	
	.service('dropdownService', ['$log', '$dropdownSuppressWarning', 'uibDropdownService', function($log, $dropdownSuppressWarning, uibDropdownService) {
	  if (!$dropdownSuppressWarning) {
	    $log.warn('dropdownService is now deprecated. Use uibDropdownService instead.');
	  }
	
	  angular.extend(this, uibDropdownService);
	}])
	
	.controller('DropdownController', ['$scope', '$element', '$attrs', '$parse', 'uibDropdownConfig', 'uibDropdownService', '$animate', '$uibPosition', '$document', '$compile', '$templateRequest', '$log', '$dropdownSuppressWarning', function($scope, $element, $attrs, $parse, dropdownConfig, uibDropdownService, $animate, $position, $document, $compile, $templateRequest, $log, $dropdownSuppressWarning) {
	  if (!$dropdownSuppressWarning) {
	    $log.warn('DropdownController is now deprecated. Use UibDropdownController instead.');
	  }
	
	  var self = this,
	    scope = $scope.$new(), // create a child scope so we are not polluting original one
	    templateScope,
	    openClass = dropdownConfig.openClass,
	    getIsOpen,
	    setIsOpen = angular.noop,
	    toggleInvoker = $attrs.onToggle ? $parse($attrs.onToggle) : angular.noop,
	    appendToBody = false,
	    keynavEnabled =false,
	    selectedOption = null;
	
	
	  $element.addClass('dropdown');
	
	  this.init = function() {
	    if ($attrs.isOpen) {
	      getIsOpen = $parse($attrs.isOpen);
	      setIsOpen = getIsOpen.assign;
	
	      $scope.$watch(getIsOpen, function(value) {
	        scope.isOpen = !!value;
	      });
	    }
	
	    appendToBody = angular.isDefined($attrs.dropdownAppendToBody);
	    keynavEnabled = angular.isDefined($attrs.uibKeyboardNav);
	
	    if (appendToBody && self.dropdownMenu) {
	      $document.find('body').append(self.dropdownMenu);
	      $element.on('$destroy', function handleDestroyEvent() {
	        self.dropdownMenu.remove();
	      });
	    }
	  };
	
	  this.toggle = function(open) {
	    return scope.isOpen = arguments.length ? !!open : !scope.isOpen;
	  };
	
	  // Allow other directives to watch status
	  this.isOpen = function() {
	    return scope.isOpen;
	  };
	
	  scope.getToggleElement = function() {
	    return self.toggleElement;
	  };
	
	  scope.getAutoClose = function() {
	    return $attrs.autoClose || 'always'; //or 'outsideClick' or 'disabled'
	  };
	
	  scope.getElement = function() {
	    return $element;
	  };
	
	  scope.isKeynavEnabled = function() {
	    return keynavEnabled;
	  };
	
	  scope.focusDropdownEntry = function(keyCode) {
	    var elems = self.dropdownMenu ? //If append to body is used.
	      (angular.element(self.dropdownMenu).find('a')) :
	      (angular.element($element).find('ul').eq(0).find('a'));
	
	    switch (keyCode) {
	      case (40): {
	        if (!angular.isNumber(self.selectedOption)) {
	          self.selectedOption = 0;
	        } else {
	          self.selectedOption = (self.selectedOption === elems.length -1 ?
	            self.selectedOption :
	          self.selectedOption + 1);
	        }
	        break;
	      }
	      case (38): {
	        if (!angular.isNumber(self.selectedOption)) {
	          self.selectedOption = elems.length - 1;
	        } else {
	          self.selectedOption = self.selectedOption === 0 ?
	            0 : self.selectedOption - 1;
	        }
	        break;
	      }
	    }
	    elems[self.selectedOption].focus();
	  };
	
	  scope.getDropdownElement = function() {
	    return self.dropdownMenu;
	  };
	
	  scope.focusToggleElement = function() {
	    if (self.toggleElement) {
	      self.toggleElement[0].focus();
	    }
	  };
	
	  scope.$watch('isOpen', function(isOpen, wasOpen) {
	    if (appendToBody && self.dropdownMenu) {
	      var pos = $position.positionElements($element, self.dropdownMenu, 'bottom-left', true);
	      var css = {
	        top: pos.top + 'px',
	        display: isOpen ? 'block' : 'none'
	      };
	
	      var rightalign = self.dropdownMenu.hasClass('dropdown-menu-right');
	      if (!rightalign) {
	        css.left = pos.left + 'px';
	        css.right = 'auto';
	      } else {
	        css.left = 'auto';
	        css.right = (window.innerWidth - (pos.left + $element.prop('offsetWidth'))) + 'px';
	      }
	
	      self.dropdownMenu.css(css);
	    }
	
	    $animate[isOpen ? 'addClass' : 'removeClass']($element, openClass).then(function() {
	      if (angular.isDefined(isOpen) && isOpen !== wasOpen) {
	        toggleInvoker($scope, { open: !!isOpen });
	      }
	    });
	
	    if (isOpen) {
	      if (self.dropdownMenuTemplateUrl) {
	        $templateRequest(self.dropdownMenuTemplateUrl).then(function(tplContent) {
	          templateScope = scope.$new();
	          $compile(tplContent.trim())(templateScope, function(dropdownElement) {
	            var newEl = dropdownElement;
	            self.dropdownMenu.replaceWith(newEl);
	            self.dropdownMenu = newEl;
	          });
	        });
	      }
	
	      scope.focusToggleElement();
	      uibDropdownService.open(scope);
	    } else {
	      if (self.dropdownMenuTemplateUrl) {
	        if (templateScope) {
	          templateScope.$destroy();
	        }
	        var newEl = angular.element('<ul class="dropdown-menu"></ul>');
	        self.dropdownMenu.replaceWith(newEl);
	        self.dropdownMenu = newEl;
	      }
	
	      uibDropdownService.close(scope);
	      self.selectedOption = null;
	    }
	
	    if (angular.isFunction(setIsOpen)) {
	      setIsOpen($scope, isOpen);
	    }
	  });
	
	  $scope.$on('$locationChangeSuccess', function() {
	    if (scope.getAutoClose() !== 'disabled') {
	      scope.isOpen = false;
	    }
	  });
	
	  var offDestroy = $scope.$on('$destroy', function() {
	    scope.$destroy();
	  });
	  scope.$on('$destroy', offDestroy);
	}])
	
	.directive('dropdown', ['$log', '$dropdownSuppressWarning', function($log, $dropdownSuppressWarning) {
	  return {
	    controller: 'DropdownController',
	    link: function(scope, element, attrs, dropdownCtrl) {
	      if (!$dropdownSuppressWarning) {
	        $log.warn('dropdown is now deprecated. Use uib-dropdown instead.');
	      }
	
	      dropdownCtrl.init();
	    }
	  };
	}])
	
	.directive('dropdownMenu', ['$log', '$dropdownSuppressWarning', function($log, $dropdownSuppressWarning) {
	  return {
	    restrict: 'AC',
	    require: '?^dropdown',
	    link: function(scope, element, attrs, dropdownCtrl) {
	      if (!dropdownCtrl || angular.isDefined(attrs.dropdownNested)) {
	        return;
	      }
	
	      if (!$dropdownSuppressWarning) {
	        $log.warn('dropdown-menu is now deprecated. Use uib-dropdown-menu instead.');
	      }
	
	      element.addClass('dropdown-menu');
	
	      var tplUrl = attrs.templateUrl;
	      if (tplUrl) {
	        dropdownCtrl.dropdownMenuTemplateUrl = tplUrl;
	      }
	
	      if (!dropdownCtrl.dropdownMenu) {
	        dropdownCtrl.dropdownMenu = element;
	      }
	    }
	  };
	}])
	
	.directive('keyboardNav', ['$log', '$dropdownSuppressWarning', function($log, $dropdownSuppressWarning) {
	  return {
	    restrict: 'A',
	    require: '?^dropdown',
	    link: function(scope, element, attrs, dropdownCtrl) {
	      if (!$dropdownSuppressWarning) {
	        $log.warn('keyboard-nav is now deprecated. Use uib-keyboard-nav instead.');
	      }
	
	      element.bind('keydown', function(e) {
	        if ([38, 40].indexOf(e.which) !== -1) {
	          e.preventDefault();
	          e.stopPropagation();
	
	          var elems = dropdownCtrl.dropdownMenu.find('a');
	
	          switch (e.which) {
	            case (40): { // Down
	              if (!angular.isNumber(dropdownCtrl.selectedOption)) {
	                dropdownCtrl.selectedOption = 0;
	              } else {
	                dropdownCtrl.selectedOption = dropdownCtrl.selectedOption === elems.length -1 ?
	                  dropdownCtrl.selectedOption : dropdownCtrl.selectedOption + 1;
	              }
	              break;
	            }
	            case (38): { // Up
	              if (!angular.isNumber(dropdownCtrl.selectedOption)) {
	                dropdownCtrl.selectedOption = elems.length - 1;
	              } else {
	                dropdownCtrl.selectedOption = dropdownCtrl.selectedOption === 0 ?
	                  0 : dropdownCtrl.selectedOption - 1;
	              }
	              break;
	            }
	          }
	          elems[dropdownCtrl.selectedOption].focus();
	        }
	      });
	    }
	  };
	}])
	
	.directive('dropdownToggle', ['$log', '$dropdownSuppressWarning', function($log, $dropdownSuppressWarning) {
	  return {
	    require: '?^dropdown',
	    link: function(scope, element, attrs, dropdownCtrl) {
	      if (!$dropdownSuppressWarning) {
	        $log.warn('dropdown-toggle is now deprecated. Use uib-dropdown-toggle instead.');
	      }
	
	      if (!dropdownCtrl) {
	        return;
	      }
	
	      element.addClass('dropdown-toggle');
	
	      dropdownCtrl.toggleElement = element;
	
	      var toggleDropdown = function(event) {
	        event.preventDefault();
	
	        if (!element.hasClass('disabled') && !attrs.disabled) {
	          scope.$apply(function() {
	            dropdownCtrl.toggle();
	          });
	        }
	      };
	
	      element.bind('click', toggleDropdown);
	
	      // WAI-ARIA
	      element.attr({ 'aria-haspopup': true, 'aria-expanded': false });
	      scope.$watch(dropdownCtrl.isOpen, function(isOpen) {
	        element.attr('aria-expanded', !!isOpen);
	      });
	
	      scope.$on('$destroy', function() {
	        element.unbind('click', toggleDropdown);
	      });
	    }
	  };
	}]);
	
	angular.module('ui.bootstrap.stackedMap', [])
	/**
	 * A helper, internal data structure that acts as a map but also allows getting / removing
	 * elements in the LIFO order
	 */
	  .factory('$$stackedMap', function() {
	    return {
	      createNew: function() {
	        var stack = [];
	
	        return {
	          add: function(key, value) {
	            stack.push({
	              key: key,
	              value: value
	            });
	          },
	          get: function(key) {
	            for (var i = 0; i < stack.length; i++) {
	              if (key == stack[i].key) {
	                return stack[i];
	              }
	            }
	          },
	          keys: function() {
	            var keys = [];
	            for (var i = 0; i < stack.length; i++) {
	              keys.push(stack[i].key);
	            }
	            return keys;
	          },
	          top: function() {
	            return stack[stack.length - 1];
	          },
	          remove: function(key) {
	            var idx = -1;
	            for (var i = 0; i < stack.length; i++) {
	              if (key == stack[i].key) {
	                idx = i;
	                break;
	              }
	            }
	            return stack.splice(idx, 1)[0];
	          },
	          removeTop: function() {
	            return stack.splice(stack.length - 1, 1)[0];
	          },
	          length: function() {
	            return stack.length;
	          }
	        };
	      }
	    };
	  });
	angular.module('ui.bootstrap.modal', ['ui.bootstrap.stackedMap'])
	/**
	 * A helper, internal data structure that stores all references attached to key
	 */
	  .factory('$$multiMap', function() {
	    return {
	      createNew: function() {
	        var map = {};
	
	        return {
	          entries: function() {
	            return Object.keys(map).map(function(key) {
	              return {
	                key: key,
	                value: map[key]
	              };
	            });
	          },
	          get: function(key) {
	            return map[key];
	          },
	          hasKey: function(key) {
	            return !!map[key];
	          },
	          keys: function() {
	            return Object.keys(map);
	          },
	          put: function(key, value) {
	            if (!map[key]) {
	              map[key] = [];
	            }
	
	            map[key].push(value);
	          },
	          remove: function(key, value) {
	            var values = map[key];
	
	            if (!values) {
	              return;
	            }
	
	            var idx = values.indexOf(value);
	
	            if (idx !== -1) {
	              values.splice(idx, 1);
	            }
	
	            if (!values.length) {
	              delete map[key];
	            }
	          }
	        };
	      }
	    };
	  })
	
	/**
	 * A helper directive for the $modal service. It creates a backdrop element.
	 */
	  .directive('uibModalBackdrop', [
	           '$animate', '$injector', '$uibModalStack',
	  function($animate ,  $injector,   $modalStack) {
	    var $animateCss = null;
	
	    if ($injector.has('$animateCss')) {
	      $animateCss = $injector.get('$animateCss');
	    }
	
	    return {
	      replace: true,
	      templateUrl: 'template/modal/backdrop.html',
	      compile: function(tElement, tAttrs) {
	        tElement.addClass(tAttrs.backdropClass);
	        return linkFn;
	      }
	    };
	
	    function linkFn(scope, element, attrs) {
	      // Temporary fix for prefixing
	      element.addClass('modal-backdrop');
	
	      if (attrs.modalInClass) {
	        if ($animateCss) {
	          $animateCss(element, {
	            addClass: attrs.modalInClass
	          }).start();
	        } else {
	          $animate.addClass(element, attrs.modalInClass);
	        }
	
	        scope.$on($modalStack.NOW_CLOSING_EVENT, function(e, setIsAsync) {
	          var done = setIsAsync();
	          if ($animateCss) {
	            $animateCss(element, {
	              removeClass: attrs.modalInClass
	            }).start().then(done);
	          } else {
	            $animate.removeClass(element, attrs.modalInClass).then(done);
	          }
	        });
	      }
	    }
	  }])
	
	  .directive('uibModalWindow', [
	           '$uibModalStack', '$q', '$animate', '$injector',
	  function($modalStack ,  $q ,  $animate,   $injector) {
	    var $animateCss = null;
	
	    if ($injector.has('$animateCss')) {
	      $animateCss = $injector.get('$animateCss');
	    }
	
	    return {
	      scope: {
	        index: '@'
	      },
	      replace: true,
	      transclude: true,
	      templateUrl: function(tElement, tAttrs) {
	        return tAttrs.templateUrl || 'template/modal/window.html';
	      },
	      link: function(scope, element, attrs) {
	        element.addClass(attrs.windowClass || '');
	        element.addClass(attrs.windowTopClass || '');
	        scope.size = attrs.size;
	
	        scope.close = function(evt) {
	          var modal = $modalStack.getTop();
	          if (modal && modal.value.backdrop && modal.value.backdrop !== 'static' && (evt.target === evt.currentTarget)) {
	            evt.preventDefault();
	            evt.stopPropagation();
	            $modalStack.dismiss(modal.key, 'backdrop click');
	          }
	        };
	
	        // moved from template to fix issue #2280
	        element.on('click', scope.close);
	
	        // This property is only added to the scope for the purpose of detecting when this directive is rendered.
	        // We can detect that by using this property in the template associated with this directive and then use
	        // {@link Attribute#$observe} on it. For more details please see {@link TableColumnResize}.
	        scope.$isRendered = true;
	
	        // Deferred object that will be resolved when this modal is render.
	        var modalRenderDeferObj = $q.defer();
	        // Observe function will be called on next digest cycle after compilation, ensuring that the DOM is ready.
	        // In order to use this way of finding whether DOM is ready, we need to observe a scope property used in modal's template.
	        attrs.$observe('modalRender', function(value) {
	          if (value == 'true') {
	            modalRenderDeferObj.resolve();
	          }
	        });
	
	        modalRenderDeferObj.promise.then(function() {
	          var animationPromise = null;
	
	          if (attrs.modalInClass) {
	            if ($animateCss) {
	              animationPromise = $animateCss(element, {
	                addClass: attrs.modalInClass
	              }).start();
	            } else {
	              animationPromise = $animate.addClass(element, attrs.modalInClass);
	            }
	
	            scope.$on($modalStack.NOW_CLOSING_EVENT, function(e, setIsAsync) {
	              var done = setIsAsync();
	              if ($animateCss) {
	                $animateCss(element, {
	                  removeClass: attrs.modalInClass
	                }).start().then(done);
	              } else {
	                $animate.removeClass(element, attrs.modalInClass).then(done);
	              }
	            });
	          }
	
	
	          $q.when(animationPromise).then(function() {
	            var inputWithAutofocus = element[0].querySelector('[autofocus]');
	            /**
	             * Auto-focusing of a freshly-opened modal element causes any child elements
	             * with the autofocus attribute to lose focus. This is an issue on touch
	             * based devices which will show and then hide the onscreen keyboard.
	             * Attempts to refocus the autofocus element via JavaScript will not reopen
	             * the onscreen keyboard. Fixed by updated the focusing logic to only autofocus
	             * the modal element if the modal does not contain an autofocus element.
	             */
	            if (inputWithAutofocus) {
	              inputWithAutofocus.focus();
	            } else {
	              element[0].focus();
	            }
	          });
	
	          // Notify {@link $modalStack} that modal is rendered.
	          var modal = $modalStack.getTop();
	          if (modal) {
	            $modalStack.modalRendered(modal.key);
	          }
	        });
	      }
	    };
	  }])
	
	  .directive('uibModalAnimationClass', function() {
	    return {
	      compile: function(tElement, tAttrs) {
	        if (tAttrs.modalAnimation) {
	          tElement.addClass(tAttrs.uibModalAnimationClass);
	        }
	      }
	    };
	  })
	
	  .directive('uibModalTransclude', function() {
	    return {
	      link: function($scope, $element, $attrs, controller, $transclude) {
	        $transclude($scope.$parent, function(clone) {
	          $element.empty();
	          $element.append(clone);
	        });
	      }
	    };
	  })
	
	  .factory('$uibModalStack', [
	             '$animate', '$timeout', '$document', '$compile', '$rootScope',
	             '$q',
	             '$injector',
	             '$$multiMap',
	             '$$stackedMap',
	    function($animate ,  $timeout ,  $document ,  $compile ,  $rootScope ,
	              $q,
	              $injector,
	              $$multiMap,
	              $$stackedMap) {
	      var $animateCss = null;
	
	      if ($injector.has('$animateCss')) {
	        $animateCss = $injector.get('$animateCss');
	      }
	
	      var OPENED_MODAL_CLASS = 'modal-open';
	
	      var backdropDomEl, backdropScope;
	      var openedWindows = $$stackedMap.createNew();
	      var openedClasses = $$multiMap.createNew();
	      var $modalStack = {
	        NOW_CLOSING_EVENT: 'modal.stack.now-closing'
	      };
	
	      //Modal focus behavior
	      var focusableElementList;
	      var focusIndex = 0;
	      var tababbleSelector = 'a[href], area[href], input:not([disabled]), ' +
	        'button:not([disabled]),select:not([disabled]), textarea:not([disabled]), ' +
	        'iframe, object, embed, *[tabindex], *[contenteditable=true]';
	
	      function backdropIndex() {
	        var topBackdropIndex = -1;
	        var opened = openedWindows.keys();
	        for (var i = 0; i < opened.length; i++) {
	          if (openedWindows.get(opened[i]).value.backdrop) {
	            topBackdropIndex = i;
	          }
	        }
	        return topBackdropIndex;
	      }
	
	      $rootScope.$watch(backdropIndex, function(newBackdropIndex) {
	        if (backdropScope) {
	          backdropScope.index = newBackdropIndex;
	        }
	      });
	
	      function removeModalWindow(modalInstance, elementToReceiveFocus) {
	        var body = $document.find('body').eq(0);
	        var modalWindow = openedWindows.get(modalInstance).value;
	
	        //clean up the stack
	        openedWindows.remove(modalInstance);
	
	        removeAfterAnimate(modalWindow.modalDomEl, modalWindow.modalScope, function() {
	          var modalBodyClass = modalWindow.openedClass || OPENED_MODAL_CLASS;
	          openedClasses.remove(modalBodyClass, modalInstance);
	          body.toggleClass(modalBodyClass, openedClasses.hasKey(modalBodyClass));
	          toggleTopWindowClass(true);
	        });
	        checkRemoveBackdrop();
	
	        //move focus to specified element if available, or else to body
	        if (elementToReceiveFocus && elementToReceiveFocus.focus) {
	          elementToReceiveFocus.focus();
	        } else {
	          body.focus();
	        }
	      }
	
	      // Add or remove "windowTopClass" from the top window in the stack
	      function toggleTopWindowClass(toggleSwitch) {
	        var modalWindow;
	
	        if (openedWindows.length() > 0) {
	          modalWindow = openedWindows.top().value;
	          modalWindow.modalDomEl.toggleClass(modalWindow.windowTopClass || '', toggleSwitch);
	        }
	      }
	
	      function checkRemoveBackdrop() {
	        //remove backdrop if no longer needed
	        if (backdropDomEl && backdropIndex() == -1) {
	          var backdropScopeRef = backdropScope;
	          removeAfterAnimate(backdropDomEl, backdropScope, function() {
	            backdropScopeRef = null;
	          });
	          backdropDomEl = undefined;
	          backdropScope = undefined;
	        }
	      }
	
	      function removeAfterAnimate(domEl, scope, done) {
	        var asyncDeferred;
	        var asyncPromise = null;
	        var setIsAsync = function() {
	          if (!asyncDeferred) {
	            asyncDeferred = $q.defer();
	            asyncPromise = asyncDeferred.promise;
	          }
	
	          return function asyncDone() {
	            asyncDeferred.resolve();
	          };
	        };
	        scope.$broadcast($modalStack.NOW_CLOSING_EVENT, setIsAsync);
	
	        // Note that it's intentional that asyncPromise might be null.
	        // That's when setIsAsync has not been called during the
	        // NOW_CLOSING_EVENT broadcast.
	        return $q.when(asyncPromise).then(afterAnimating);
	
	        function afterAnimating() {
	          if (afterAnimating.done) {
	            return;
	          }
	          afterAnimating.done = true;
	
	          if ($animateCss) {
	            $animateCss(domEl, {
	              event: 'leave'
	            }).start().then(function() {
	              domEl.remove();
	            });
	          } else {
	            $animate.leave(domEl);
	          }
	          scope.$destroy();
	          if (done) {
	            done();
	          }
	        }
	      }
	
	      $document.bind('keydown', function(evt) {
	        if (evt.isDefaultPrevented()) {
	          return evt;
	        }
	
	        var modal = openedWindows.top();
	        if (modal && modal.value.keyboard) {
	          switch (evt.which) {
	            case 27: {
	              evt.preventDefault();
	              $rootScope.$apply(function() {
	                $modalStack.dismiss(modal.key, 'escape key press');
	              });
	              break;
	            }
	            case 9: {
	              $modalStack.loadFocusElementList(modal);
	              var focusChanged = false;
	              if (evt.shiftKey) {
	                if ($modalStack.isFocusInFirstItem(evt)) {
	                  focusChanged = $modalStack.focusLastFocusableElement();
	                }
	              } else {
	                if ($modalStack.isFocusInLastItem(evt)) {
	                  focusChanged = $modalStack.focusFirstFocusableElement();
	                }
	              }
	
	              if (focusChanged) {
	                evt.preventDefault();
	                evt.stopPropagation();
	              }
	              break;
	            }
	          }
	        }
	      });
	
	      $modalStack.open = function(modalInstance, modal) {
	        var modalOpener = $document[0].activeElement,
	          modalBodyClass = modal.openedClass || OPENED_MODAL_CLASS;
	
	        toggleTopWindowClass(false);
	
	        openedWindows.add(modalInstance, {
	          deferred: modal.deferred,
	          renderDeferred: modal.renderDeferred,
	          modalScope: modal.scope,
	          backdrop: modal.backdrop,
	          keyboard: modal.keyboard,
	          openedClass: modal.openedClass,
	          windowTopClass: modal.windowTopClass
	        });
	
	        openedClasses.put(modalBodyClass, modalInstance);
	
	        var body = $document.find('body').eq(0),
	            currBackdropIndex = backdropIndex();
	
	        if (currBackdropIndex >= 0 && !backdropDomEl) {
	          backdropScope = $rootScope.$new(true);
	          backdropScope.index = currBackdropIndex;
	          var angularBackgroundDomEl = angular.element('<div uib-modal-backdrop="modal-backdrop"></div>');
	          angularBackgroundDomEl.attr('backdrop-class', modal.backdropClass);
	          if (modal.animation) {
	            angularBackgroundDomEl.attr('modal-animation', 'true');
	          }
	          backdropDomEl = $compile(angularBackgroundDomEl)(backdropScope);
	          body.append(backdropDomEl);
	        }
	
	        var angularDomEl = angular.element('<div uib-modal-window="modal-window"></div>');
	        angularDomEl.attr({
	          'template-url': modal.windowTemplateUrl,
	          'window-class': modal.windowClass,
	          'window-top-class': modal.windowTopClass,
	          'size': modal.size,
	          'index': openedWindows.length() - 1,
	          'animate': 'animate'
	        }).html(modal.content);
	        if (modal.animation) {
	          angularDomEl.attr('modal-animation', 'true');
	        }
	
	        var modalDomEl = $compile(angularDomEl)(modal.scope);
	        openedWindows.top().value.modalDomEl = modalDomEl;
	        openedWindows.top().value.modalOpener = modalOpener;
	        body.append(modalDomEl);
	        body.addClass(modalBodyClass);
	
	        $modalStack.clearFocusListCache();
	      };
	
	      function broadcastClosing(modalWindow, resultOrReason, closing) {
	        return !modalWindow.value.modalScope.$broadcast('modal.closing', resultOrReason, closing).defaultPrevented;
	      }
	
	      $modalStack.close = function(modalInstance, result) {
	        var modalWindow = openedWindows.get(modalInstance);
	        if (modalWindow && broadcastClosing(modalWindow, result, true)) {
	          modalWindow.value.modalScope.$$uibDestructionScheduled = true;
	          modalWindow.value.deferred.resolve(result);
	          removeModalWindow(modalInstance, modalWindow.value.modalOpener);
	          return true;
	        }
	        return !modalWindow;
	      };
	
	      $modalStack.dismiss = function(modalInstance, reason) {
	        var modalWindow = openedWindows.get(modalInstance);
	        if (modalWindow && broadcastClosing(modalWindow, reason, false)) {
	          modalWindow.value.modalScope.$$uibDestructionScheduled = true;
	          modalWindow.value.deferred.reject(reason);
	          removeModalWindow(modalInstance, modalWindow.value.modalOpener);
	          return true;
	        }
	        return !modalWindow;
	      };
	
	      $modalStack.dismissAll = function(reason) {
	        var topModal = this.getTop();
	        while (topModal && this.dismiss(topModal.key, reason)) {
	          topModal = this.getTop();
	        }
	      };
	
	      $modalStack.getTop = function() {
	        return openedWindows.top();
	      };
	
	      $modalStack.modalRendered = function(modalInstance) {
	        var modalWindow = openedWindows.get(modalInstance);
	        if (modalWindow) {
	          modalWindow.value.renderDeferred.resolve();
	        }
	      };
	
	      $modalStack.focusFirstFocusableElement = function() {
	        if (focusableElementList.length > 0) {
	          focusableElementList[0].focus();
	          return true;
	        }
	        return false;
	      };
	      $modalStack.focusLastFocusableElement = function() {
	        if (focusableElementList.length > 0) {
	          focusableElementList[focusableElementList.length - 1].focus();
	          return true;
	        }
	        return false;
	      };
	
	      $modalStack.isFocusInFirstItem = function(evt) {
	        if (focusableElementList.length > 0) {
	          return (evt.target || evt.srcElement) == focusableElementList[0];
	        }
	        return false;
	      };
	
	      $modalStack.isFocusInLastItem = function(evt) {
	        if (focusableElementList.length > 0) {
	          return (evt.target || evt.srcElement) == focusableElementList[focusableElementList.length - 1];
	        }
	        return false;
	      };
	
	      $modalStack.clearFocusListCache = function() {
	        focusableElementList = [];
	        focusIndex = 0;
	      };
	
	      $modalStack.loadFocusElementList = function(modalWindow) {
	        if (focusableElementList === undefined || !focusableElementList.length) {
	          if (modalWindow) {
	            var modalDomE1 = modalWindow.value.modalDomEl;
	            if (modalDomE1 && modalDomE1.length) {
	              focusableElementList = modalDomE1[0].querySelectorAll(tababbleSelector);
	            }
	          }
	        }
	      };
	
	      return $modalStack;
	    }])
	
	  .provider('$uibModal', function() {
	    var $modalProvider = {
	      options: {
	        animation: true,
	        backdrop: true, //can also be false or 'static'
	        keyboard: true
	      },
	      $get: ['$injector', '$rootScope', '$q', '$templateRequest', '$controller', '$uibModalStack', '$modalSuppressWarning', '$log',
	        function ($injector, $rootScope, $q, $templateRequest, $controller, $modalStack, $modalSuppressWarning, $log) {
	          var $modal = {};
	
	          function getTemplatePromise(options) {
	            return options.template ? $q.when(options.template) :
	              $templateRequest(angular.isFunction(options.templateUrl) ? (options.templateUrl)() : options.templateUrl);
	          }
	
	          function getResolvePromises(resolves) {
	            var promisesArr = [];
	            angular.forEach(resolves, function(value) {
	              if (angular.isFunction(value) || angular.isArray(value)) {
	                promisesArr.push($q.when($injector.invoke(value)));
	              } else if (angular.isString(value)) {
	                promisesArr.push($q.when($injector.get(value)));
	              } else {
	                promisesArr.push($q.when(value));
	              }
	            });
	            return promisesArr;
	          }
	
	          var promiseChain = null;
	          $modal.getPromiseChain = function() {
	            return promiseChain;
	          };
	
	          $modal.open = function(modalOptions) {
	            var modalResultDeferred = $q.defer();
	            var modalOpenedDeferred = $q.defer();
	            var modalRenderDeferred = $q.defer();
	
	            //prepare an instance of a modal to be injected into controllers and returned to a caller
	            var modalInstance = {
	              result: modalResultDeferred.promise,
	              opened: modalOpenedDeferred.promise,
	              rendered: modalRenderDeferred.promise,
	              close: function (result) {
	                return $modalStack.close(modalInstance, result);
	              },
	              dismiss: function (reason) {
	                return $modalStack.dismiss(modalInstance, reason);
	              }
	            };
	
	            //merge and clean up options
	            modalOptions = angular.extend({}, $modalProvider.options, modalOptions);
	            modalOptions.resolve = modalOptions.resolve || {};
	
	            //verify options
	            if (!modalOptions.template && !modalOptions.templateUrl) {
	              throw new Error('One of template or templateUrl options is required.');
	            }
	
	            var templateAndResolvePromise =
	              $q.all([getTemplatePromise(modalOptions)].concat(getResolvePromises(modalOptions.resolve)));
	
	            function resolveWithTemplate() {
	              return templateAndResolvePromise;
	            }
	
	            // Wait for the resolution of the existing promise chain.
	            // Then switch to our own combined promise dependency (regardless of how the previous modal fared).
	            // Then add to $modalStack and resolve opened.
	            // Finally clean up the chain variable if no subsequent modal has overwritten it.
	            var samePromise;
	            samePromise = promiseChain = $q.all([promiseChain])
	              .then(resolveWithTemplate, resolveWithTemplate)
	              .then(function resolveSuccess(tplAndVars) {
	
	                var modalScope = (modalOptions.scope || $rootScope).$new();
	                modalScope.$close = modalInstance.close;
	                modalScope.$dismiss = modalInstance.dismiss;
	
	                modalScope.$on('$destroy', function() {
	                  if (!modalScope.$$uibDestructionScheduled) {
	                    modalScope.$dismiss('$uibUnscheduledDestruction');
	                  }
	                });
	
	                var ctrlInstance, ctrlLocals = {};
	                var resolveIter = 1;
	
	                //controllers
	                if (modalOptions.controller) {
	                  ctrlLocals.$scope = modalScope;
	                  ctrlLocals.$uibModalInstance = modalInstance;
	                  Object.defineProperty(ctrlLocals, '$modalInstance', {
	                    get: function() {
	                      if (!$modalSuppressWarning) {
	                        $log.warn('$modalInstance is now deprecated. Use $uibModalInstance instead.');
	                      }
	
	                      return modalInstance;
	                    }
	                  });
	                  angular.forEach(modalOptions.resolve, function(value, key) {
	                    ctrlLocals[key] = tplAndVars[resolveIter++];
	                  });
	
	                  ctrlInstance = $controller(modalOptions.controller, ctrlLocals);
	                  if (modalOptions.controllerAs) {
	                    if (modalOptions.bindToController) {
	                      angular.extend(ctrlInstance, modalScope);
	                    }
	
	                    modalScope[modalOptions.controllerAs] = ctrlInstance;
	                  }
	                }
	
	                $modalStack.open(modalInstance, {
	                  scope: modalScope,
	                  deferred: modalResultDeferred,
	                  renderDeferred: modalRenderDeferred,
	                  content: tplAndVars[0],
	                  animation: modalOptions.animation,
	                  backdrop: modalOptions.backdrop,
	                  keyboard: modalOptions.keyboard,
	                  backdropClass: modalOptions.backdropClass,
	                  windowTopClass: modalOptions.windowTopClass,
	                  windowClass: modalOptions.windowClass,
	                  windowTemplateUrl: modalOptions.windowTemplateUrl,
	                  size: modalOptions.size,
	                  openedClass: modalOptions.openedClass
	                });
	                modalOpenedDeferred.resolve(true);
	
	            }, function resolveError(reason) {
	              modalOpenedDeferred.reject(reason);
	              modalResultDeferred.reject(reason);
	            })
	            .finally(function() {
	              if (promiseChain === samePromise) {
	                promiseChain = null;
	              }
	            });
	
	            return modalInstance;
	          };
	
	          return $modal;
	        }
	      ]
	    };
	
	    return $modalProvider;
	  });
	
	/* deprecated modal below */
	
	angular.module('ui.bootstrap.modal')
	
	  .value('$modalSuppressWarning', false)
	
	  /**
	   * A helper directive for the $modal service. It creates a backdrop element.
	   */
	  .directive('modalBackdrop', [
	    '$animate', '$injector', '$modalStack', '$log', '$modalSuppressWarning',
	    function($animate ,  $injector,   $modalStack, $log, $modalSuppressWarning) {
	      var $animateCss = null;
	
	      if ($injector.has('$animateCss')) {
	        $animateCss = $injector.get('$animateCss');
	      }
	
	      return {
	        replace: true,
	        templateUrl: 'template/modal/backdrop.html',
	        compile: function(tElement, tAttrs) {
	          tElement.addClass(tAttrs.backdropClass);
	          return linkFn;
	        }
	      };
	
	      function linkFn(scope, element, attrs) {
	        if (!$modalSuppressWarning) {
	          $log.warn('modal-backdrop is now deprecated. Use uib-modal-backdrop instead.');
	        }
	        element.addClass('modal-backdrop');
	
	        if (attrs.modalInClass) {
	          if ($animateCss) {
	            $animateCss(element, {
	              addClass: attrs.modalInClass
	            }).start();
	          } else {
	            $animate.addClass(element, attrs.modalInClass);
	          }
	
	          scope.$on($modalStack.NOW_CLOSING_EVENT, function(e, setIsAsync) {
	            var done = setIsAsync();
	            if ($animateCss) {
	              $animateCss(element, {
	                removeClass: attrs.modalInClass
	              }).start().then(done);
	            } else {
	              $animate.removeClass(element, attrs.modalInClass).then(done);
	            }
	          });
	        }
	      }
	    }])
	
	  .directive('modalWindow', [
	    '$modalStack', '$q', '$animate', '$injector', '$log', '$modalSuppressWarning',
	    function($modalStack ,  $q ,  $animate,   $injector, $log, $modalSuppressWarning) {
	      var $animateCss = null;
	
	      if ($injector.has('$animateCss')) {
	        $animateCss = $injector.get('$animateCss');
	      }
	
	      return {
	        scope: {
	          index: '@'
	        },
	        replace: true,
	        transclude: true,
	        templateUrl: function(tElement, tAttrs) {
	          return tAttrs.templateUrl || 'template/modal/window.html';
	        },
	        link: function(scope, element, attrs) {
	          if (!$modalSuppressWarning) {
	            $log.warn('modal-window is now deprecated. Use uib-modal-window instead.');
	          }
	          element.addClass(attrs.windowClass || '');
	          element.addClass(attrs.windowTopClass || '');
	          scope.size = attrs.size;
	
	          scope.close = function(evt) {
	            var modal = $modalStack.getTop();
	            if (modal && modal.value.backdrop && modal.value.backdrop !== 'static' && (evt.target === evt.currentTarget)) {
	              evt.preventDefault();
	              evt.stopPropagation();
	              $modalStack.dismiss(modal.key, 'backdrop click');
	            }
	          };
	
	          // moved from template to fix issue #2280
	          element.on('click', scope.close);
	
	          // This property is only added to the scope for the purpose of detecting when this directive is rendered.
	          // We can detect that by using this property in the template associated with this directive and then use
	          // {@link Attribute#$observe} on it. For more details please see {@link TableColumnResize}.
	          scope.$isRendered = true;
	
	          // Deferred object that will be resolved when this modal is render.
	          var modalRenderDeferObj = $q.defer();
	          // Observe function will be called on next digest cycle after compilation, ensuring that the DOM is ready.
	          // In order to use this way of finding whether DOM is ready, we need to observe a scope property used in modal's template.
	          attrs.$observe('modalRender', function(value) {
	            if (value == 'true') {
	              modalRenderDeferObj.resolve();
	            }
	          });
	
	          modalRenderDeferObj.promise.then(function() {
	            var animationPromise = null;
	
	            if (attrs.modalInClass) {
	              if ($animateCss) {
	                animationPromise = $animateCss(element, {
	                  addClass: attrs.modalInClass
	                }).start();
	              } else {
	                animationPromise = $animate.addClass(element, attrs.modalInClass);
	              }
	
	              scope.$on($modalStack.NOW_CLOSING_EVENT, function(e, setIsAsync) {
	                var done = setIsAsync();
	                if ($animateCss) {
	                  $animateCss(element, {
	                    removeClass: attrs.modalInClass
	                  }).start().then(done);
	                } else {
	                  $animate.removeClass(element, attrs.modalInClass).then(done);
	                }
	              });
	            }
	
	
	            $q.when(animationPromise).then(function() {
	              var inputWithAutofocus = element[0].querySelector('[autofocus]');
	              /**
	               * Auto-focusing of a freshly-opened modal element causes any child elements
	               * with the autofocus attribute to lose focus. This is an issue on touch
	               * based devices which will show and then hide the onscreen keyboard.
	               * Attempts to refocus the autofocus element via JavaScript will not reopen
	               * the onscreen keyboard. Fixed by updated the focusing logic to only autofocus
	               * the modal element if the modal does not contain an autofocus element.
	               */
	              if (inputWithAutofocus) {
	                inputWithAutofocus.focus();
	              } else {
	                element[0].focus();
	              }
	            });
	
	            // Notify {@link $modalStack} that modal is rendered.
	            var modal = $modalStack.getTop();
	            if (modal) {
	              $modalStack.modalRendered(modal.key);
	            }
	          });
	        }
	      };
	    }])
	
	  .directive('modalAnimationClass', [
	    '$log', '$modalSuppressWarning',
	    function ($log, $modalSuppressWarning) {
	      return {
	        compile: function(tElement, tAttrs) {
	          if (!$modalSuppressWarning) {
	            $log.warn('modal-animation-class is now deprecated. Use uib-modal-animation-class instead.');
	          }
	          if (tAttrs.modalAnimation) {
	            tElement.addClass(tAttrs.modalAnimationClass);
	          }
	        }
	      };
	    }])
	
	  .directive('modalTransclude', [
	    '$log', '$modalSuppressWarning',
	    function ($log, $modalSuppressWarning) {
	    return {
	      link: function($scope, $element, $attrs, controller, $transclude) {
	        if (!$modalSuppressWarning) {
	          $log.warn('modal-transclude is now deprecated. Use uib-modal-transclude instead.');
	        }
	        $transclude($scope.$parent, function(clone) {
	          $element.empty();
	          $element.append(clone);
	        });
	      }
	    };
	  }])
	
	  .service('$modalStack', [
	    '$animate', '$timeout', '$document', '$compile', '$rootScope',
	    '$q',
	    '$injector',
	    '$$multiMap',
	    '$$stackedMap',
	    '$uibModalStack',
	    '$log',
	    '$modalSuppressWarning',
	    function($animate ,  $timeout ,  $document ,  $compile ,  $rootScope ,
	             $q,
	             $injector,
	             $$multiMap,
	             $$stackedMap,
	             $uibModalStack,
	             $log,
	             $modalSuppressWarning) {
	      if (!$modalSuppressWarning) {
	        $log.warn('$modalStack is now deprecated. Use $uibModalStack instead.');
	      }
	
	      angular.extend(this, $uibModalStack);
	    }])
	
	  .provider('$modal', ['$uibModalProvider', function($uibModalProvider) {
	    angular.extend(this, $uibModalProvider);
	
	    this.$get = ['$injector', '$log', '$modalSuppressWarning',
	      function ($injector, $log, $modalSuppressWarning) {
	        if (!$modalSuppressWarning) {
	          $log.warn('$modal is now deprecated. Use $uibModal instead.');
	        }
	
	        return $injector.invoke($uibModalProvider.$get);
	      }];
	  }]);
	
	angular.module('ui.bootstrap.pagination', [])
	.controller('UibPaginationController', ['$scope', '$attrs', '$parse', function($scope, $attrs, $parse) {
	  var self = this,
	      ngModelCtrl = { $setViewValue: angular.noop }, // nullModelCtrl
	      setNumPages = $attrs.numPages ? $parse($attrs.numPages).assign : angular.noop;
	
	  this.init = function(ngModelCtrl_, config) {
	    ngModelCtrl = ngModelCtrl_;
	    this.config = config;
	
	    ngModelCtrl.$render = function() {
	      self.render();
	    };
	
	    if ($attrs.itemsPerPage) {
	      $scope.$parent.$watch($parse($attrs.itemsPerPage), function(value) {
	        self.itemsPerPage = parseInt(value, 10);
	        $scope.totalPages = self.calculateTotalPages();
	      });
	    } else {
	      this.itemsPerPage = config.itemsPerPage;
	    }
	
	    $scope.$watch('totalItems', function() {
	      $scope.totalPages = self.calculateTotalPages();
	    });
	
	    $scope.$watch('totalPages', function(value) {
	      setNumPages($scope.$parent, value); // Readonly variable
	
	      if ( $scope.page > value ) {
	        $scope.selectPage(value);
	      } else {
	        ngModelCtrl.$render();
	      }
	    });
	  };
	
	  this.calculateTotalPages = function() {
	    var totalPages = this.itemsPerPage < 1 ? 1 : Math.ceil($scope.totalItems / this.itemsPerPage);
	    return Math.max(totalPages || 0, 1);
	  };
	
	  this.render = function() {
	    $scope.page = parseInt(ngModelCtrl.$viewValue, 10) || 1;
	  };
	
	  $scope.selectPage = function(page, evt) {
	    if (evt) {
	      evt.preventDefault();
	    }
	
	    var clickAllowed = !$scope.ngDisabled || !evt;
	    if (clickAllowed && $scope.page !== page && page > 0 && page <= $scope.totalPages) {
	      if (evt && evt.target) {
	        evt.target.blur();
	      }
	      ngModelCtrl.$setViewValue(page);
	      ngModelCtrl.$render();
	    }
	  };
	
	  $scope.getText = function(key) {
	    return $scope[key + 'Text'] || self.config[key + 'Text'];
	  };
	
	  $scope.noPrevious = function() {
	    return $scope.page === 1;
	  };
	
	  $scope.noNext = function() {
	    return $scope.page === $scope.totalPages;
	  };
	}])
	
	.constant('uibPaginationConfig', {
	  itemsPerPage: 10,
	  boundaryLinks: false,
	  directionLinks: true,
	  firstText: 'First',
	  previousText: 'Previous',
	  nextText: 'Next',
	  lastText: 'Last',
	  rotate: true
	})
	
	.directive('uibPagination', ['$parse', 'uibPaginationConfig', function($parse, paginationConfig) {
	  return {
	    restrict: 'EA',
	    scope: {
	      totalItems: '=',
	      firstText: '@',
	      previousText: '@',
	      nextText: '@',
	      lastText: '@',
	      ngDisabled:'='
	    },
	    require: ['uibPagination', '?ngModel'],
	    controller: 'UibPaginationController',
	    controllerAs: 'pagination',
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/pagination/pagination.html';
	    },
	    replace: true,
	    link: function(scope, element, attrs, ctrls) {
	      var paginationCtrl = ctrls[0], ngModelCtrl = ctrls[1];
	
	      if (!ngModelCtrl) {
	         return; // do nothing if no ng-model
	      }
	
	      // Setup configuration parameters
	      var maxSize = angular.isDefined(attrs.maxSize) ? scope.$parent.$eval(attrs.maxSize) : paginationConfig.maxSize,
	          rotate = angular.isDefined(attrs.rotate) ? scope.$parent.$eval(attrs.rotate) : paginationConfig.rotate;
	      scope.boundaryLinks = angular.isDefined(attrs.boundaryLinks) ? scope.$parent.$eval(attrs.boundaryLinks) : paginationConfig.boundaryLinks;
	      scope.directionLinks = angular.isDefined(attrs.directionLinks) ? scope.$parent.$eval(attrs.directionLinks) : paginationConfig.directionLinks;
	
	      paginationCtrl.init(ngModelCtrl, paginationConfig);
	
	      if (attrs.maxSize) {
	        scope.$parent.$watch($parse(attrs.maxSize), function(value) {
	          maxSize = parseInt(value, 10);
	          paginationCtrl.render();
	        });
	      }
	
	      // Create page object used in template
	      function makePage(number, text, isActive) {
	        return {
	          number: number,
	          text: text,
	          active: isActive
	        };
	      }
	
	      function getPages(currentPage, totalPages) {
	        var pages = [];
	
	        // Default page limits
	        var startPage = 1, endPage = totalPages;
	        var isMaxSized = angular.isDefined(maxSize) && maxSize < totalPages;
	
	        // recompute if maxSize
	        if (isMaxSized) {
	          if (rotate) {
	            // Current page is displayed in the middle of the visible ones
	            startPage = Math.max(currentPage - Math.floor(maxSize/2), 1);
	            endPage   = startPage + maxSize - 1;
	
	            // Adjust if limit is exceeded
	            if (endPage > totalPages) {
	              endPage   = totalPages;
	              startPage = endPage - maxSize + 1;
	            }
	          } else {
	            // Visible pages are paginated with maxSize
	            startPage = ((Math.ceil(currentPage / maxSize) - 1) * maxSize) + 1;
	
	            // Adjust last page if limit is exceeded
	            endPage = Math.min(startPage + maxSize - 1, totalPages);
	          }
	        }
	
	        // Add page number links
	        for (var number = startPage; number <= endPage; number++) {
	          var page = makePage(number, number, number === currentPage);
	          pages.push(page);
	        }
	
	        // Add links to move between page sets
	        if (isMaxSized && ! rotate) {
	          if (startPage > 1) {
	            var previousPageSet = makePage(startPage - 1, '...', false);
	            pages.unshift(previousPageSet);
	          }
	
	          if (endPage < totalPages) {
	            var nextPageSet = makePage(endPage + 1, '...', false);
	            pages.push(nextPageSet);
	          }
	        }
	
	        return pages;
	      }
	
	      var originalRender = paginationCtrl.render;
	      paginationCtrl.render = function() {
	        originalRender();
	        if (scope.page > 0 && scope.page <= scope.totalPages) {
	          scope.pages = getPages(scope.page, scope.totalPages);
	        }
	      };
	    }
	  };
	}])
	
	.constant('uibPagerConfig', {
	  itemsPerPage: 10,
	  previousText: '« Previous',
	  nextText: 'Next »',
	  align: true
	})
	
	.directive('uibPager', ['uibPagerConfig', function(pagerConfig) {
	  return {
	    restrict: 'EA',
	    scope: {
	      totalItems: '=',
	      previousText: '@',
	      nextText: '@',
	      ngDisabled: '='
	    },
	    require: ['uibPager', '?ngModel'],
	    controller: 'UibPaginationController',
	    controllerAs: 'pagination',
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/pagination/pager.html';
	    },
	    replace: true,
	    link: function(scope, element, attrs, ctrls) {
	      var paginationCtrl = ctrls[0], ngModelCtrl = ctrls[1];
	
	      if (!ngModelCtrl) {
	         return; // do nothing if no ng-model
	      }
	
	      scope.align = angular.isDefined(attrs.align) ? scope.$parent.$eval(attrs.align) : pagerConfig.align;
	      paginationCtrl.init(ngModelCtrl, pagerConfig);
	    }
	  };
	}]);
	
	/* Deprecated Pagination Below */
	
	angular.module('ui.bootstrap.pagination')
	.value('$paginationSuppressWarning', false)
	.controller('PaginationController', ['$scope', '$attrs', '$parse', '$log', '$paginationSuppressWarning', function($scope, $attrs, $parse, $log, $paginationSuppressWarning) {
	  if (!$paginationSuppressWarning) {
	    $log.warn('PaginationController is now deprecated. Use UibPaginationController instead.');
	  }
	
	  var self = this,
	    ngModelCtrl = { $setViewValue: angular.noop }, // nullModelCtrl
	    setNumPages = $attrs.numPages ? $parse($attrs.numPages).assign : angular.noop;
	
	  this.init = function(ngModelCtrl_, config) {
	    ngModelCtrl = ngModelCtrl_;
	    this.config = config;
	
	    ngModelCtrl.$render = function() {
	      self.render();
	    };
	
	    if ($attrs.itemsPerPage) {
	      $scope.$parent.$watch($parse($attrs.itemsPerPage), function(value) {
	        self.itemsPerPage = parseInt(value, 10);
	        $scope.totalPages = self.calculateTotalPages();
	      });
	    } else {
	      this.itemsPerPage = config.itemsPerPage;
	    }
	
	    $scope.$watch('totalItems', function() {
	      $scope.totalPages = self.calculateTotalPages();
	    });
	
	    $scope.$watch('totalPages', function(value) {
	      setNumPages($scope.$parent, value); // Readonly variable
	
	      if ( $scope.page > value ) {
	        $scope.selectPage(value);
	      } else {
	        ngModelCtrl.$render();
	      }
	    });
	  };
	
	  this.calculateTotalPages = function() {
	    var totalPages = this.itemsPerPage < 1 ? 1 : Math.ceil($scope.totalItems / this.itemsPerPage);
	    return Math.max(totalPages || 0, 1);
	  };
	
	  this.render = function() {
	    $scope.page = parseInt(ngModelCtrl.$viewValue, 10) || 1;
	  };
	
	  $scope.selectPage = function(page, evt) {
	    if (evt) {
	      evt.preventDefault();
	    }
	
	    var clickAllowed = !$scope.ngDisabled || !evt;
	    if (clickAllowed && $scope.page !== page && page > 0 && page <= $scope.totalPages) {
	      if (evt && evt.target) {
	        evt.target.blur();
	      }
	      ngModelCtrl.$setViewValue(page);
	      ngModelCtrl.$render();
	    }
	  };
	
	  $scope.getText = function(key) {
	    return $scope[key + 'Text'] || self.config[key + 'Text'];
	  };
	
	  $scope.noPrevious = function() {
	    return $scope.page === 1;
	  };
	
	  $scope.noNext = function() {
	    return $scope.page === $scope.totalPages;
	  };
	}])
	.directive('pagination', ['$parse', 'uibPaginationConfig', '$log', '$paginationSuppressWarning', function($parse, paginationConfig, $log, $paginationSuppressWarning) {
	  return {
	    restrict: 'EA',
	    scope: {
	      totalItems: '=',
	      firstText: '@',
	      previousText: '@',
	      nextText: '@',
	      lastText: '@',
	      ngDisabled:'='
	    },
	    require: ['pagination', '?ngModel'],
	    controller: 'PaginationController',
	    controllerAs: 'pagination',
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/pagination/pagination.html';
	    },
	    replace: true,
	    link: function(scope, element, attrs, ctrls) {
	      if (!$paginationSuppressWarning) {
	        $log.warn('pagination is now deprecated. Use uib-pagination instead.');
	      }
	      var paginationCtrl = ctrls[0], ngModelCtrl = ctrls[1];
	
	      if (!ngModelCtrl) {
	         return; // do nothing if no ng-model
	      }
	
	      // Setup configuration parameters
	      var maxSize = angular.isDefined(attrs.maxSize) ? scope.$parent.$eval(attrs.maxSize) : paginationConfig.maxSize,
	          rotate = angular.isDefined(attrs.rotate) ? scope.$parent.$eval(attrs.rotate) : paginationConfig.rotate;
	      scope.boundaryLinks = angular.isDefined(attrs.boundaryLinks) ? scope.$parent.$eval(attrs.boundaryLinks) : paginationConfig.boundaryLinks;
	      scope.directionLinks = angular.isDefined(attrs.directionLinks) ? scope.$parent.$eval(attrs.directionLinks) : paginationConfig.directionLinks;
	
	      paginationCtrl.init(ngModelCtrl, paginationConfig);
	
	      if (attrs.maxSize) {
	        scope.$parent.$watch($parse(attrs.maxSize), function(value) {
	          maxSize = parseInt(value, 10);
	          paginationCtrl.render();
	        });
	      }
	
	      // Create page object used in template
	      function makePage(number, text, isActive) {
	        return {
	          number: number,
	          text: text,
	          active: isActive
	        };
	      }
	
	      function getPages(currentPage, totalPages) {
	        var pages = [];
	
	        // Default page limits
	        var startPage = 1, endPage = totalPages;
	        var isMaxSized = angular.isDefined(maxSize) && maxSize < totalPages;
	
	        // recompute if maxSize
	        if (isMaxSized) {
	          if (rotate) {
	            // Current page is displayed in the middle of the visible ones
	            startPage = Math.max(currentPage - Math.floor(maxSize/2), 1);
	            endPage   = startPage + maxSize - 1;
	
	            // Adjust if limit is exceeded
	            if (endPage > totalPages) {
	              endPage   = totalPages;
	              startPage = endPage - maxSize + 1;
	            }
	          } else {
	            // Visible pages are paginated with maxSize
	            startPage = ((Math.ceil(currentPage / maxSize) - 1) * maxSize) + 1;
	
	            // Adjust last page if limit is exceeded
	            endPage = Math.min(startPage + maxSize - 1, totalPages);
	          }
	        }
	
	        // Add page number links
	        for (var number = startPage; number <= endPage; number++) {
	          var page = makePage(number, number, number === currentPage);
	          pages.push(page);
	        }
	
	        // Add links to move between page sets
	        if (isMaxSized && ! rotate) {
	          if (startPage > 1) {
	            var previousPageSet = makePage(startPage - 1, '...', false);
	            pages.unshift(previousPageSet);
	          }
	
	          if (endPage < totalPages) {
	            var nextPageSet = makePage(endPage + 1, '...', false);
	            pages.push(nextPageSet);
	          }
	        }
	
	        return pages;
	      }
	
	      var originalRender = paginationCtrl.render;
	      paginationCtrl.render = function() {
	        originalRender();
	        if (scope.page > 0 && scope.page <= scope.totalPages) {
	          scope.pages = getPages(scope.page, scope.totalPages);
	        }
	      };
	    }
	  };
	}])
	
	.directive('pager', ['uibPagerConfig', '$log', '$paginationSuppressWarning', function(pagerConfig, $log, $paginationSuppressWarning) {
	  return {
	    restrict: 'EA',
	    scope: {
	      totalItems: '=',
	      previousText: '@',
	      nextText: '@',
	      ngDisabled: '='
	    },
	    require: ['pager', '?ngModel'],
	    controller: 'PaginationController',
	    controllerAs: 'pagination',
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/pagination/pager.html';
	    },
	    replace: true,
	    link: function(scope, element, attrs, ctrls) {
	      if (!$paginationSuppressWarning) {
	        $log.warn('pager is now deprecated. Use uib-pager instead.');
	      }
	      var paginationCtrl = ctrls[0], ngModelCtrl = ctrls[1];
	
	      if (!ngModelCtrl) {
	         return; // do nothing if no ng-model
	      }
	
	      scope.align = angular.isDefined(attrs.align) ? scope.$parent.$eval(attrs.align) : pagerConfig.align;
	      paginationCtrl.init(ngModelCtrl, pagerConfig);
	    }
	  };
	}]);
	
	/**
	 * The following features are still outstanding: animation as a
	 * function, placement as a function, inside, support for more triggers than
	 * just mouse enter/leave, html tooltips, and selector delegation.
	 */
	angular.module('ui.bootstrap.tooltip', ['ui.bootstrap.position', 'ui.bootstrap.stackedMap'])
	
	/**
	 * The $tooltip service creates tooltip- and popover-like directives as well as
	 * houses global options for them.
	 */
	.provider('$uibTooltip', function() {
	  // The default options tooltip and popover.
	  var defaultOptions = {
	    placement: 'top',
	    animation: true,
	    popupDelay: 0,
	    popupCloseDelay: 0,
	    useContentExp: false
	  };
	
	  // Default hide triggers for each show trigger
	  var triggerMap = {
	    'mouseenter': 'mouseleave',
	    'click': 'click',
	    'focus': 'blur',
	    'none': ''
	  };
	
	  // The options specified to the provider globally.
	  var globalOptions = {};
	
	  /**
	   * `options({})` allows global configuration of all tooltips in the
	   * application.
	   *
	   *   var app = angular.module( 'App', ['ui.bootstrap.tooltip'], function( $tooltipProvider ) {
	   *     // place tooltips left instead of top by default
	   *     $tooltipProvider.options( { placement: 'left' } );
	   *   });
	   */
		this.options = function(value) {
			angular.extend(globalOptions, value);
		};
	
	  /**
	   * This allows you to extend the set of trigger mappings available. E.g.:
	   *
	   *   $tooltipProvider.setTriggers( 'openTrigger': 'closeTrigger' );
	   */
	  this.setTriggers = function setTriggers(triggers) {
	    angular.extend(triggerMap, triggers);
	  };
	
	  /**
	   * This is a helper function for translating camel-case to snake-case.
	   */
	  function snake_case(name) {
	    var regexp = /[A-Z]/g;
	    var separator = '-';
	    return name.replace(regexp, function(letter, pos) {
	      return (pos ? separator : '') + letter.toLowerCase();
	    });
	  }
	
	  /**
	   * Returns the actual instance of the $tooltip service.
	   * TODO support multiple triggers
	   */
	  this.$get = ['$window', '$compile', '$timeout', '$document', '$uibPosition', '$interpolate', '$rootScope', '$parse', '$$stackedMap', function($window, $compile, $timeout, $document, $position, $interpolate, $rootScope, $parse, $$stackedMap) {
	    var openedTooltips = $$stackedMap.createNew();
	    $document.on('keypress', function(e) {
	      if (e.which === 27) {
	        var last = openedTooltips.top();
	        if (last) {
	          last.value.close();
	          openedTooltips.removeTop();
	          last = null;
	        }
	      }
	    });
	
	    return function $tooltip(ttType, prefix, defaultTriggerShow, options) {
	      options = angular.extend({}, defaultOptions, globalOptions, options);
	
	      /**
	       * Returns an object of show and hide triggers.
	       *
	       * If a trigger is supplied,
	       * it is used to show the tooltip; otherwise, it will use the `trigger`
	       * option passed to the `$tooltipProvider.options` method; else it will
	       * default to the trigger supplied to this directive factory.
	       *
	       * The hide trigger is based on the show trigger. If the `trigger` option
	       * was passed to the `$tooltipProvider.options` method, it will use the
	       * mapped trigger from `triggerMap` or the passed trigger if the map is
	       * undefined; otherwise, it uses the `triggerMap` value of the show
	       * trigger; else it will just use the show trigger.
	       */
	      function getTriggers(trigger) {
	        var show = (trigger || options.trigger || defaultTriggerShow).split(' ');
	        var hide = show.map(function(trigger) {
	          return triggerMap[trigger] || trigger;
	        });
	        return {
	          show: show,
	          hide: hide
	        };
	      }
	
	      var directiveName = snake_case(ttType);
	
	      var startSym = $interpolate.startSymbol();
	      var endSym = $interpolate.endSymbol();
	      var template =
	        '<div '+ directiveName + '-popup '+
	          'title="' + startSym + 'title' + endSym + '" '+
	          (options.useContentExp ?
	            'content-exp="contentExp()" ' :
	            'content="' + startSym + 'content' + endSym + '" ') +
	          'placement="' + startSym + 'placement' + endSym + '" '+
	          'popup-class="' + startSym + 'popupClass' + endSym + '" '+
	          'animation="animation" ' +
	          'is-open="isOpen"' +
	          'origin-scope="origScope" ' +
	          'style="visibility: hidden; display: block; top: -9999px; left: -9999px;"' +
	          '>' +
	        '</div>';
	
	      return {
	        compile: function(tElem, tAttrs) {
	          var tooltipLinker = $compile(template);
	
	          return function link(scope, element, attrs, tooltipCtrl) {
	            var tooltip;
	            var tooltipLinkedScope;
	            var transitionTimeout;
	            var showTimeout;
	            var hideTimeout;
	            var positionTimeout;
	            var appendToBody = angular.isDefined(options.appendToBody) ? options.appendToBody : false;
	            var triggers = getTriggers(undefined);
	            var hasEnableExp = angular.isDefined(attrs[prefix + 'Enable']);
	            var ttScope = scope.$new(true);
	            var repositionScheduled = false;
	            var isOpenParse = angular.isDefined(attrs[prefix + 'IsOpen']) ? $parse(attrs[prefix + 'IsOpen']) : false;
	            var contentParse = options.useContentExp ? $parse(attrs[ttType]) : false;
	            var observers = [];
	
	            var positionTooltip = function() {
	              // check if tooltip exists and is not empty
	              if (!tooltip || !tooltip.html()) { return; }
	
	              if (!positionTimeout) {
	                positionTimeout = $timeout(function() {
	                  // Reset the positioning.
	                  tooltip.css({ top: 0, left: 0 });
	
	                  // Now set the calculated positioning.
	                  var ttCss = $position.positionElements(element, tooltip, ttScope.placement, appendToBody);
	                  ttCss.top += 'px';
	                  ttCss.left += 'px';
	                  ttCss.visibility = 'visible';
	                  tooltip.css(ttCss);
	
	                  positionTimeout = null;
	                }, 0, false);
	              }
	            };
	
	            // Set up the correct scope to allow transclusion later
	            ttScope.origScope = scope;
	
	            // By default, the tooltip is not open.
	            // TODO add ability to start tooltip opened
	            ttScope.isOpen = false;
	            openedTooltips.add(ttScope, {
	              close: hide
	            });
	
	            function toggleTooltipBind() {
	              if (!ttScope.isOpen) {
	                showTooltipBind();
	              } else {
	                hideTooltipBind();
	              }
	            }
	
	            // Show the tooltip with delay if specified, otherwise show it immediately
	            function showTooltipBind() {
	              if (hasEnableExp && !scope.$eval(attrs[prefix + 'Enable'])) {
	                return;
	              }
	
	              cancelHide();
	              prepareTooltip();
	
	              if (ttScope.popupDelay) {
	                // Do nothing if the tooltip was already scheduled to pop-up.
	                // This happens if show is triggered multiple times before any hide is triggered.
	                if (!showTimeout) {
	                  showTimeout = $timeout(show, ttScope.popupDelay, false);
	                }
	              } else {
	                show();
	              }
	            }
	
	            function hideTooltipBind() {
	              cancelShow();
	
	              if (ttScope.popupCloseDelay) {
	                if (!hideTimeout) {
	                  hideTimeout = $timeout(hide, ttScope.popupCloseDelay, false);
	                }
	              } else {
	                hide();
	              }
	            }
	
	            // Show the tooltip popup element.
	            function show() {
	              cancelShow();
	              cancelHide();
	
	              // Don't show empty tooltips.
	              if (!ttScope.content) {
	                return angular.noop;
	              }
	
	              createTooltip();
	
	              // And show the tooltip.
	              ttScope.$evalAsync(function() {
	                ttScope.isOpen = true;
	                assignIsOpen(true);
	                positionTooltip();
	              });
	            }
	
	            function cancelShow() {
	              if (showTimeout) {
	                $timeout.cancel(showTimeout);
	                showTimeout = null;
	              }
	
	              if (positionTimeout) {
	                $timeout.cancel(positionTimeout);
	                positionTimeout = null;
	              }
	            }
	
	            // Hide the tooltip popup element.
	            function hide() {
	              cancelShow();
	              cancelHide();
	
	              if (!ttScope) {
	                return;
	              }
	
	              // First things first: we don't show it anymore.
	              ttScope.$evalAsync(function() {
	                ttScope.isOpen = false;
	                assignIsOpen(false);
	                // And now we remove it from the DOM. However, if we have animation, we
	                // need to wait for it to expire beforehand.
	                // FIXME: this is a placeholder for a port of the transitions library.
	                // The fade transition in TWBS is 150ms.
	                if (ttScope.animation) {
	                  if (!transitionTimeout) {
	                    transitionTimeout = $timeout(removeTooltip, 150, false);
	                  }
	                } else {
	                  removeTooltip();
	                }
	              });
	            }
	
	            function cancelHide() {
	              if (hideTimeout) {
	                $timeout.cancel(hideTimeout);
	                hideTimeout = null;
	              }
	              if (transitionTimeout) {
	                $timeout.cancel(transitionTimeout);
	                transitionTimeout = null;
	              }
	            }
	
	            function createTooltip() {
	              // There can only be one tooltip element per directive shown at once.
	              if (tooltip) {
	                return;
	              }
	
	              tooltipLinkedScope = ttScope.$new();
	              tooltip = tooltipLinker(tooltipLinkedScope, function(tooltip) {
	                if (appendToBody) {
	                  $document.find('body').append(tooltip);
	                } else {
	                  element.after(tooltip);
	                }
	              });
	
	              prepObservers();
	            }
	
	            function removeTooltip() {
	              unregisterObservers();
	
	              transitionTimeout = null;
	              if (tooltip) {
	                tooltip.remove();
	                tooltip = null;
	              }
	              if (tooltipLinkedScope) {
	                tooltipLinkedScope.$destroy();
	                tooltipLinkedScope = null;
	              }
	            }
	
	            /**
	             * Set the inital scope values. Once
	             * the tooltip is created, the observers
	             * will be added to keep things in synch.
	             */
	            function prepareTooltip() {
	              ttScope.title = attrs[prefix + 'Title'];
	              if (contentParse) {
	                ttScope.content = contentParse(scope);
	              } else {
	                ttScope.content = attrs[ttType];
	              }
	
	              ttScope.popupClass = attrs[prefix + 'Class'];
	              ttScope.placement = angular.isDefined(attrs[prefix + 'Placement']) ? attrs[prefix + 'Placement'] : options.placement;
	
	              var delay = parseInt(attrs[prefix + 'PopupDelay'], 10);
	              var closeDelay = parseInt(attrs[prefix + 'PopupCloseDelay'], 10);
	              ttScope.popupDelay = !isNaN(delay) ? delay : options.popupDelay;
	              ttScope.popupCloseDelay = !isNaN(closeDelay) ? closeDelay : options.popupCloseDelay;
	            }
	
	            function assignIsOpen(isOpen) {
	              if (isOpenParse && angular.isFunction(isOpenParse.assign)) {
	                isOpenParse.assign(scope, isOpen);
	              }
	            }
	
	            ttScope.contentExp = function() {
	              return ttScope.content;
	            };
	
	            /**
	             * Observe the relevant attributes.
	             */
	            attrs.$observe('disabled', function(val) {
	              if (val) {
	                cancelShow();
	              }
	
	              if (val && ttScope.isOpen) {
	                hide();
	              }
	            });
	
	            if (isOpenParse) {
	              scope.$watch(isOpenParse, function(val) {
	                /*jshint -W018 */
	                if (ttScope && !val === ttScope.isOpen) {
	                  toggleTooltipBind();
	                }
	                /*jshint +W018 */
	              });
	            }
	
	            function prepObservers() {
	              observers.length = 0;
	
	              if (contentParse) {
	                observers.push(
	                  scope.$watch(contentParse, function(val) {
	                    ttScope.content = val;
	                    if (!val && ttScope.isOpen) {
	                      hide();
	                    }
	                  })
	                );
	
	                observers.push(
	                  tooltipLinkedScope.$watch(function() {
	                    if (!repositionScheduled) {
	                      repositionScheduled = true;
	                      tooltipLinkedScope.$$postDigest(function() {
	                        repositionScheduled = false;
	                        if (ttScope && ttScope.isOpen) {
	                          positionTooltip();
	                        }
	                      });
	                    }
	                  })
	                );
	              } else {
	                observers.push(
	                  attrs.$observe(ttType, function(val) {
	                    ttScope.content = val;
	                    if (!val && ttScope.isOpen) {
	                      hide();
	                    } else {
	                      positionTooltip();
	                    }
	                  })
	                );
	              }
	
	              observers.push(
	                attrs.$observe(prefix + 'Title', function(val) {
	                  ttScope.title = val;
	                  if (ttScope.isOpen) {
	                    positionTooltip();
	                  }
	                })
	              );
	
	              observers.push(
	                attrs.$observe(prefix + 'Placement', function(val) {
	                  ttScope.placement = val ? val : options.placement;
	                  if (ttScope.isOpen) {
	                    positionTooltip();
	                  }
	                })
	              );
	            }
	
	            function unregisterObservers() {
	              if (observers.length) {
	                angular.forEach(observers, function(observer) {
	                  observer();
	                });
	                observers.length = 0;
	              }
	            }
	
	            var unregisterTriggers = function() {
	              triggers.show.forEach(function(trigger) {
	                element.unbind(trigger, showTooltipBind);
	              });
	              triggers.hide.forEach(function(trigger) {
	                trigger.split(' ').forEach(function(hideTrigger) {
	                  element[0].removeEventListener(hideTrigger, hideTooltipBind);
	                });
	              });
	            };
	
	            function prepTriggers() {
	              var val = attrs[prefix + 'Trigger'];
	              unregisterTriggers();
	
	              triggers = getTriggers(val);
	
	              if (triggers.show !== 'none') {
	                triggers.show.forEach(function(trigger, idx) {
	                  // Using raw addEventListener due to jqLite/jQuery bug - #4060
	                  if (trigger === triggers.hide[idx]) {
	                    element[0].addEventListener(trigger, toggleTooltipBind);
	                  } else if (trigger) {
	                    element[0].addEventListener(trigger, showTooltipBind);
	                    triggers.hide[idx].split(' ').forEach(function(trigger) {
	                      element[0].addEventListener(trigger, hideTooltipBind);
	                    });
	                  }
	
	                  element.on('keypress', function(e) {
	                    if (e.which === 27) {
	                      hideTooltipBind();
	                    }
	                  });
	                });
	              }
	            }
	
	            prepTriggers();
	
	            var animation = scope.$eval(attrs[prefix + 'Animation']);
	            ttScope.animation = angular.isDefined(animation) ? !!animation : options.animation;
	
	            var appendToBodyVal = scope.$eval(attrs[prefix + 'AppendToBody']);
	            appendToBody = angular.isDefined(appendToBodyVal) ? appendToBodyVal : appendToBody;
	
	            // if a tooltip is attached to <body> we need to remove it on
	            // location change as its parent scope will probably not be destroyed
	            // by the change.
	            if (appendToBody) {
	              scope.$on('$locationChangeSuccess', function closeTooltipOnLocationChangeSuccess() {
	                if (ttScope.isOpen) {
	                  hide();
	                }
	              });
	            }
	
	            // Make sure tooltip is destroyed and removed.
	            scope.$on('$destroy', function onDestroyTooltip() {
	              cancelShow();
	              cancelHide();
	              unregisterTriggers();
	              removeTooltip();
	              openedTooltips.remove(ttScope);
	              ttScope = null;
	            });
	          };
	        }
	      };
	    };
	  }];
	})
	
	// This is mostly ngInclude code but with a custom scope
	.directive('uibTooltipTemplateTransclude', [
	         '$animate', '$sce', '$compile', '$templateRequest',
	function ($animate ,  $sce ,  $compile ,  $templateRequest) {
	  return {
	    link: function(scope, elem, attrs) {
	      var origScope = scope.$eval(attrs.tooltipTemplateTranscludeScope);
	
	      var changeCounter = 0,
	        currentScope,
	        previousElement,
	        currentElement;
	
	      var cleanupLastIncludeContent = function() {
	        if (previousElement) {
	          previousElement.remove();
	          previousElement = null;
	        }
	
	        if (currentScope) {
	          currentScope.$destroy();
	          currentScope = null;
	        }
	
	        if (currentElement) {
	          $animate.leave(currentElement).then(function() {
	            previousElement = null;
	          });
	          previousElement = currentElement;
	          currentElement = null;
	        }
	      };
	
	      scope.$watch($sce.parseAsResourceUrl(attrs.uibTooltipTemplateTransclude), function(src) {
	        var thisChangeId = ++changeCounter;
	
	        if (src) {
	          //set the 2nd param to true to ignore the template request error so that the inner
	          //contents and scope can be cleaned up.
	          $templateRequest(src, true).then(function(response) {
	            if (thisChangeId !== changeCounter) { return; }
	            var newScope = origScope.$new();
	            var template = response;
	
	            var clone = $compile(template)(newScope, function(clone) {
	              cleanupLastIncludeContent();
	              $animate.enter(clone, elem);
	            });
	
	            currentScope = newScope;
	            currentElement = clone;
	
	            currentScope.$emit('$includeContentLoaded', src);
	          }, function() {
	            if (thisChangeId === changeCounter) {
	              cleanupLastIncludeContent();
	              scope.$emit('$includeContentError', src);
	            }
	          });
	          scope.$emit('$includeContentRequested', src);
	        } else {
	          cleanupLastIncludeContent();
	        }
	      });
	
	      scope.$on('$destroy', cleanupLastIncludeContent);
	    }
	  };
	}])
	
	/**
	 * Note that it's intentional that these classes are *not* applied through $animate.
	 * They must not be animated as they're expected to be present on the tooltip on
	 * initialization.
	 */
	.directive('uibTooltipClasses', function() {
	  return {
	    restrict: 'A',
	    link: function(scope, element, attrs) {
	      if (scope.placement) {
	        element.addClass(scope.placement);
	      }
	
	      if (scope.popupClass) {
	        element.addClass(scope.popupClass);
	      }
	
	      if (scope.animation()) {
	        element.addClass(attrs.tooltipAnimationClass);
	      }
	    }
	  };
	})
	
	.directive('uibTooltipPopup', function() {
	  return {
	    replace: true,
	    scope: { content: '@', placement: '@', popupClass: '@', animation: '&', isOpen: '&' },
	    templateUrl: 'template/tooltip/tooltip-popup.html',
	    link: function(scope, element) {
	      element.addClass('tooltip');
	    }
	  };
	})
	
	.directive('uibTooltip', [ '$uibTooltip', function($uibTooltip) {
	  return $uibTooltip('uibTooltip', 'tooltip', 'mouseenter');
	}])
	
	.directive('uibTooltipTemplatePopup', function() {
	  return {
	    replace: true,
	    scope: { contentExp: '&', placement: '@', popupClass: '@', animation: '&', isOpen: '&',
	      originScope: '&' },
	    templateUrl: 'template/tooltip/tooltip-template-popup.html',
	    link: function(scope, element) {
	      element.addClass('tooltip');
	    }
	  };
	})
	
	.directive('uibTooltipTemplate', ['$uibTooltip', function($uibTooltip) {
	  return $uibTooltip('uibTooltipTemplate', 'tooltip', 'mouseenter', {
	    useContentExp: true
	  });
	}])
	
	.directive('uibTooltipHtmlPopup', function() {
	  return {
	    replace: true,
	    scope: { contentExp: '&', placement: '@', popupClass: '@', animation: '&', isOpen: '&' },
	    templateUrl: 'template/tooltip/tooltip-html-popup.html',
	    link: function(scope, element) {
	      element.addClass('tooltip');
	    }
	  };
	})
	
	.directive('uibTooltipHtml', ['$uibTooltip', function($uibTooltip) {
	  return $uibTooltip('uibTooltipHtml', 'tooltip', 'mouseenter', {
	    useContentExp: true
	  });
	}]);
	
	/* Deprecated tooltip below */
	
	angular.module('ui.bootstrap.tooltip')
	
	.value('$tooltipSuppressWarning', false)
	
	.provider('$tooltip', ['$uibTooltipProvider', function($uibTooltipProvider) {
	  angular.extend(this, $uibTooltipProvider);
	
	  this.$get = ['$log', '$tooltipSuppressWarning', '$injector', function($log, $tooltipSuppressWarning, $injector) {
	    if (!$tooltipSuppressWarning) {
	      $log.warn('$tooltip is now deprecated. Use $uibTooltip instead.');
	    }
	
	    return $injector.invoke($uibTooltipProvider.$get);
	  }];
	}])
	
	// This is mostly ngInclude code but with a custom scope
	.directive('tooltipTemplateTransclude', [
	         '$animate', '$sce', '$compile', '$templateRequest', '$log', '$tooltipSuppressWarning',
	function ($animate ,  $sce ,  $compile ,  $templateRequest,   $log,   $tooltipSuppressWarning) {
	  return {
	    link: function(scope, elem, attrs) {
	      if (!$tooltipSuppressWarning) {
	        $log.warn('tooltip-template-transclude is now deprecated. Use uib-tooltip-template-transclude instead.');
	      }
	
	      var origScope = scope.$eval(attrs.tooltipTemplateTranscludeScope);
	
	      var changeCounter = 0,
	        currentScope,
	        previousElement,
	        currentElement;
	
	      var cleanupLastIncludeContent = function() {
	        if (previousElement) {
	          previousElement.remove();
	          previousElement = null;
	        }
	        if (currentScope) {
	          currentScope.$destroy();
	          currentScope = null;
	        }
	        if (currentElement) {
	          $animate.leave(currentElement).then(function() {
	            previousElement = null;
	          });
	          previousElement = currentElement;
	          currentElement = null;
	        }
	      };
	
	      scope.$watch($sce.parseAsResourceUrl(attrs.tooltipTemplateTransclude), function(src) {
	        var thisChangeId = ++changeCounter;
	
	        if (src) {
	          //set the 2nd param to true to ignore the template request error so that the inner
	          //contents and scope can be cleaned up.
	          $templateRequest(src, true).then(function(response) {
	            if (thisChangeId !== changeCounter) { return; }
	            var newScope = origScope.$new();
	            var template = response;
	
	            var clone = $compile(template)(newScope, function(clone) {
	              cleanupLastIncludeContent();
	              $animate.enter(clone, elem);
	            });
	
	            currentScope = newScope;
	            currentElement = clone;
	
	            currentScope.$emit('$includeContentLoaded', src);
	          }, function() {
	            if (thisChangeId === changeCounter) {
	              cleanupLastIncludeContent();
	              scope.$emit('$includeContentError', src);
	            }
	          });
	          scope.$emit('$includeContentRequested', src);
	        } else {
	          cleanupLastIncludeContent();
	        }
	      });
	
	      scope.$on('$destroy', cleanupLastIncludeContent);
	    }
	  };
	}])
	
	.directive('tooltipClasses', ['$log', '$tooltipSuppressWarning', function($log, $tooltipSuppressWarning) {
	  return {
	    restrict: 'A',
	    link: function(scope, element, attrs) {
	      if (!$tooltipSuppressWarning) {
	        $log.warn('tooltip-classes is now deprecated. Use uib-tooltip-classes instead.');
	      }
	
	      if (scope.placement) {
	        element.addClass(scope.placement);
	      }
	      if (scope.popupClass) {
	        element.addClass(scope.popupClass);
	      }
	      if (scope.animation()) {
	        element.addClass(attrs.tooltipAnimationClass);
	      }
	    }
	  };
	}])
	
	.directive('tooltipPopup', ['$log', '$tooltipSuppressWarning', function($log, $tooltipSuppressWarning) {
	  return {
	    replace: true,
	    scope: { content: '@', placement: '@', popupClass: '@', animation: '&', isOpen: '&' },
	    templateUrl: 'template/tooltip/tooltip-popup.html',
	    link: function(scope, element) {
	      if (!$tooltipSuppressWarning) {
	        $log.warn('tooltip-popup is now deprecated. Use uib-tooltip-popup instead.');
	      }
	
	      element.addClass('tooltip');
	    }
	  };
	}])
	
	.directive('tooltip', ['$tooltip', function($tooltip) {
	  return $tooltip('tooltip', 'tooltip', 'mouseenter');
	}])
	
	.directive('tooltipTemplatePopup', ['$log', '$tooltipSuppressWarning', function($log, $tooltipSuppressWarning) {
	  return {
	    replace: true,
	    scope: { contentExp: '&', placement: '@', popupClass: '@', animation: '&', isOpen: '&',
	      originScope: '&' },
	    templateUrl: 'template/tooltip/tooltip-template-popup.html',
	    link: function(scope, element) {
	      if (!$tooltipSuppressWarning) {
	        $log.warn('tooltip-template-popup is now deprecated. Use uib-tooltip-template-popup instead.');
	      }
	
	      element.addClass('tooltip');
	    }
	  };
	}])
	
	.directive('tooltipTemplate', ['$tooltip', function($tooltip) {
	  return $tooltip('tooltipTemplate', 'tooltip', 'mouseenter', {
	    useContentExp: true
	  });
	}])
	
	.directive('tooltipHtmlPopup', ['$log', '$tooltipSuppressWarning', function($log, $tooltipSuppressWarning) {
	  return {
	    replace: true,
	    scope: { contentExp: '&', placement: '@', popupClass: '@', animation: '&', isOpen: '&' },
	    templateUrl: 'template/tooltip/tooltip-html-popup.html',
	    link: function(scope, element) {
	      if (!$tooltipSuppressWarning) {
	        $log.warn('tooltip-html-popup is now deprecated. Use uib-tooltip-html-popup instead.');
	      }
	
	      element.addClass('tooltip');
	    }
	  };
	}])
	
	.directive('tooltipHtml', ['$tooltip', function($tooltip) {
	  return $tooltip('tooltipHtml', 'tooltip', 'mouseenter', {
	    useContentExp: true
	  });
	}]);
	
	/**
	 * The following features are still outstanding: popup delay, animation as a
	 * function, placement as a function, inside, support for more triggers than
	 * just mouse enter/leave, and selector delegatation.
	 */
	angular.module('ui.bootstrap.popover', ['ui.bootstrap.tooltip'])
	
	.directive('uibPopoverTemplatePopup', function() {
	  return {
	    replace: true,
	    scope: { title: '@', contentExp: '&', placement: '@', popupClass: '@', animation: '&', isOpen: '&',
	      originScope: '&' },
	    templateUrl: 'template/popover/popover-template.html',
	    link: function(scope, element) {
	      element.addClass('popover');
	    }
	  };
	})
	
	.directive('uibPopoverTemplate', ['$uibTooltip', function($uibTooltip) {
	  return $uibTooltip('uibPopoverTemplate', 'popover', 'click', {
	    useContentExp: true
	  });
	}])
	
	.directive('uibPopoverHtmlPopup', function() {
	  return {
	    replace: true,
	    scope: { contentExp: '&', title: '@', placement: '@', popupClass: '@', animation: '&', isOpen: '&' },
	    templateUrl: 'template/popover/popover-html.html',
	    link: function(scope, element) {
	      element.addClass('popover');
	    }
	  };
	})
	
	.directive('uibPopoverHtml', ['$uibTooltip', function($uibTooltip) {
	  return $uibTooltip('uibPopoverHtml', 'popover', 'click', {
	    useContentExp: true
	  });
	}])
	
	.directive('uibPopoverPopup', function() {
	  return {
	    replace: true,
	    scope: { title: '@', content: '@', placement: '@', popupClass: '@', animation: '&', isOpen: '&' },
	    templateUrl: 'template/popover/popover.html',
	    link: function(scope, element) {
	      element.addClass('popover');
	    }
	  };
	})
	
	.directive('uibPopover', ['$uibTooltip', function($uibTooltip) {
	  return $uibTooltip('uibPopover', 'popover', 'click');
	}]);
	
	/* Deprecated popover below */
	
	angular.module('ui.bootstrap.popover')
	
	.value('$popoverSuppressWarning', false)
	
	.directive('popoverTemplatePopup', ['$log', '$popoverSuppressWarning', function($log, $popoverSuppressWarning) {
	  return {
	    replace: true,
	    scope: { title: '@', contentExp: '&', placement: '@', popupClass: '@', animation: '&', isOpen: '&',
	      originScope: '&' },
	    templateUrl: 'template/popover/popover-template.html',
	    link: function(scope, element) {
	      if (!$popoverSuppressWarning) {
	        $log.warn('popover-template-popup is now deprecated. Use uib-popover-template-popup instead.');
	      }
	
	      element.addClass('popover');
	    }
	  };
	}])
	
	.directive('popoverTemplate', ['$tooltip', function($tooltip) {
	  return $tooltip('popoverTemplate', 'popover', 'click', {
	    useContentExp: true
	  });
	}])
	
	.directive('popoverHtmlPopup', ['$log', '$popoverSuppressWarning', function($log, $popoverSuppressWarning) {
	  return {
	    replace: true,
	    scope: { contentExp: '&', title: '@', placement: '@', popupClass: '@', animation: '&', isOpen: '&' },
	    templateUrl: 'template/popover/popover-html.html',
	    link: function(scope, element) {
	      if (!$popoverSuppressWarning) {
	        $log.warn('popover-html-popup is now deprecated. Use uib-popover-html-popup instead.');
	      }
	
	      element.addClass('popover');
	    }
	  };
	}])
	
	.directive('popoverHtml', ['$tooltip', function($tooltip) {
	  return $tooltip('popoverHtml', 'popover', 'click', {
	    useContentExp: true
	  });
	}])
	
	.directive('popoverPopup', ['$log', '$popoverSuppressWarning', function($log, $popoverSuppressWarning) {
	  return {
	    replace: true,
	    scope: { title: '@', content: '@', placement: '@', popupClass: '@', animation: '&', isOpen: '&' },
	    templateUrl: 'template/popover/popover.html',
	    link: function(scope, element) {
	      if (!$popoverSuppressWarning) {
	        $log.warn('popover-popup is now deprecated. Use uib-popover-popup instead.');
	      }
	
	      element.addClass('popover');
	    }
	  };
	}])
	
	.directive('popover', ['$tooltip', function($tooltip) {
	
	  return $tooltip('popover', 'popover', 'click');
	}]);
	
	angular.module('ui.bootstrap.progressbar', [])
	
	.constant('uibProgressConfig', {
	  animate: true,
	  max: 100
	})
	
	.controller('UibProgressController', ['$scope', '$attrs', 'uibProgressConfig', function($scope, $attrs, progressConfig) {
	  var self = this,
	      animate = angular.isDefined($attrs.animate) ? $scope.$parent.$eval($attrs.animate) : progressConfig.animate;
	
	  this.bars = [];
	  $scope.max = angular.isDefined($scope.max) ? $scope.max : progressConfig.max;
	
	  this.addBar = function(bar, element, attrs) {
	    if (!animate) {
	      element.css({'transition': 'none'});
	    }
	
	    this.bars.push(bar);
	
	    bar.max = $scope.max;
	    bar.title = attrs && angular.isDefined(attrs.title) ? attrs.title : 'progressbar';
	
	    bar.$watch('value', function(value) {
	      bar.recalculatePercentage();
	    });
	
	    bar.recalculatePercentage = function() {
	      var totalPercentage = self.bars.reduce(function(total, bar) {
	        bar.percent = +(100 * bar.value / bar.max).toFixed(2);
	        return total + bar.percent;
	      }, 0);
	
	      if (totalPercentage > 100) {
	        bar.percent -= totalPercentage - 100;
	      }
	    };
	
	    bar.$on('$destroy', function() {
	      element = null;
	      self.removeBar(bar);
	    });
	  };
	
	  this.removeBar = function(bar) {
	    this.bars.splice(this.bars.indexOf(bar), 1);
	    this.bars.forEach(function (bar) {
	      bar.recalculatePercentage();
	    });
	  };
	
	  $scope.$watch('max', function(max) {
	    self.bars.forEach(function(bar) {
	      bar.max = $scope.max;
	      bar.recalculatePercentage();
	    });
	  });
	}])
	
	.directive('uibProgress', function() {
	  return {
	    replace: true,
	    transclude: true,
	    controller: 'UibProgressController',
	    require: 'uibProgress',
	    scope: {
	      max: '=?'
	    },
	    templateUrl: 'template/progressbar/progress.html'
	  };
	})
	
	.directive('uibBar', function() {
	  return {
	    replace: true,
	    transclude: true,
	    require: '^uibProgress',
	    scope: {
	      value: '=',
	      type: '@'
	    },
	    templateUrl: 'template/progressbar/bar.html',
	    link: function(scope, element, attrs, progressCtrl) {
	      progressCtrl.addBar(scope, element, attrs);
	    }
	  };
	})
	
	.directive('uibProgressbar', function() {
	  return {
	    replace: true,
	    transclude: true,
	    controller: 'UibProgressController',
	    scope: {
	      value: '=',
	      max: '=?',
	      type: '@'
	    },
	    templateUrl: 'template/progressbar/progressbar.html',
	    link: function(scope, element, attrs, progressCtrl) {
	      progressCtrl.addBar(scope, angular.element(element.children()[0]), {title: attrs.title});
	    }
	  };
	});
	
	/* Deprecated progressbar below */
	
	angular.module('ui.bootstrap.progressbar')
	
	.value('$progressSuppressWarning', false)
	
	.controller('ProgressController', ['$scope', '$attrs', 'uibProgressConfig', '$log', '$progressSuppressWarning', function($scope, $attrs, progressConfig, $log, $progressSuppressWarning) {
	  if (!$progressSuppressWarning) {
	    $log.warn('ProgressController is now deprecated. Use UibProgressController instead.');
	  }
	
	  var self = this,
	    animate = angular.isDefined($attrs.animate) ? $scope.$parent.$eval($attrs.animate) : progressConfig.animate;
	
	  this.bars = [];
	  $scope.max = angular.isDefined($scope.max) ? $scope.max : progressConfig.max;
	
	  this.addBar = function(bar, element, attrs) {
	    if (!animate) {
	      element.css({'transition': 'none'});
	    }
	
	    this.bars.push(bar);
	
	    bar.max = $scope.max;
	    bar.title = attrs && angular.isDefined(attrs.title) ? attrs.title : 'progressbar';
	
	    bar.$watch('value', function(value) {
	      bar.recalculatePercentage();
	    });
	
	    bar.recalculatePercentage = function() {
	      bar.percent = +(100 * bar.value / bar.max).toFixed(2);
	
	      var totalPercentage = self.bars.reduce(function(total, bar) {
	        return total + bar.percent;
	      }, 0);
	
	      if (totalPercentage > 100) {
	        bar.percent -= totalPercentage - 100;
	      }
	    };
	
	    bar.$on('$destroy', function() {
	      element = null;
	      self.removeBar(bar);
	    });
	  };
	
	  this.removeBar = function(bar) {
	    this.bars.splice(this.bars.indexOf(bar), 1);
	  };
	
	  $scope.$watch('max', function(max) {
	    self.bars.forEach(function(bar) {
	      bar.max = $scope.max;
	      bar.recalculatePercentage();
	    });
	  });
	}])
	
	.directive('progress', ['$log', '$progressSuppressWarning', function($log, $progressSuppressWarning) {
	  return {
	    replace: true,
	    transclude: true,
	    controller: 'ProgressController',
	    require: 'progress',
	    scope: {
	      max: '=?',
	      title: '@?'
	    },
	    templateUrl: 'template/progressbar/progress.html',
	    link: function() {
	      if (!$progressSuppressWarning) {
	        $log.warn('progress is now deprecated. Use uib-progress instead.');
	      }
	    }
	  };
	}])
	
	.directive('bar', ['$log', '$progressSuppressWarning', function($log, $progressSuppressWarning) {
	  return {
	    replace: true,
	    transclude: true,
	    require: '^progress',
	    scope: {
	      value: '=',
	      type: '@'
	    },
	    templateUrl: 'template/progressbar/bar.html',
	    link: function(scope, element, attrs, progressCtrl) {
	      if (!$progressSuppressWarning) {
	        $log.warn('bar is now deprecated. Use uib-bar instead.');
	      }
	      progressCtrl.addBar(scope, element);
	    }
	  };
	}])
	
	.directive('progressbar', ['$log', '$progressSuppressWarning', function($log, $progressSuppressWarning) {
	  return {
	    replace: true,
	    transclude: true,
	    controller: 'ProgressController',
	    scope: {
	      value: '=',
	      max: '=?',
	      type: '@'
	    },
	    templateUrl: 'template/progressbar/progressbar.html',
	    link: function(scope, element, attrs, progressCtrl) {
	      if (!$progressSuppressWarning) {
	        $log.warn('progressbar is now deprecated. Use uib-progressbar instead.');
	      }
	      progressCtrl.addBar(scope, angular.element(element.children()[0]), {title: attrs.title});
	    }
	  };
	}]);
	
	angular.module('ui.bootstrap.rating', [])
	
	.constant('uibRatingConfig', {
	  max: 5,
	  stateOn: null,
	  stateOff: null,
	  titles : ['one', 'two', 'three', 'four', 'five']
	})
	
	.controller('UibRatingController', ['$scope', '$attrs', 'uibRatingConfig', function($scope, $attrs, ratingConfig) {
	  var ngModelCtrl  = { $setViewValue: angular.noop };
	
	  this.init = function(ngModelCtrl_) {
	    ngModelCtrl = ngModelCtrl_;
	    ngModelCtrl.$render = this.render;
	
	    ngModelCtrl.$formatters.push(function(value) {
	      if (angular.isNumber(value) && value << 0 !== value) {
	        value = Math.round(value);
	      }
	      return value;
	    });
	
	    this.stateOn = angular.isDefined($attrs.stateOn) ? $scope.$parent.$eval($attrs.stateOn) : ratingConfig.stateOn;
	    this.stateOff = angular.isDefined($attrs.stateOff) ? $scope.$parent.$eval($attrs.stateOff) : ratingConfig.stateOff;
	    var tmpTitles = angular.isDefined($attrs.titles)  ? $scope.$parent.$eval($attrs.titles) : ratingConfig.titles ;
	    this.titles = angular.isArray(tmpTitles) && tmpTitles.length > 0 ?
	      tmpTitles : ratingConfig.titles;
	
	    var ratingStates = angular.isDefined($attrs.ratingStates) ?
	      $scope.$parent.$eval($attrs.ratingStates) :
	      new Array(angular.isDefined($attrs.max) ? $scope.$parent.$eval($attrs.max) : ratingConfig.max);
	    $scope.range = this.buildTemplateObjects(ratingStates);
	  };
	
	  this.buildTemplateObjects = function(states) {
	    for (var i = 0, n = states.length; i < n; i++) {
	      states[i] = angular.extend({ index: i }, { stateOn: this.stateOn, stateOff: this.stateOff, title: this.getTitle(i) }, states[i]);
	    }
	    return states;
	  };
	
	  this.getTitle = function(index) {
	    if (index >= this.titles.length) {
	      return index + 1;
	    } else {
	      return this.titles[index];
	    }
	  };
	
	  $scope.rate = function(value) {
	    if (!$scope.readonly && value >= 0 && value <= $scope.range.length) {
	      ngModelCtrl.$setViewValue(ngModelCtrl.$viewValue === value ? 0 : value);
	      ngModelCtrl.$render();
	    }
	  };
	
	  $scope.enter = function(value) {
	    if (!$scope.readonly) {
	      $scope.value = value;
	    }
	    $scope.onHover({value: value});
	  };
	
	  $scope.reset = function() {
	    $scope.value = ngModelCtrl.$viewValue;
	    $scope.onLeave();
	  };
	
	  $scope.onKeydown = function(evt) {
	    if (/(37|38|39|40)/.test(evt.which)) {
	      evt.preventDefault();
	      evt.stopPropagation();
	      $scope.rate($scope.value + (evt.which === 38 || evt.which === 39 ? 1 : -1));
	    }
	  };
	
	  this.render = function() {
	    $scope.value = ngModelCtrl.$viewValue;
	  };
	}])
	
	.directive('uibRating', function() {
	  return {
	    require: ['uibRating', 'ngModel'],
	    scope: {
	      readonly: '=?',
	      onHover: '&',
	      onLeave: '&'
	    },
	    controller: 'UibRatingController',
	    templateUrl: 'template/rating/rating.html',
	    replace: true,
	    link: function(scope, element, attrs, ctrls) {
	      var ratingCtrl = ctrls[0], ngModelCtrl = ctrls[1];
	      ratingCtrl.init(ngModelCtrl);
	    }
	  };
	});
	
	/* Deprecated rating below */
	
	angular.module('ui.bootstrap.rating')
	
	.value('$ratingSuppressWarning', false)
	
	.controller('RatingController', ['$scope', '$attrs', '$controller', '$log', '$ratingSuppressWarning', function($scope, $attrs, $controller, $log, $ratingSuppressWarning) {
	  if (!$ratingSuppressWarning) {
	    $log.warn('RatingController is now deprecated. Use UibRatingController instead.');
	  }
	
	  angular.extend(this, $controller('UibRatingController', {
	    $scope: $scope,
	    $attrs: $attrs
	  }));
	}])
	
	.directive('rating', ['$log', '$ratingSuppressWarning', function($log, $ratingSuppressWarning) {
	  return {
	    require: ['rating', 'ngModel'],
	    scope: {
	      readonly: '=?',
	      onHover: '&',
	      onLeave: '&'
	    },
	    controller: 'RatingController',
	    templateUrl: 'template/rating/rating.html',
	    replace: true,
	    link: function(scope, element, attrs, ctrls) {
	      if (!$ratingSuppressWarning) {
	        $log.warn('rating is now deprecated. Use uib-rating instead.');
	      }
	      var ratingCtrl = ctrls[0], ngModelCtrl = ctrls[1];
	      ratingCtrl.init(ngModelCtrl);
	    }
	  };
	}]);
	
	
	/**
	 * @ngdoc overview
	 * @name ui.bootstrap.tabs
	 *
	 * @description
	 * AngularJS version of the tabs directive.
	 */
	
	angular.module('ui.bootstrap.tabs', [])
	
	.controller('UibTabsetController', ['$scope', function ($scope) {
	  var ctrl = this,
	      tabs = ctrl.tabs = $scope.tabs = [];
	
	  ctrl.select = function(selectedTab) {
	    angular.forEach(tabs, function(tab) {
	      if (tab.active && tab !== selectedTab) {
	        tab.active = false;
	        tab.onDeselect();
	        selectedTab.selectCalled = false;
	      }
	    });
	    selectedTab.active = true;
	    // only call select if it has not already been called
	    if (!selectedTab.selectCalled) {
	      selectedTab.onSelect();
	      selectedTab.selectCalled = true;
	    }
	  };
	
	  ctrl.addTab = function addTab(tab) {
	    tabs.push(tab);
	    // we can't run the select function on the first tab
	    // since that would select it twice
	    if (tabs.length === 1 && tab.active !== false) {
	      tab.active = true;
	    } else if (tab.active) {
	      ctrl.select(tab);
	    } else {
	      tab.active = false;
	    }
	  };
	
	  ctrl.removeTab = function removeTab(tab) {
	    var index = tabs.indexOf(tab);
	    //Select a new tab if the tab to be removed is selected and not destroyed
	    if (tab.active && tabs.length > 1 && !destroyed) {
	      //If this is the last tab, select the previous tab. else, the next tab.
	      var newActiveIndex = index == tabs.length - 1 ? index - 1 : index + 1;
	      ctrl.select(tabs[newActiveIndex]);
	    }
	    tabs.splice(index, 1);
	  };
	
	  var destroyed;
	  $scope.$on('$destroy', function() {
	    destroyed = true;
	  });
	}])
	
	/**
	 * @ngdoc directive
	 * @name ui.bootstrap.tabs.directive:tabset
	 * @restrict EA
	 *
	 * @description
	 * Tabset is the outer container for the tabs directive
	 *
	 * @param {boolean=} vertical Whether or not to use vertical styling for the tabs.
	 * @param {boolean=} justified Whether or not to use justified styling for the tabs.
	 *
	 * @example
	<example module="ui.bootstrap">
	  <file name="index.html">
	    <uib-tabset>
	      <uib-tab heading="Tab 1"><b>First</b> Content!</uib-tab>
	      <uib-tab heading="Tab 2"><i>Second</i> Content!</uib-tab>
	    </uib-tabset>
	    <hr />
	    <uib-tabset vertical="true">
	      <uib-tab heading="Vertical Tab 1"><b>First</b> Vertical Content!</uib-tab>
	      <uib-tab heading="Vertical Tab 2"><i>Second</i> Vertical Content!</uib-tab>
	    </uib-tabset>
	    <uib-tabset justified="true">
	      <uib-tab heading="Justified Tab 1"><b>First</b> Justified Content!</uib-tab>
	      <uib-tab heading="Justified Tab 2"><i>Second</i> Justified Content!</uib-tab>
	    </uib-tabset>
	  </file>
	</example>
	 */
	.directive('uibTabset', function() {
	  return {
	    restrict: 'EA',
	    transclude: true,
	    replace: true,
	    scope: {
	      type: '@'
	    },
	    controller: 'UibTabsetController',
	    templateUrl: 'template/tabs/tabset.html',
	    link: function(scope, element, attrs) {
	      scope.vertical = angular.isDefined(attrs.vertical) ? scope.$parent.$eval(attrs.vertical) : false;
	      scope.justified = angular.isDefined(attrs.justified) ? scope.$parent.$eval(attrs.justified) : false;
	    }
	  };
	})
	
	/**
	 * @ngdoc directive
	 * @name ui.bootstrap.tabs.directive:tab
	 * @restrict EA
	 *
	 * @param {string=} heading The visible heading, or title, of the tab. Set HTML headings with {@link ui.bootstrap.tabs.directive:tabHeading tabHeading}.
	 * @param {string=} select An expression to evaluate when the tab is selected.
	 * @param {boolean=} active A binding, telling whether or not this tab is selected.
	 * @param {boolean=} disabled A binding, telling whether or not this tab is disabled.
	 *
	 * @description
	 * Creates a tab with a heading and content. Must be placed within a {@link ui.bootstrap.tabs.directive:tabset tabset}.
	 *
	 * @example
	<example module="ui.bootstrap">
	  <file name="index.html">
	    <div ng-controller="TabsDemoCtrl">
	      <button class="btn btn-small" ng-click="items[0].active = true">
	        Select item 1, using active binding
	      </button>
	      <button class="btn btn-small" ng-click="items[1].disabled = !items[1].disabled">
	        Enable/disable item 2, using disabled binding
	      </button>
	      <br />
	      <uib-tabset>
	        <uib-tab heading="Tab 1">First Tab</uib-tab>
	        <uib-tab select="alertMe()">
	          <uib-tab-heading><i class="icon-bell"></i> Alert me!</tab-heading>
	          Second Tab, with alert callback and html heading!
	        </uib-tab>
	        <uib-tab ng-repeat="item in items"
	          heading="{{item.title}}"
	          disabled="item.disabled"
	          active="item.active">
	          {{item.content}}
	        </uib-tab>
	      </uib-tabset>
	    </div>
	  </file>
	  <file name="script.js">
	    function TabsDemoCtrl($scope) {
	      $scope.items = [
	        { title:"Dynamic Title 1", content:"Dynamic Item 0" },
	        { title:"Dynamic Title 2", content:"Dynamic Item 1", disabled: true }
	      ];
	
	      $scope.alertMe = function() {
	        setTimeout(function() {
	          alert("You've selected the alert tab!");
	        });
	      };
	    };
	  </file>
	</example>
	 */
	
	/**
	 * @ngdoc directive
	 * @name ui.bootstrap.tabs.directive:tabHeading
	 * @restrict EA
	 *
	 * @description
	 * Creates an HTML heading for a {@link ui.bootstrap.tabs.directive:tab tab}. Must be placed as a child of a tab element.
	 *
	 * @example
	<example module="ui.bootstrap">
	  <file name="index.html">
	    <uib-tabset>
	      <uib-tab>
	        <uib-tab-heading><b>HTML</b> in my titles?!</tab-heading>
	        And some content, too!
	      </uib-tab>
	      <uib-tab>
	        <uib-tab-heading><i class="icon-heart"></i> Icon heading?!?</tab-heading>
	        That's right.
	      </uib-tab>
	    </uib-tabset>
	  </file>
	</example>
	 */
	.directive('uibTab', ['$parse', function($parse) {
	  return {
	    require: '^uibTabset',
	    restrict: 'EA',
	    replace: true,
	    templateUrl: 'template/tabs/tab.html',
	    transclude: true,
	    scope: {
	      active: '=?',
	      heading: '@',
	      onSelect: '&select', //This callback is called in contentHeadingTransclude
	                          //once it inserts the tab's content into the dom
	      onDeselect: '&deselect'
	    },
	    controller: function() {
	      //Empty controller so other directives can require being 'under' a tab
	    },
	    link: function(scope, elm, attrs, tabsetCtrl, transclude) {
	      scope.$watch('active', function(active) {
	        if (active) {
	          tabsetCtrl.select(scope);
	        }
	      });
	
	      scope.disabled = false;
	      if (attrs.disable) {
	        scope.$parent.$watch($parse(attrs.disable), function(value) {
	          scope.disabled = !! value;
	        });
	      }
	
	      scope.select = function() {
	        if (!scope.disabled) {
	          scope.active = true;
	        }
	      };
	
	      tabsetCtrl.addTab(scope);
	      scope.$on('$destroy', function() {
	        tabsetCtrl.removeTab(scope);
	      });
	
	      //We need to transclude later, once the content container is ready.
	      //when this link happens, we're inside a tab heading.
	      scope.$transcludeFn = transclude;
	    }
	  };
	}])
	
	.directive('uibTabHeadingTransclude', function() {
	  return {
	    restrict: 'A',
	    require: ['?^uibTab', '?^tab'], // TODO: change to '^uibTab' after deprecation removal
	    link: function(scope, elm) {
	      scope.$watch('headingElement', function updateHeadingElement(heading) {
	        if (heading) {
	          elm.html('');
	          elm.append(heading);
	        }
	      });
	    }
	  };
	})
	
	.directive('uibTabContentTransclude', function() {
	  return {
	    restrict: 'A',
	    require: ['?^uibTabset', '?^tabset'], // TODO: change to '^uibTabset' after deprecation removal
	    link: function(scope, elm, attrs) {
	      var tab = scope.$eval(attrs.uibTabContentTransclude);
	
	      //Now our tab is ready to be transcluded: both the tab heading area
	      //and the tab content area are loaded.  Transclude 'em both.
	      tab.$transcludeFn(tab.$parent, function(contents) {
	        angular.forEach(contents, function(node) {
	          if (isTabHeading(node)) {
	            //Let tabHeadingTransclude know.
	            tab.headingElement = node;
	          } else {
	            elm.append(node);
	          }
	        });
	      });
	    }
	  };
	
	  function isTabHeading(node) {
	    return node.tagName && (
	      node.hasAttribute('tab-heading') || // TODO: remove after deprecation removal
	      node.hasAttribute('data-tab-heading') || // TODO: remove after deprecation removal
	      node.hasAttribute('x-tab-heading') || // TODO: remove after deprecation removal
	      node.hasAttribute('uib-tab-heading') ||
	      node.hasAttribute('data-uib-tab-heading') ||
	      node.hasAttribute('x-uib-tab-heading') ||
	      node.tagName.toLowerCase() === 'tab-heading' || // TODO: remove after deprecation removal
	      node.tagName.toLowerCase() === 'data-tab-heading' || // TODO: remove after deprecation removal
	      node.tagName.toLowerCase() === 'x-tab-heading' || // TODO: remove after deprecation removal
	      node.tagName.toLowerCase() === 'uib-tab-heading' ||
	      node.tagName.toLowerCase() === 'data-uib-tab-heading' ||
	      node.tagName.toLowerCase() === 'x-uib-tab-heading'
	    );
	  }
	});
	
	/* deprecated tabs below */
	
	angular.module('ui.bootstrap.tabs')
	
	  .value('$tabsSuppressWarning', false)
	
	  .controller('TabsetController', ['$scope', '$controller', '$log', '$tabsSuppressWarning', function($scope, $controller, $log, $tabsSuppressWarning) {
	    if (!$tabsSuppressWarning) {
	      $log.warn('TabsetController is now deprecated. Use UibTabsetController instead.');
	    }
	
	    angular.extend(this, $controller('UibTabsetController', {
	      $scope: $scope
	    }));
	  }])
	
	  .directive('tabset', ['$log', '$tabsSuppressWarning', function($log, $tabsSuppressWarning) {
	    return {
	      restrict: 'EA',
	      transclude: true,
	      replace: true,
	      scope: {
	        type: '@'
	      },
	      controller: 'TabsetController',
	      templateUrl: 'template/tabs/tabset.html',
	      link: function(scope, element, attrs) {
	
	        if (!$tabsSuppressWarning) {
	          $log.warn('tabset is now deprecated. Use uib-tabset instead.');
	        }
	        scope.vertical = angular.isDefined(attrs.vertical) ? scope.$parent.$eval(attrs.vertical) : false;
	        scope.justified = angular.isDefined(attrs.justified) ? scope.$parent.$eval(attrs.justified) : false;
	      }
	    };
	  }])
	
	  .directive('tab', ['$parse', '$log', '$tabsSuppressWarning', function($parse, $log, $tabsSuppressWarning) {
	    return {
	      require: '^tabset',
	      restrict: 'EA',
	      replace: true,
	      templateUrl: 'template/tabs/tab.html',
	      transclude: true,
	      scope: {
	        active: '=?',
	        heading: '@',
	        onSelect: '&select', //This callback is called in contentHeadingTransclude
	        //once it inserts the tab's content into the dom
	        onDeselect: '&deselect'
	      },
	      controller: function() {
	        //Empty controller so other directives can require being 'under' a tab
	      },
	      link: function(scope, elm, attrs, tabsetCtrl, transclude) {
	        if (!$tabsSuppressWarning) {
	          $log.warn('tab is now deprecated. Use uib-tab instead.');
	        }
	
	        scope.$watch('active', function(active) {
	          if (active) {
	            tabsetCtrl.select(scope);
	          }
	        });
	
	        scope.disabled = false;
	        if (attrs.disable) {
	          scope.$parent.$watch($parse(attrs.disable), function(value) {
	            scope.disabled = !!value;
	          });
	        }
	
	        scope.select = function() {
	          if (!scope.disabled) {
	            scope.active = true;
	          }
	        };
	
	        tabsetCtrl.addTab(scope);
	        scope.$on('$destroy', function() {
	          tabsetCtrl.removeTab(scope);
	        });
	
	        //We need to transclude later, once the content container is ready.
	        //when this link happens, we're inside a tab heading.
	        scope.$transcludeFn = transclude;
	      }
	    };
	  }])
	
	  .directive('tabHeadingTransclude', ['$log', '$tabsSuppressWarning', function($log, $tabsSuppressWarning) {
	    return {
	      restrict: 'A',
	      require: '^tab',
	      link: function(scope, elm) {
	        if (!$tabsSuppressWarning) {
	          $log.warn('tab-heading-transclude is now deprecated. Use uib-tab-heading-transclude instead.');
	        }
	
	        scope.$watch('headingElement', function updateHeadingElement(heading) {
	          if (heading) {
	            elm.html('');
	            elm.append(heading);
	          }
	        });
	      }
	    };
	  }])
	
	  .directive('tabContentTransclude', ['$log', '$tabsSuppressWarning', function($log, $tabsSuppressWarning) {
	    return {
	      restrict: 'A',
	      require: '^tabset',
	      link: function(scope, elm, attrs) {
	        if (!$tabsSuppressWarning) {
	          $log.warn('tab-content-transclude is now deprecated. Use uib-tab-content-transclude instead.');
	        }
	
	        var tab = scope.$eval(attrs.tabContentTransclude);
	
	        //Now our tab is ready to be transcluded: both the tab heading area
	        //and the tab content area are loaded.  Transclude 'em both.
	        tab.$transcludeFn(tab.$parent, function(contents) {
	          angular.forEach(contents, function(node) {
	            if (isTabHeading(node)) {
	              //Let tabHeadingTransclude know.
	              tab.headingElement = node;
	            }
	            else {
	              elm.append(node);
	            }
	          });
	        });
	      }
	    };
	
	    function isTabHeading(node) {
	      return node.tagName && (
	          node.hasAttribute('tab-heading') ||
	          node.hasAttribute('data-tab-heading') ||
	          node.hasAttribute('x-tab-heading') ||
	          node.tagName.toLowerCase() === 'tab-heading' ||
	          node.tagName.toLowerCase() === 'data-tab-heading' ||
	          node.tagName.toLowerCase() === 'x-tab-heading'
	        );
	    }
	  }]);
	
	angular.module('ui.bootstrap.timepicker', [])
	
	.constant('uibTimepickerConfig', {
	  hourStep: 1,
	  minuteStep: 1,
	  showMeridian: true,
	  meridians: null,
	  readonlyInput: false,
	  mousewheel: true,
	  arrowkeys: true,
	  showSpinners: true
	})
	
	.controller('UibTimepickerController', ['$scope', '$element', '$attrs', '$parse', '$log', '$locale', 'uibTimepickerConfig', function($scope, $element, $attrs, $parse, $log, $locale, timepickerConfig) {
	  var selected = new Date(),
	      ngModelCtrl = { $setViewValue: angular.noop }, // nullModelCtrl
	      meridians = angular.isDefined($attrs.meridians) ? $scope.$parent.$eval($attrs.meridians) : timepickerConfig.meridians || $locale.DATETIME_FORMATS.AMPMS;
	
	  $scope.tabindex = angular.isDefined($attrs.tabindex) ? $attrs.tabindex : 0;
	  $element.removeAttr('tabindex');
	
	  this.init = function(ngModelCtrl_, inputs) {
	    ngModelCtrl = ngModelCtrl_;
	    ngModelCtrl.$render = this.render;
	
	    ngModelCtrl.$formatters.unshift(function(modelValue) {
	      return modelValue ? new Date(modelValue) : null;
	    });
	
	    var hoursInputEl = inputs.eq(0),
	        minutesInputEl = inputs.eq(1);
	
	    var mousewheel = angular.isDefined($attrs.mousewheel) ? $scope.$parent.$eval($attrs.mousewheel) : timepickerConfig.mousewheel;
	    if (mousewheel) {
	      this.setupMousewheelEvents(hoursInputEl, minutesInputEl);
	    }
	
	    var arrowkeys = angular.isDefined($attrs.arrowkeys) ? $scope.$parent.$eval($attrs.arrowkeys) : timepickerConfig.arrowkeys;
	    if (arrowkeys) {
	      this.setupArrowkeyEvents(hoursInputEl, minutesInputEl);
	    }
	
	    $scope.readonlyInput = angular.isDefined($attrs.readonlyInput) ? $scope.$parent.$eval($attrs.readonlyInput) : timepickerConfig.readonlyInput;
	    this.setupInputEvents(hoursInputEl, minutesInputEl);
	  };
	
	  var hourStep = timepickerConfig.hourStep;
	  if ($attrs.hourStep) {
	    $scope.$parent.$watch($parse($attrs.hourStep), function(value) {
	      hourStep = parseInt(value, 10);
	    });
	  }
	
	  var minuteStep = timepickerConfig.minuteStep;
	  if ($attrs.minuteStep) {
	    $scope.$parent.$watch($parse($attrs.minuteStep), function(value) {
	      minuteStep = parseInt(value, 10);
	    });
	  }
	
	  var min;
	  $scope.$parent.$watch($parse($attrs.min), function(value) {
	    var dt = new Date(value);
	    min = isNaN(dt) ? undefined : dt;
	  });
	
	  var max;
	  $scope.$parent.$watch($parse($attrs.max), function(value) {
	    var dt = new Date(value);
	    max = isNaN(dt) ? undefined : dt;
	  });
	
	  $scope.noIncrementHours = function() {
	    var incrementedSelected = addMinutes(selected, hourStep * 60);
	    return incrementedSelected > max ||
	      (incrementedSelected < selected && incrementedSelected < min);
	  };
	
	  $scope.noDecrementHours = function() {
	    var decrementedSelected = addMinutes(selected, -hourStep * 60);
	    return decrementedSelected < min ||
	      (decrementedSelected > selected && decrementedSelected > max);
	  };
	
	  $scope.noIncrementMinutes = function() {
	    var incrementedSelected = addMinutes(selected, minuteStep);
	    return incrementedSelected > max ||
	      (incrementedSelected < selected && incrementedSelected < min);
	  };
	
	  $scope.noDecrementMinutes = function() {
	    var decrementedSelected = addMinutes(selected, -minuteStep);
	    return decrementedSelected < min ||
	      (decrementedSelected > selected && decrementedSelected > max);
	  };
	
	  $scope.noToggleMeridian = function() {
	    if (selected.getHours() < 13) {
	      return addMinutes(selected, 12 * 60) > max;
	    } else {
	      return addMinutes(selected, -12 * 60) < min;
	    }
	  };
	
	  // 12H / 24H mode
	  $scope.showMeridian = timepickerConfig.showMeridian;
	  if ($attrs.showMeridian) {
	    $scope.$parent.$watch($parse($attrs.showMeridian), function(value) {
	      $scope.showMeridian = !!value;
	
	      if (ngModelCtrl.$error.time) {
	        // Evaluate from template
	        var hours = getHoursFromTemplate(), minutes = getMinutesFromTemplate();
	        if (angular.isDefined(hours) && angular.isDefined(minutes)) {
	          selected.setHours(hours);
	          refresh();
	        }
	      } else {
	        updateTemplate();
	      }
	    });
	  }
	
	  // Get $scope.hours in 24H mode if valid
	  function getHoursFromTemplate() {
	    var hours = parseInt($scope.hours, 10);
	    var valid = $scope.showMeridian ? (hours > 0 && hours < 13) : (hours >= 0 && hours < 24);
	    if (!valid) {
	      return undefined;
	    }
	
	    if ($scope.showMeridian) {
	      if (hours === 12) {
	        hours = 0;
	      }
	      if ($scope.meridian === meridians[1]) {
	        hours = hours + 12;
	      }
	    }
	    return hours;
	  }
	
	  function getMinutesFromTemplate() {
	    var minutes = parseInt($scope.minutes, 10);
	    return (minutes >= 0 && minutes < 60) ? minutes : undefined;
	  }
	
	  function pad(value) {
	    return (angular.isDefined(value) && value.toString().length < 2) ? '0' + value : value.toString();
	  }
	
	  // Respond on mousewheel spin
	  this.setupMousewheelEvents = function(hoursInputEl, minutesInputEl) {
	    var isScrollingUp = function(e) {
	      if (e.originalEvent) {
	        e = e.originalEvent;
	      }
	      //pick correct delta variable depending on event
	      var delta = (e.wheelDelta) ? e.wheelDelta : -e.deltaY;
	      return (e.detail || delta > 0);
	    };
	
	    hoursInputEl.bind('mousewheel wheel', function(e) {
	      $scope.$apply(isScrollingUp(e) ? $scope.incrementHours() : $scope.decrementHours());
	      e.preventDefault();
	    });
	
	    minutesInputEl.bind('mousewheel wheel', function(e) {
	      $scope.$apply(isScrollingUp(e) ? $scope.incrementMinutes() : $scope.decrementMinutes());
	      e.preventDefault();
	    });
	
	  };
	
	  // Respond on up/down arrowkeys
	  this.setupArrowkeyEvents = function(hoursInputEl, minutesInputEl) {
	    hoursInputEl.bind('keydown', function(e) {
	      if (e.which === 38) { // up
	        e.preventDefault();
	        $scope.incrementHours();
	        $scope.$apply();
	      } else if (e.which === 40) { // down
	        e.preventDefault();
	        $scope.decrementHours();
	        $scope.$apply();
	      }
	    });
	
	    minutesInputEl.bind('keydown', function(e) {
	      if (e.which === 38) { // up
	        e.preventDefault();
	        $scope.incrementMinutes();
	        $scope.$apply();
	      } else if (e.which === 40) { // down
	        e.preventDefault();
	        $scope.decrementMinutes();
	        $scope.$apply();
	      }
	    });
	  };
	
	  this.setupInputEvents = function(hoursInputEl, minutesInputEl) {
	    if ($scope.readonlyInput) {
	      $scope.updateHours = angular.noop;
	      $scope.updateMinutes = angular.noop;
	      return;
	    }
	
	    var invalidate = function(invalidHours, invalidMinutes) {
	      ngModelCtrl.$setViewValue(null);
	      ngModelCtrl.$setValidity('time', false);
	      if (angular.isDefined(invalidHours)) {
	        $scope.invalidHours = invalidHours;
	      }
	      if (angular.isDefined(invalidMinutes)) {
	        $scope.invalidMinutes = invalidMinutes;
	      }
	    };
	
	    $scope.updateHours = function() {
	      var hours = getHoursFromTemplate(),
	        minutes = getMinutesFromTemplate();
	
	      if (angular.isDefined(hours) && angular.isDefined(minutes)) {
	        selected.setHours(hours);
	        if (selected < min || selected > max) {
	          invalidate(true);
	        } else {
	          refresh('h');
	        }
	      } else {
	        invalidate(true);
	      }
	    };
	
	    hoursInputEl.bind('blur', function(e) {
	      if (!$scope.invalidHours && $scope.hours < 10) {
	        $scope.$apply(function() {
	          $scope.hours = pad($scope.hours);
	        });
	      }
	    });
	
	    $scope.updateMinutes = function() {
	      var minutes = getMinutesFromTemplate(),
	        hours = getHoursFromTemplate();
	
	      if (angular.isDefined(minutes) && angular.isDefined(hours)) {
	        selected.setMinutes(minutes);
	        if (selected < min || selected > max) {
	          invalidate(undefined, true);
	        } else {
	          refresh('m');
	        }
	      } else {
	        invalidate(undefined, true);
	      }
	    };
	
	    minutesInputEl.bind('blur', function(e) {
	      if (!$scope.invalidMinutes && $scope.minutes < 10) {
	        $scope.$apply(function() {
	          $scope.minutes = pad($scope.minutes);
	        });
	      }
	    });
	
	  };
	
	  this.render = function() {
	    var date = ngModelCtrl.$viewValue;
	
	    if (isNaN(date)) {
	      ngModelCtrl.$setValidity('time', false);
	      $log.error('Timepicker directive: "ng-model" value must be a Date object, a number of milliseconds since 01.01.1970 or a string representing an RFC2822 or ISO 8601 date.');
	    } else {
	      if (date) {
	        selected = date;
	      }
	
	      if (selected < min || selected > max) {
	        ngModelCtrl.$setValidity('time', false);
	        $scope.invalidHours = true;
	        $scope.invalidMinutes = true;
	      } else {
	        makeValid();
	      }
	      updateTemplate();
	    }
	  };
	
	  // Call internally when we know that model is valid.
	  function refresh(keyboardChange) {
	    makeValid();
	    ngModelCtrl.$setViewValue(new Date(selected));
	    updateTemplate(keyboardChange);
	  }
	
	  function makeValid() {
	    ngModelCtrl.$setValidity('time', true);
	    $scope.invalidHours = false;
	    $scope.invalidMinutes = false;
	  }
	
	  function updateTemplate(keyboardChange) {
	    var hours = selected.getHours(), minutes = selected.getMinutes();
	
	    if ($scope.showMeridian) {
	      hours = (hours === 0 || hours === 12) ? 12 : hours % 12; // Convert 24 to 12 hour system
	    }
	
	    $scope.hours = keyboardChange === 'h' ? hours : pad(hours);
	    if (keyboardChange !== 'm') {
	      $scope.minutes = pad(minutes);
	    }
	    $scope.meridian = selected.getHours() < 12 ? meridians[0] : meridians[1];
	  }
	
	  function addMinutes(date, minutes) {
	    var dt = new Date(date.getTime() + minutes * 60000);
	    var newDate = new Date(date);
	    newDate.setHours(dt.getHours(), dt.getMinutes());
	    return newDate;
	  }
	
	  function addMinutesToSelected(minutes) {
	    selected = addMinutes(selected, minutes);
	    refresh();
	  }
	
	  $scope.showSpinners = angular.isDefined($attrs.showSpinners) ?
	    $scope.$parent.$eval($attrs.showSpinners) : timepickerConfig.showSpinners;
	
	  $scope.incrementHours = function() {
	    if (!$scope.noIncrementHours()) {
	      addMinutesToSelected(hourStep * 60);
	    }
	  };
	
	  $scope.decrementHours = function() {
	    if (!$scope.noDecrementHours()) {
	      addMinutesToSelected(-hourStep * 60);
	    }
	  };
	
	  $scope.incrementMinutes = function() {
	    if (!$scope.noIncrementMinutes()) {
	      addMinutesToSelected(minuteStep);
	    }
	  };
	
	  $scope.decrementMinutes = function() {
	    if (!$scope.noDecrementMinutes()) {
	      addMinutesToSelected(-minuteStep);
	    }
	  };
	
	  $scope.toggleMeridian = function() {
	    if (!$scope.noToggleMeridian()) {
	      addMinutesToSelected(12 * 60 * (selected.getHours() < 12 ? 1 : -1));
	    }
	  };
	}])
	
	.directive('uibTimepicker', function() {
	  return {
	    restrict: 'EA',
	    require: ['uibTimepicker', '?^ngModel'],
	    controller: 'UibTimepickerController',
	    controllerAs: 'timepicker',
	    replace: true,
	    scope: {},
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/timepicker/timepicker.html';
	    },
	    link: function(scope, element, attrs, ctrls) {
	      var timepickerCtrl = ctrls[0], ngModelCtrl = ctrls[1];
	
	      if (ngModelCtrl) {
	        timepickerCtrl.init(ngModelCtrl, element.find('input'));
	      }
	    }
	  };
	});
	
	/* Deprecated timepicker below */
	
	angular.module('ui.bootstrap.timepicker')
	
	.value('$timepickerSuppressWarning', false)
	
	.controller('TimepickerController', ['$scope', '$element', '$attrs', '$controller', '$log', '$timepickerSuppressWarning', function($scope, $element, $attrs, $controller, $log, $timepickerSuppressWarning) {
	  if (!$timepickerSuppressWarning) {
	    $log.warn('TimepickerController is now deprecated. Use UibTimepickerController instead.');
	  }
	
	  angular.extend(this, $controller('UibTimepickerController', {
	    $scope: $scope,
	    $element: $element,
	    $attrs: $attrs
	  }));
	}])
	
	.directive('timepicker', ['$log', '$timepickerSuppressWarning', function($log, $timepickerSuppressWarning) {
	  return {
	    restrict: 'EA',
	    require: ['timepicker', '?^ngModel'],
	    controller: 'TimepickerController',
	    controllerAs: 'timepicker',
	    replace: true,
	    scope: {},
	    templateUrl: function(element, attrs) {
	      return attrs.templateUrl || 'template/timepicker/timepicker.html';
	    },
	    link: function(scope, element, attrs, ctrls) {
	      if (!$timepickerSuppressWarning) {
	        $log.warn('timepicker is now deprecated. Use uib-timepicker instead.');
	      }
	      var timepickerCtrl = ctrls[0], ngModelCtrl = ctrls[1];
	
	      if (ngModelCtrl) {
	        timepickerCtrl.init(ngModelCtrl, element.find('input'));
	      }
	    }
	  };
	}]);
	
	angular.module('ui.bootstrap.typeahead', ['ui.bootstrap.position'])
	
	/**
	 * A helper service that can parse typeahead's syntax (string provided by users)
	 * Extracted to a separate service for ease of unit testing
	 */
	  .factory('uibTypeaheadParser', ['$parse', function($parse) {
	    //                      00000111000000000000022200000000000000003333333333333330000000000044000
	    var TYPEAHEAD_REGEXP = /^\s*([\s\S]+?)(?:\s+as\s+([\s\S]+?))?\s+for\s+(?:([\$\w][\$\w\d]*))\s+in\s+([\s\S]+?)$/;
	    return {
	      parse: function(input) {
	        var match = input.match(TYPEAHEAD_REGEXP);
	        if (!match) {
	          throw new Error(
	            'Expected typeahead specification in form of "_modelValue_ (as _label_)? for _item_ in _collection_"' +
	              ' but got "' + input + '".');
	        }
	
	        return {
	          itemName: match[3],
	          source: $parse(match[4]),
	          viewMapper: $parse(match[2] || match[1]),
	          modelMapper: $parse(match[1])
	        };
	      }
	    };
	  }])
	
	  .controller('UibTypeaheadController', ['$scope', '$element', '$attrs', '$compile', '$parse', '$q', '$timeout', '$document', '$window', '$rootScope', '$uibPosition', 'uibTypeaheadParser',
	    function(originalScope, element, attrs, $compile, $parse, $q, $timeout, $document, $window, $rootScope, $position, typeaheadParser) {
	    var HOT_KEYS = [9, 13, 27, 38, 40];
	    var eventDebounceTime = 200;
	    var modelCtrl, ngModelOptions;
	    //SUPPORTED ATTRIBUTES (OPTIONS)
	
	    //minimal no of characters that needs to be entered before typeahead kicks-in
	    var minLength = originalScope.$eval(attrs.typeaheadMinLength);
	    if (!minLength && minLength !== 0) {
	      minLength = 1;
	    }
	
	    //minimal wait time after last character typed before typeahead kicks-in
	    var waitTime = originalScope.$eval(attrs.typeaheadWaitMs) || 0;
	
	    //should it restrict model values to the ones selected from the popup only?
	    var isEditable = originalScope.$eval(attrs.typeaheadEditable) !== false;
	
	    //binding to a variable that indicates if matches are being retrieved asynchronously
	    var isLoadingSetter = $parse(attrs.typeaheadLoading).assign || angular.noop;
	
	    //a callback executed when a match is selected
	    var onSelectCallback = $parse(attrs.typeaheadOnSelect);
	
	    //should it select highlighted popup value when losing focus?
	    var isSelectOnBlur = angular.isDefined(attrs.typeaheadSelectOnBlur) ? originalScope.$eval(attrs.typeaheadSelectOnBlur) : false;
	
	    //binding to a variable that indicates if there were no results after the query is completed
	    var isNoResultsSetter = $parse(attrs.typeaheadNoResults).assign || angular.noop;
	
	    var inputFormatter = attrs.typeaheadInputFormatter ? $parse(attrs.typeaheadInputFormatter) : undefined;
	
	    var appendToBody =  attrs.typeaheadAppendToBody ? originalScope.$eval(attrs.typeaheadAppendToBody) : false;
	
	    var appendToElementId =  attrs.typeaheadAppendToElementId || false;
	
	    var focusFirst = originalScope.$eval(attrs.typeaheadFocusFirst) !== false;
	
	    //If input matches an item of the list exactly, select it automatically
	    var selectOnExact = attrs.typeaheadSelectOnExact ? originalScope.$eval(attrs.typeaheadSelectOnExact) : false;
	
	    //INTERNAL VARIABLES
	
	    //model setter executed upon match selection
	    var parsedModel = $parse(attrs.ngModel);
	    var invokeModelSetter = $parse(attrs.ngModel + '($$$p)');
	    var $setModelValue = function(scope, newValue) {
	      if (angular.isFunction(parsedModel(originalScope)) &&
	        ngModelOptions && ngModelOptions.$options && ngModelOptions.$options.getterSetter) {
	        return invokeModelSetter(scope, {$$$p: newValue});
	      } else {
	        return parsedModel.assign(scope, newValue);
	      }
	    };
	
	    //expressions used by typeahead
	    var parserResult = typeaheadParser.parse(attrs.uibTypeahead);
	
	    var hasFocus;
	
	    //Used to avoid bug in iOS webview where iOS keyboard does not fire
	    //mousedown & mouseup events
	    //Issue #3699
	    var selected;
	
	    //create a child scope for the typeahead directive so we are not polluting original scope
	    //with typeahead-specific data (matches, query etc.)
	    var scope = originalScope.$new();
	    var offDestroy = originalScope.$on('$destroy', function() {
	      scope.$destroy();
	    });
	    scope.$on('$destroy', offDestroy);
	
	    // WAI-ARIA
	    var popupId = 'typeahead-' + scope.$id + '-' + Math.floor(Math.random() * 10000);
	    element.attr({
	      'aria-autocomplete': 'list',
	      'aria-expanded': false,
	      'aria-owns': popupId
	    });
	
	    //pop-up element used to display matches
	    var popUpEl = angular.element('<div uib-typeahead-popup></div>');
	    popUpEl.attr({
	      id: popupId,
	      matches: 'matches',
	      active: 'activeIdx',
	      select: 'select(activeIdx)',
	      'move-in-progress': 'moveInProgress',
	      query: 'query',
	      position: 'position'
	    });
	    //custom item template
	    if (angular.isDefined(attrs.typeaheadTemplateUrl)) {
	      popUpEl.attr('template-url', attrs.typeaheadTemplateUrl);
	    }
	
	    if (angular.isDefined(attrs.typeaheadPopupTemplateUrl)) {
	      popUpEl.attr('popup-template-url', attrs.typeaheadPopupTemplateUrl);
	    }
	
	    var resetMatches = function() {
	      scope.matches = [];
	      scope.activeIdx = -1;
	      element.attr('aria-expanded', false);
	    };
	
	    var getMatchId = function(index) {
	      return popupId + '-option-' + index;
	    };
	
	    // Indicate that the specified match is the active (pre-selected) item in the list owned by this typeahead.
	    // This attribute is added or removed automatically when the `activeIdx` changes.
	    scope.$watch('activeIdx', function(index) {
	      if (index < 0) {
	        element.removeAttr('aria-activedescendant');
	      } else {
	        element.attr('aria-activedescendant', getMatchId(index));
	      }
	    });
	
	    var inputIsExactMatch = function(inputValue, index) {
	      if (scope.matches.length > index && inputValue) {
	        return inputValue.toUpperCase() === scope.matches[index].label.toUpperCase();
	      }
	
	      return false;
	    };
	
	    var getMatchesAsync = function(inputValue) {
	      var locals = {$viewValue: inputValue};
	      isLoadingSetter(originalScope, true);
	      isNoResultsSetter(originalScope, false);
	      $q.when(parserResult.source(originalScope, locals)).then(function(matches) {
	        //it might happen that several async queries were in progress if a user were typing fast
	        //but we are interested only in responses that correspond to the current view value
	        var onCurrentRequest = (inputValue === modelCtrl.$viewValue);
	        if (onCurrentRequest && hasFocus) {
	          if (matches && matches.length > 0) {
	            scope.activeIdx = focusFirst ? 0 : -1;
	            isNoResultsSetter(originalScope, false);
	            scope.matches.length = 0;
	
	            //transform labels
	            for (var i = 0; i < matches.length; i++) {
	              locals[parserResult.itemName] = matches[i];
	              scope.matches.push({
	                id: getMatchId(i),
	                label: parserResult.viewMapper(scope, locals),
	                model: matches[i]
	              });
	            }
	
	            scope.query = inputValue;
	            //position pop-up with matches - we need to re-calculate its position each time we are opening a window
	            //with matches as a pop-up might be absolute-positioned and position of an input might have changed on a page
	            //due to other elements being rendered
	            recalculatePosition();
	
	            element.attr('aria-expanded', true);
	
	            //Select the single remaining option if user input matches
	            if (selectOnExact && scope.matches.length === 1 && inputIsExactMatch(inputValue, 0)) {
	              scope.select(0);
	            }
	          } else {
	            resetMatches();
	            isNoResultsSetter(originalScope, true);
	          }
	        }
	        if (onCurrentRequest) {
	          isLoadingSetter(originalScope, false);
	        }
	      }, function() {
	        resetMatches();
	        isLoadingSetter(originalScope, false);
	        isNoResultsSetter(originalScope, true);
	      });
	    };
	
	    // bind events only if appendToBody params exist - performance feature
	    if (appendToBody) {
	      angular.element($window).bind('resize', fireRecalculating);
	      $document.find('body').bind('scroll', fireRecalculating);
	    }
	
	    // Declare the timeout promise var outside the function scope so that stacked calls can be cancelled later
	    var timeoutEventPromise;
	
	    // Default progress type
	    scope.moveInProgress = false;
	
	    function fireRecalculating() {
	      if (!scope.moveInProgress) {
	        scope.moveInProgress = true;
	        scope.$digest();
	      }
	
	      // Cancel previous timeout
	      if (timeoutEventPromise) {
	        $timeout.cancel(timeoutEventPromise);
	      }
	
	      // Debounced executing recalculate after events fired
	      timeoutEventPromise = $timeout(function() {
	        // if popup is visible
	        if (scope.matches.length) {
	          recalculatePosition();
	        }
	
	        scope.moveInProgress = false;
	      }, eventDebounceTime);
	    }
	
	    // recalculate actual position and set new values to scope
	    // after digest loop is popup in right position
	    function recalculatePosition() {
	      scope.position = appendToBody ? $position.offset(element) : $position.position(element);
	      scope.position.top += element.prop('offsetHeight');
	    }
	
	    //we need to propagate user's query so we can higlight matches
	    scope.query = undefined;
	
	    //Declare the timeout promise var outside the function scope so that stacked calls can be cancelled later
	    var timeoutPromise;
	
	    var scheduleSearchWithTimeout = function(inputValue) {
	      timeoutPromise = $timeout(function() {
	        getMatchesAsync(inputValue);
	      }, waitTime);
	    };
	
	    var cancelPreviousTimeout = function() {
	      if (timeoutPromise) {
	        $timeout.cancel(timeoutPromise);
	      }
	    };
	
	    resetMatches();
	
	    scope.select = function(activeIdx) {
	      //called from within the $digest() cycle
	      var locals = {};
	      var model, item;
	
	      selected = true;
	      locals[parserResult.itemName] = item = scope.matches[activeIdx].model;
	      model = parserResult.modelMapper(originalScope, locals);
	      $setModelValue(originalScope, model);
	      modelCtrl.$setValidity('editable', true);
	      modelCtrl.$setValidity('parse', true);
	
	      onSelectCallback(originalScope, {
	        $item: item,
	        $model: model,
	        $label: parserResult.viewMapper(originalScope, locals)
	      });
	
	      resetMatches();
	
	      //return focus to the input element if a match was selected via a mouse click event
	      // use timeout to avoid $rootScope:inprog error
	      if (scope.$eval(attrs.typeaheadFocusOnSelect) !== false) {
	        $timeout(function() { element[0].focus(); }, 0, false);
	      }
	    };
	
	    //bind keyboard events: arrows up(38) / down(40), enter(13) and tab(9), esc(27)
	    element.bind('keydown', function(evt) {
	      //typeahead is open and an "interesting" key was pressed
	      if (scope.matches.length === 0 || HOT_KEYS.indexOf(evt.which) === -1) {
	        return;
	      }
	
	      // if there's nothing selected (i.e. focusFirst) and enter or tab is hit, clear the results
	      if (scope.activeIdx === -1 && (evt.which === 9 || evt.which === 13)) {
	        resetMatches();
	        scope.$digest();
	        return;
	      }
	
	      evt.preventDefault();
	
	      if (evt.which === 40) {
	        scope.activeIdx = (scope.activeIdx + 1) % scope.matches.length;
	        scope.$digest();
	      } else if (evt.which === 38) {
	        scope.activeIdx = (scope.activeIdx > 0 ? scope.activeIdx : scope.matches.length) - 1;
	        scope.$digest();
	      } else if (evt.which === 13 || evt.which === 9) {
	        scope.$apply(function () {
	          scope.select(scope.activeIdx);
	        });
	      } else if (evt.which === 27) {
	        evt.stopPropagation();
	
	        resetMatches();
	        scope.$digest();
	      }
	    });
	
	    element.bind('blur', function() {
	      if (isSelectOnBlur && scope.matches.length && scope.activeIdx !== -1 && !selected) {
	        selected = true;
	        scope.$apply(function() {
	          scope.select(scope.activeIdx);
	        });
	      }
	      hasFocus = false;
	      selected = false;
	    });
	
	    // Keep reference to click handler to unbind it.
	    var dismissClickHandler = function(evt) {
	      // Issue #3973
	      // Firefox treats right click as a click on document
	      if (element[0] !== evt.target && evt.which !== 3 && scope.matches.length !== 0) {
	        resetMatches();
	        if (!$rootScope.$$phase) {
	          scope.$digest();
	        }
	      }
	    };
	
	    $document.bind('click', dismissClickHandler);
	
	    originalScope.$on('$destroy', function() {
	      $document.unbind('click', dismissClickHandler);
	      if (appendToBody || appendToElementId) {
	        $popup.remove();
	      }
	
	      if (appendToBody) {
	        angular.element($window).unbind('resize', fireRecalculating);
	        $document.find('body').unbind('scroll', fireRecalculating);
	      }
	      // Prevent jQuery cache memory leak
	      popUpEl.remove();
	    });
	
	    var $popup = $compile(popUpEl)(scope);
	
	    if (appendToBody) {
	      $document.find('body').append($popup);
	    } else if (appendToElementId !== false) {
	      angular.element($document[0].getElementById(appendToElementId)).append($popup);
	    } else {
	      element.after($popup);
	    }
	
	    this.init = function(_modelCtrl, _ngModelOptions) {
	      modelCtrl = _modelCtrl;
	      ngModelOptions = _ngModelOptions;
	
	      //plug into $parsers pipeline to open a typeahead on view changes initiated from DOM
	      //$parsers kick-in on all the changes coming from the view as well as manually triggered by $setViewValue
	      modelCtrl.$parsers.unshift(function(inputValue) {
	        hasFocus = true;
	
	        if (minLength === 0 || inputValue && inputValue.length >= minLength) {
	          if (waitTime > 0) {
	            cancelPreviousTimeout();
	            scheduleSearchWithTimeout(inputValue);
	          } else {
	            getMatchesAsync(inputValue);
	          }
	        } else {
	          isLoadingSetter(originalScope, false);
	          cancelPreviousTimeout();
	          resetMatches();
	        }
	
	        if (isEditable) {
	          return inputValue;
	        } else {
	          if (!inputValue) {
	            // Reset in case user had typed something previously.
	            modelCtrl.$setValidity('editable', true);
	            return null;
	          } else {
	            modelCtrl.$setValidity('editable', false);
	            return undefined;
	          }
	        }
	      });
	
	      modelCtrl.$formatters.push(function(modelValue) {
	        var candidateViewValue, emptyViewValue;
	        var locals = {};
	
	        // The validity may be set to false via $parsers (see above) if
	        // the model is restricted to selected values. If the model
	        // is set manually it is considered to be valid.
	        if (!isEditable) {
	          modelCtrl.$setValidity('editable', true);
	        }
	
	        if (inputFormatter) {
	          locals.$model = modelValue;
	          return inputFormatter(originalScope, locals);
	        } else {
	          //it might happen that we don't have enough info to properly render input value
	          //we need to check for this situation and simply return model value if we can't apply custom formatting
	          locals[parserResult.itemName] = modelValue;
	          candidateViewValue = parserResult.viewMapper(originalScope, locals);
	          locals[parserResult.itemName] = undefined;
	          emptyViewValue = parserResult.viewMapper(originalScope, locals);
	
	          return candidateViewValue !== emptyViewValue ? candidateViewValue : modelValue;
	        }
	      });
	    };
	  }])
	
	  .directive('uibTypeahead', function() {
	    return {
	      controller: 'UibTypeaheadController',
	      require: ['ngModel', '^?ngModelOptions', 'uibTypeahead'],
	      link: function(originalScope, element, attrs, ctrls) {
	        ctrls[2].init(ctrls[0], ctrls[1]);
	      }
	    };
	  })
	
	  .directive('uibTypeaheadPopup', function() {
	    return {
	      scope: {
	        matches: '=',
	        query: '=',
	        active: '=',
	        position: '&',
	        moveInProgress: '=',
	        select: '&'
	      },
	      replace: true,
	      templateUrl: function(element, attrs) {
	        return attrs.popupTemplateUrl || 'template/typeahead/typeahead-popup.html';
	      },
	      link: function(scope, element, attrs) {
	        scope.templateUrl = attrs.templateUrl;
	
	        scope.isOpen = function() {
	          return scope.matches.length > 0;
	        };
	
	        scope.isActive = function(matchIdx) {
	          return scope.active == matchIdx;
	        };
	
	        scope.selectActive = function(matchIdx) {
	          scope.active = matchIdx;
	        };
	
	        scope.selectMatch = function(activeIdx) {
	          scope.select({activeIdx:activeIdx});
	        };
	      }
	    };
	  })
	
	  .directive('uibTypeaheadMatch', ['$templateRequest', '$compile', '$parse', function($templateRequest, $compile, $parse) {
	    return {
	      scope: {
	        index: '=',
	        match: '=',
	        query: '='
	      },
	      link:function(scope, element, attrs) {
	        var tplUrl = $parse(attrs.templateUrl)(scope.$parent) || 'template/typeahead/typeahead-match.html';
	        $templateRequest(tplUrl).then(function(tplContent) {
	          $compile(tplContent.trim())(scope, function(clonedElement) {
	            element.replaceWith(clonedElement);
	          });
	        });
	      }
	    };
	  }])
	
	  .filter('uibTypeaheadHighlight', ['$sce', '$injector', '$log', function($sce, $injector, $log) {
	    var isSanitizePresent;
	    isSanitizePresent = $injector.has('$sanitize');
	
	    function escapeRegexp(queryToEscape) {
	      // Regex: capture the whole query string and replace it with the string that will be used to match
	      // the results, for example if the capture is "a" the result will be \a
	      return queryToEscape.replace(/([.?*+^$[\]\\(){}|-])/g, '\\$1');
	    }
	
	    function containsHtml(matchItem) {
	      return /<.*>/g.test(matchItem);
	    }
	
	    return function(matchItem, query) {
	      if (!isSanitizePresent && containsHtml(matchItem)) {
	        $log.warn('Unsafe use of typeahead please use ngSanitize'); // Warn the user about the danger
	      }
	      matchItem = query? ('' + matchItem).replace(new RegExp(escapeRegexp(query), 'gi'), '<strong>$&</strong>') : matchItem; // Replaces the capture string with a the same string inside of a "strong" tag
	      if (!isSanitizePresent) {
	        matchItem = $sce.trustAsHtml(matchItem); // If $sanitize is not present we pack the string in a $sce object for the ng-bind-html directive
	      }
	      return matchItem;
	    };
	  }]);
	
	/* Deprecated typeahead below */
	  
	angular.module('ui.bootstrap.typeahead')
	  .value('$typeaheadSuppressWarning', false)
	  .service('typeaheadParser', ['$parse', 'uibTypeaheadParser', '$log', '$typeaheadSuppressWarning', function($parse, uibTypeaheadParser, $log, $typeaheadSuppressWarning) {
	    if (!$typeaheadSuppressWarning) {
	      $log.warn('typeaheadParser is now deprecated. Use uibTypeaheadParser instead.');
	    }
	
	    return uibTypeaheadParser;
	  }])
	
	  .directive('typeahead', ['$compile', '$parse', '$q', '$timeout', '$document', '$window', '$rootScope', '$uibPosition', 'typeaheadParser', '$log', '$typeaheadSuppressWarning',
	    function($compile, $parse, $q, $timeout, $document, $window, $rootScope, $position, typeaheadParser, $log, $typeaheadSuppressWarning) {
	    var HOT_KEYS = [9, 13, 27, 38, 40];
	    var eventDebounceTime = 200;
	    return {
	      require: ['ngModel', '^?ngModelOptions'],
	      link: function(originalScope, element, attrs, ctrls) {
	        if (!$typeaheadSuppressWarning) {
	          $log.warn('typeahead is now deprecated. Use uib-typeahead instead.');
	        }
	        var modelCtrl = ctrls[0];
	        var ngModelOptions = ctrls[1];
	        //SUPPORTED ATTRIBUTES (OPTIONS)
	
	        //minimal no of characters that needs to be entered before typeahead kicks-in
	        var minLength = originalScope.$eval(attrs.typeaheadMinLength);
	        if (!minLength && minLength !== 0) {
	          minLength = 1;
	        }
	
	        //minimal wait time after last character typed before typeahead kicks-in
	        var waitTime = originalScope.$eval(attrs.typeaheadWaitMs) || 0;
	
	        //should it restrict model values to the ones selected from the popup only?
	        var isEditable = originalScope.$eval(attrs.typeaheadEditable) !== false;
	
	        //binding to a variable that indicates if matches are being retrieved asynchronously
	        var isLoadingSetter = $parse(attrs.typeaheadLoading).assign || angular.noop;
	
	        //a callback executed when a match is selected
	        var onSelectCallback = $parse(attrs.typeaheadOnSelect);
	
	        //should it select highlighted popup value when losing focus?
	        var isSelectOnBlur = angular.isDefined(attrs.typeaheadSelectOnBlur) ? originalScope.$eval(attrs.typeaheadSelectOnBlur) : false;
	
	        //binding to a variable that indicates if there were no results after the query is completed
	        var isNoResultsSetter = $parse(attrs.typeaheadNoResults).assign || angular.noop;
	
	        var inputFormatter = attrs.typeaheadInputFormatter ? $parse(attrs.typeaheadInputFormatter) : undefined;
	
	        var appendToBody =  attrs.typeaheadAppendToBody ? originalScope.$eval(attrs.typeaheadAppendToBody) : false;
	
	        var appendToElementId =  attrs.typeaheadAppendToElementId || false;
	
	        var focusFirst = originalScope.$eval(attrs.typeaheadFocusFirst) !== false;
	
	        //If input matches an item of the list exactly, select it automatically
	        var selectOnExact = attrs.typeaheadSelectOnExact ? originalScope.$eval(attrs.typeaheadSelectOnExact) : false;
	
	        //INTERNAL VARIABLES
	
	        //model setter executed upon match selection
	        var parsedModel = $parse(attrs.ngModel);
	        var invokeModelSetter = $parse(attrs.ngModel + '($$$p)');
	        var $setModelValue = function(scope, newValue) {
	          if (angular.isFunction(parsedModel(originalScope)) &&
	            ngModelOptions && ngModelOptions.$options && ngModelOptions.$options.getterSetter) {
	            return invokeModelSetter(scope, {$$$p: newValue});
	          } else {
	            return parsedModel.assign(scope, newValue);
	          }
	        };
	
	        //expressions used by typeahead
	        var parserResult = typeaheadParser.parse(attrs.typeahead);
	
	        var hasFocus;
	
	        //Used to avoid bug in iOS webview where iOS keyboard does not fire
	        //mousedown & mouseup events
	        //Issue #3699
	        var selected;
	
	        //create a child scope for the typeahead directive so we are not polluting original scope
	        //with typeahead-specific data (matches, query etc.)
	        var scope = originalScope.$new();
	        var offDestroy = originalScope.$on('$destroy', function() {
				    scope.$destroy();
	        });
	        scope.$on('$destroy', offDestroy);
	
	        // WAI-ARIA
	        var popupId = 'typeahead-' + scope.$id + '-' + Math.floor(Math.random() * 10000);
	        element.attr({
	          'aria-autocomplete': 'list',
	          'aria-expanded': false,
	          'aria-owns': popupId
	        });
	
	        //pop-up element used to display matches
	        var popUpEl = angular.element('<div typeahead-popup></div>');
	        popUpEl.attr({
	          id: popupId,
	          matches: 'matches',
	          active: 'activeIdx',
	          select: 'select(activeIdx)',
	          'move-in-progress': 'moveInProgress',
	          query: 'query',
	          position: 'position'
	        });
	        //custom item template
	        if (angular.isDefined(attrs.typeaheadTemplateUrl)) {
	          popUpEl.attr('template-url', attrs.typeaheadTemplateUrl);
	        }
	
	        if (angular.isDefined(attrs.typeaheadPopupTemplateUrl)) {
	          popUpEl.attr('popup-template-url', attrs.typeaheadPopupTemplateUrl);
	        }
	
	        var resetMatches = function() {
	          scope.matches = [];
	          scope.activeIdx = -1;
	          element.attr('aria-expanded', false);
	        };
	
	        var getMatchId = function(index) {
	          return popupId + '-option-' + index;
	        };
	
	        // Indicate that the specified match is the active (pre-selected) item in the list owned by this typeahead.
	        // This attribute is added or removed automatically when the `activeIdx` changes.
	        scope.$watch('activeIdx', function(index) {
	          if (index < 0) {
	            element.removeAttr('aria-activedescendant');
	          } else {
	            element.attr('aria-activedescendant', getMatchId(index));
	          }
	        });
	
	        var inputIsExactMatch = function(inputValue, index) {
	          if (scope.matches.length > index && inputValue) {
	            return inputValue.toUpperCase() === scope.matches[index].label.toUpperCase();
	          }
	
	          return false;
	        };
	
	        var getMatchesAsync = function(inputValue) {
	          var locals = {$viewValue: inputValue};
	          isLoadingSetter(originalScope, true);
	          isNoResultsSetter(originalScope, false);
	          $q.when(parserResult.source(originalScope, locals)).then(function(matches) {
	            //it might happen that several async queries were in progress if a user were typing fast
	            //but we are interested only in responses that correspond to the current view value
	            var onCurrentRequest = (inputValue === modelCtrl.$viewValue);
	            if (onCurrentRequest && hasFocus) {
	              if (matches && matches.length > 0) {
	                scope.activeIdx = focusFirst ? 0 : -1;
	                isNoResultsSetter(originalScope, false);
	                scope.matches.length = 0;
	
	                //transform labels
	                for (var i = 0; i < matches.length; i++) {
	                  locals[parserResult.itemName] = matches[i];
	                  scope.matches.push({
	                    id: getMatchId(i),
	                    label: parserResult.viewMapper(scope, locals),
	                    model: matches[i]
	                  });
	                }
	
	                scope.query = inputValue;
	                //position pop-up with matches - we need to re-calculate its position each time we are opening a window
	                //with matches as a pop-up might be absolute-positioned and position of an input might have changed on a page
	                //due to other elements being rendered
	                recalculatePosition();
	
	                element.attr('aria-expanded', true);
	
	                //Select the single remaining option if user input matches
	                if (selectOnExact && scope.matches.length === 1 && inputIsExactMatch(inputValue, 0)) {
	                  scope.select(0);
	                }
	              } else {
	                resetMatches();
	                isNoResultsSetter(originalScope, true);
	              }
	            }
	            if (onCurrentRequest) {
	              isLoadingSetter(originalScope, false);
	            }
	          }, function() {
	            resetMatches();
	            isLoadingSetter(originalScope, false);
	            isNoResultsSetter(originalScope, true);
	          });
	        };
	
	        // bind events only if appendToBody params exist - performance feature
	        if (appendToBody) {
	          angular.element($window).bind('resize', fireRecalculating);
	          $document.find('body').bind('scroll', fireRecalculating);
	        }
	
	        // Declare the timeout promise var outside the function scope so that stacked calls can be cancelled later
	        var timeoutEventPromise;
	
	        // Default progress type
	        scope.moveInProgress = false;
	
	        function fireRecalculating() {
	          if (!scope.moveInProgress) {
	            scope.moveInProgress = true;
	            scope.$digest();
	          }
	
	          // Cancel previous timeout
	          if (timeoutEventPromise) {
	            $timeout.cancel(timeoutEventPromise);
	          }
	
	          // Debounced executing recalculate after events fired
	          timeoutEventPromise = $timeout(function() {
	            // if popup is visible
	            if (scope.matches.length) {
	              recalculatePosition();
	            }
	
	            scope.moveInProgress = false;
	          }, eventDebounceTime);
	        }
	
	        // recalculate actual position and set new values to scope
	        // after digest loop is popup in right position
	        function recalculatePosition() {
	          scope.position = appendToBody ? $position.offset(element) : $position.position(element);
	          scope.position.top += element.prop('offsetHeight');
	        }
	
	        resetMatches();
	
	        //we need to propagate user's query so we can higlight matches
	        scope.query = undefined;
	
	        //Declare the timeout promise var outside the function scope so that stacked calls can be cancelled later
	        var timeoutPromise;
	
	        var scheduleSearchWithTimeout = function(inputValue) {
	          timeoutPromise = $timeout(function() {
	            getMatchesAsync(inputValue);
	          }, waitTime);
	        };
	
	        var cancelPreviousTimeout = function() {
	          if (timeoutPromise) {
	            $timeout.cancel(timeoutPromise);
	          }
	        };
	
	        //plug into $parsers pipeline to open a typeahead on view changes initiated from DOM
	        //$parsers kick-in on all the changes coming from the view as well as manually triggered by $setViewValue
	        modelCtrl.$parsers.unshift(function(inputValue) {
	          hasFocus = true;
	
	          if (minLength === 0 || inputValue && inputValue.length >= minLength) {
	            if (waitTime > 0) {
	              cancelPreviousTimeout();
	              scheduleSearchWithTimeout(inputValue);
	            } else {
	              getMatchesAsync(inputValue);
	            }
	          } else {
	            isLoadingSetter(originalScope, false);
	            cancelPreviousTimeout();
	            resetMatches();
	          }
	
	          if (isEditable) {
	            return inputValue;
	          } else {
	            if (!inputValue) {
	              // Reset in case user had typed something previously.
	              modelCtrl.$setValidity('editable', true);
	              return null;
	            } else {
	              modelCtrl.$setValidity('editable', false);
	              return undefined;
	            }
	          }
	        });
	
	        modelCtrl.$formatters.push(function(modelValue) {
	          var candidateViewValue, emptyViewValue;
	          var locals = {};
	
	          // The validity may be set to false via $parsers (see above) if
	          // the model is restricted to selected values. If the model
	          // is set manually it is considered to be valid.
	          if (!isEditable) {
	            modelCtrl.$setValidity('editable', true);
	          }
	
	          if (inputFormatter) {
	            locals.$model = modelValue;
	            return inputFormatter(originalScope, locals);
	          } else {
	            //it might happen that we don't have enough info to properly render input value
	            //we need to check for this situation and simply return model value if we can't apply custom formatting
	            locals[parserResult.itemName] = modelValue;
	            candidateViewValue = parserResult.viewMapper(originalScope, locals);
	            locals[parserResult.itemName] = undefined;
	            emptyViewValue = parserResult.viewMapper(originalScope, locals);
	
	            return candidateViewValue !== emptyViewValue ? candidateViewValue : modelValue;
	          }
	        });
	
	        scope.select = function(activeIdx) {
	          //called from within the $digest() cycle
	          var locals = {};
	          var model, item;
	
	          selected = true;
	          locals[parserResult.itemName] = item = scope.matches[activeIdx].model;
	          model = parserResult.modelMapper(originalScope, locals);
	          $setModelValue(originalScope, model);
	          modelCtrl.$setValidity('editable', true);
	          modelCtrl.$setValidity('parse', true);
	
	          onSelectCallback(originalScope, {
	            $item: item,
	            $model: model,
	            $label: parserResult.viewMapper(originalScope, locals)
	          });
	
	          resetMatches();
	
	          //return focus to the input element if a match was selected via a mouse click event
	          // use timeout to avoid $rootScope:inprog error
	          if (scope.$eval(attrs.typeaheadFocusOnSelect) !== false) {
	            $timeout(function() { element[0].focus(); }, 0, false);
	          }
	        };
	
	        //bind keyboard events: arrows up(38) / down(40), enter(13) and tab(9), esc(27)
	        element.bind('keydown', function(evt) {
	          //typeahead is open and an "interesting" key was pressed
	          if (scope.matches.length === 0 || HOT_KEYS.indexOf(evt.which) === -1) {
	            return;
	          }
	
	          // if there's nothing selected (i.e. focusFirst) and enter or tab is hit, clear the results
	          if (scope.activeIdx === -1 && (evt.which === 9 || evt.which === 13)) {
	            resetMatches();
	            scope.$digest();
	            return;
	          }
	
	          evt.preventDefault();
	
	          if (evt.which === 40) {
	            scope.activeIdx = (scope.activeIdx + 1) % scope.matches.length;
	            scope.$digest();
	          } else if (evt.which === 38) {
	            scope.activeIdx = (scope.activeIdx > 0 ? scope.activeIdx : scope.matches.length) - 1;
	            scope.$digest();
	          } else if (evt.which === 13 || evt.which === 9) {
	            scope.$apply(function () {
	              scope.select(scope.activeIdx);
	            });
	          } else if (evt.which === 27) {
	            evt.stopPropagation();
	
	            resetMatches();
	            scope.$digest();
	          }
	        });
	
	        element.bind('blur', function() {
	          if (isSelectOnBlur && scope.matches.length && scope.activeIdx !== -1 && !selected) {
	            selected = true;
	            scope.$apply(function() {
	              scope.select(scope.activeIdx);
	            });
	          }
	          hasFocus = false;
	          selected = false;
	        });
	
	        // Keep reference to click handler to unbind it.
	        var dismissClickHandler = function(evt) {
	          // Issue #3973
	          // Firefox treats right click as a click on document
	          if (element[0] !== evt.target && evt.which !== 3 && scope.matches.length !== 0) {
	            resetMatches();
	            if (!$rootScope.$$phase) {
	              scope.$digest();
	            }
	          }
	        };
	
	        $document.bind('click', dismissClickHandler);
	
	        originalScope.$on('$destroy', function() {
	          $document.unbind('click', dismissClickHandler);
	          if (appendToBody || appendToElementId) {
	            $popup.remove();
	          }
	
	          if (appendToBody) {
	            angular.element($window).unbind('resize', fireRecalculating);
	            $document.find('body').unbind('scroll', fireRecalculating);
	          }
	          // Prevent jQuery cache memory leak
	          popUpEl.remove();
	        });
	
	        var $popup = $compile(popUpEl)(scope);
	
	        if (appendToBody) {
	          $document.find('body').append($popup);
	        } else if (appendToElementId !== false) {
	          angular.element($document[0].getElementById(appendToElementId)).append($popup);
	        } else {
	          element.after($popup);
	        }
	      }
	    };
	  }])
	  
	  .directive('typeaheadPopup', ['$typeaheadSuppressWarning', '$log', function($typeaheadSuppressWarning, $log) {
	    return {
	      scope: {
	        matches: '=',
	        query: '=',
	        active: '=',
	        position: '&',
	        moveInProgress: '=',
	        select: '&'
	      },
	      replace: true,
	      templateUrl: function(element, attrs) {
	        return attrs.popupTemplateUrl || 'template/typeahead/typeahead-popup.html';
	      },
	      link: function(scope, element, attrs) {
	        
	        if (!$typeaheadSuppressWarning) {
	          $log.warn('typeahead-popup is now deprecated. Use uib-typeahead-popup instead.');
	        }
	        scope.templateUrl = attrs.templateUrl;
	
	        scope.isOpen = function() {
	          return scope.matches.length > 0;
	        };
	
	        scope.isActive = function(matchIdx) {
	          return scope.active == matchIdx;
	        };
	
	        scope.selectActive = function(matchIdx) {
	          scope.active = matchIdx;
	        };
	
	        scope.selectMatch = function(activeIdx) {
	          scope.select({activeIdx:activeIdx});
	        };
	      }
	    };
	  }])
	  
	  .directive('typeaheadMatch', ['$templateRequest', '$compile', '$parse', '$typeaheadSuppressWarning', '$log', function($templateRequest, $compile, $parse, $typeaheadSuppressWarning, $log) {
	    return {
	      restrict: 'EA',
	      scope: {
	        index: '=',
	        match: '=',
	        query: '='
	      },
	      link:function(scope, element, attrs) {
	        if (!$typeaheadSuppressWarning) {
	          $log.warn('typeahead-match is now deprecated. Use uib-typeahead-match instead.');
	        }
	
	        var tplUrl = $parse(attrs.templateUrl)(scope.$parent) || 'template/typeahead/typeahead-match.html';
	        $templateRequest(tplUrl).then(function(tplContent) {
	          $compile(tplContent.trim())(scope, function(clonedElement) {
	            element.replaceWith(clonedElement);
	          });
	        });
	      }
	    };
	  }])
	  
	  .filter('typeaheadHighlight', ['$sce', '$injector', '$log', '$typeaheadSuppressWarning', function($sce, $injector, $log, $typeaheadSuppressWarning) {
	    var isSanitizePresent;
	    isSanitizePresent = $injector.has('$sanitize');
	
	    function escapeRegexp(queryToEscape) {
	      // Regex: capture the whole query string and replace it with the string that will be used to match
	      // the results, for example if the capture is "a" the result will be \a
	      return queryToEscape.replace(/([.?*+^$[\]\\(){}|-])/g, '\\$1');
	    }
	
	    function containsHtml(matchItem) {
	      return /<.*>/g.test(matchItem);
	    }
	
	    return function(matchItem, query) {
	      if (!$typeaheadSuppressWarning) {
	        $log.warn('typeaheadHighlight is now deprecated. Use uibTypeaheadHighlight instead.');
	      }
	
	      if (!isSanitizePresent && containsHtml(matchItem)) {
	        $log.warn('Unsafe use of typeahead please use ngSanitize'); // Warn the user about the danger
	      }
	
	      matchItem = query? ('' + matchItem).replace(new RegExp(escapeRegexp(query), 'gi'), '<strong>$&</strong>') : matchItem; // Replaces the capture string with a the same string inside of a "strong" tag
	      if (!isSanitizePresent) {
	        matchItem = $sce.trustAsHtml(matchItem); // If $sanitize is not present we pack the string in a $sce object for the ng-bind-html directive
	      }
	
	      return matchItem;
	    };
	  }]);
	
	angular.module("template/accordion/accordion-group.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/accordion/accordion-group.html",
	    "<div class=\"panel {{panelClass || 'panel-default'}}\">\n" +
	    "  <div class=\"panel-heading\" ng-keypress=\"toggleOpen($event)\">\n" +
	    "    <h4 class=\"panel-title\">\n" +
	    "      <a href tabindex=\"0\" class=\"accordion-toggle\" ng-click=\"toggleOpen()\" uib-accordion-transclude=\"heading\"><span ng-class=\"{'text-muted': isDisabled}\">{{heading}}</span></a>\n" +
	    "    </h4>\n" +
	    "  </div>\n" +
	    "  <div class=\"panel-collapse collapse\" uib-collapse=\"!isOpen\">\n" +
	    "	  <div class=\"panel-body\" ng-transclude></div>\n" +
	    "  </div>\n" +
	    "</div>\n" +
	    "");
	}]);
	
	angular.module("template/accordion/accordion.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/accordion/accordion.html",
	    "<div class=\"panel-group\" ng-transclude></div>");
	}]);
	
	angular.module("template/alert/alert.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/alert/alert.html",
	    "<div class=\"alert\" ng-class=\"['alert-' + (type || 'warning'), closeable ? 'alert-dismissible' : null]\" role=\"alert\">\n" +
	    "    <button ng-show=\"closeable\" type=\"button\" class=\"close\" ng-click=\"close({$event: $event})\">\n" +
	    "        <span aria-hidden=\"true\">&times;</span>\n" +
	    "        <span class=\"sr-only\">Close</span>\n" +
	    "    </button>\n" +
	    "    <div ng-transclude></div>\n" +
	    "</div>\n" +
	    "");
	}]);
	
	angular.module("template/carousel/carousel.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/carousel/carousel.html",
	    "<div ng-mouseenter=\"pause()\" ng-mouseleave=\"play()\" class=\"carousel\" ng-swipe-right=\"prev()\" ng-swipe-left=\"next()\">\n" +
	    "  <div class=\"carousel-inner\" ng-transclude></div>\n" +
	    "  <a role=\"button\" href class=\"left carousel-control\" ng-click=\"prev()\" ng-show=\"slides.length > 1\">\n" +
	    "    <span aria-hidden=\"true\" class=\"glyphicon glyphicon-chevron-left\"></span>\n" +
	    "    <span class=\"sr-only\">previous</span>\n" +
	    "  </a>\n" +
	    "  <a role=\"button\" href class=\"right carousel-control\" ng-click=\"next()\" ng-show=\"slides.length > 1\">\n" +
	    "    <span aria-hidden=\"true\" class=\"glyphicon glyphicon-chevron-right\"></span>\n" +
	    "    <span class=\"sr-only\">next</span>\n" +
	    "  </a>\n" +
	    "  <ol class=\"carousel-indicators\" ng-show=\"slides.length > 1\">\n" +
	    "    <li ng-repeat=\"slide in slides | orderBy:indexOfSlide track by $index\" ng-class=\"{ active: isActive(slide) }\" ng-click=\"select(slide)\">\n" +
	    "      <span class=\"sr-only\">slide {{ $index + 1 }} of {{ slides.length }}<span ng-if=\"isActive(slide)\">, currently active</span></span>\n" +
	    "    </li>\n" +
	    "  </ol>\n" +
	    "</div>");
	}]);
	
	angular.module("template/carousel/slide.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/carousel/slide.html",
	    "<div ng-class=\"{\n" +
	    "    'active': active\n" +
	    "  }\" class=\"item text-center\" ng-transclude></div>\n" +
	    "");
	}]);
	
	angular.module("template/datepicker/datepicker.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/datepicker/datepicker.html",
	    "<div ng-switch=\"datepickerMode\" role=\"application\" ng-keydown=\"keydown($event)\">\n" +
	    "  <uib-daypicker ng-switch-when=\"day\" tabindex=\"0\"></uib-daypicker>\n" +
	    "  <uib-monthpicker ng-switch-when=\"month\" tabindex=\"0\"></uib-monthpicker>\n" +
	    "  <uib-yearpicker ng-switch-when=\"year\" tabindex=\"0\"></uib-yearpicker>\n" +
	    "</div>");
	}]);
	
	angular.module("template/datepicker/day.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/datepicker/day.html",
	    "<table role=\"grid\" aria-labelledby=\"{{::uniqueId}}-title\" aria-activedescendant=\"{{activeDateId}}\">\n" +
	    "  <thead>\n" +
	    "    <tr>\n" +
	    "      <th><button type=\"button\" class=\"btn btn-default btn-sm pull-left\" ng-click=\"move(-1)\" tabindex=\"-1\"><i class=\"glyphicon glyphicon-chevron-left\"></i></button></th>\n" +
	    "      <th colspan=\"{{::5 + showWeeks}}\"><button id=\"{{::uniqueId}}-title\" role=\"heading\" aria-live=\"assertive\" aria-atomic=\"true\" type=\"button\" class=\"btn btn-default btn-sm\" ng-click=\"toggleMode()\" ng-disabled=\"datepickerMode === maxMode\" tabindex=\"-1\" style=\"width:100%;\"><strong>{{title}}</strong></button></th>\n" +
	    "      <th><button type=\"button\" class=\"btn btn-default btn-sm pull-right\" ng-click=\"move(1)\" tabindex=\"-1\"><i class=\"glyphicon glyphicon-chevron-right\"></i></button></th>\n" +
	    "    </tr>\n" +
	    "    <tr>\n" +
	    "      <th ng-if=\"showWeeks\" class=\"text-center\"></th>\n" +
	    "      <th ng-repeat=\"label in ::labels track by $index\" class=\"text-center\"><small aria-label=\"{{::label.full}}\">{{::label.abbr}}</small></th>\n" +
	    "    </tr>\n" +
	    "  </thead>\n" +
	    "  <tbody>\n" +
	    "    <tr ng-repeat=\"row in rows track by $index\">\n" +
	    "      <td ng-if=\"showWeeks\" class=\"text-center h6\"><em>{{ weekNumbers[$index] }}</em></td>\n" +
	    "      <td ng-repeat=\"dt in row track by dt.date\" class=\"text-center\" role=\"gridcell\" id=\"{{::dt.uid}}\" ng-class=\"::dt.customClass\">\n" +
	    "        <button type=\"button\" style=\"min-width:100%;\" class=\"btn btn-default btn-sm\" ng-class=\"{'btn-info': dt.selected, active: isActive(dt)}\" ng-click=\"select(dt.date)\" ng-disabled=\"dt.disabled\" tabindex=\"-1\"><span ng-class=\"::{'text-muted': dt.secondary, 'text-info': dt.current}\">{{::dt.label}}</span></button>\n" +
	    "      </td>\n" +
	    "    </tr>\n" +
	    "  </tbody>\n" +
	    "</table>\n" +
	    "");
	}]);
	
	angular.module("template/datepicker/month.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/datepicker/month.html",
	    "<table role=\"grid\" aria-labelledby=\"{{::uniqueId}}-title\" aria-activedescendant=\"{{activeDateId}}\">\n" +
	    "  <thead>\n" +
	    "    <tr>\n" +
	    "      <th><button type=\"button\" class=\"btn btn-default btn-sm pull-left\" ng-click=\"move(-1)\" tabindex=\"-1\"><i class=\"glyphicon glyphicon-chevron-left\"></i></button></th>\n" +
	    "      <th><button id=\"{{::uniqueId}}-title\" role=\"heading\" aria-live=\"assertive\" aria-atomic=\"true\" type=\"button\" class=\"btn btn-default btn-sm\" ng-click=\"toggleMode()\" ng-disabled=\"datepickerMode === maxMode\" tabindex=\"-1\" style=\"width:100%;\"><strong>{{title}}</strong></button></th>\n" +
	    "      <th><button type=\"button\" class=\"btn btn-default btn-sm pull-right\" ng-click=\"move(1)\" tabindex=\"-1\"><i class=\"glyphicon glyphicon-chevron-right\"></i></button></th>\n" +
	    "    </tr>\n" +
	    "  </thead>\n" +
	    "  <tbody>\n" +
	    "    <tr ng-repeat=\"row in rows track by $index\">\n" +
	    "      <td ng-repeat=\"dt in row track by dt.date\" class=\"text-center\" role=\"gridcell\" id=\"{{::dt.uid}}\" ng-class=\"::dt.customClass\">\n" +
	    "        <button type=\"button\" style=\"min-width:100%;\" class=\"btn btn-default\" ng-class=\"{'btn-info': dt.selected, active: isActive(dt)}\" ng-click=\"select(dt.date)\" ng-disabled=\"dt.disabled\" tabindex=\"-1\"><span ng-class=\"::{'text-info': dt.current}\">{{::dt.label}}</span></button>\n" +
	    "      </td>\n" +
	    "    </tr>\n" +
	    "  </tbody>\n" +
	    "</table>\n" +
	    "");
	}]);
	
	angular.module("template/datepicker/popup.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/datepicker/popup.html",
	    "<ul class=\"dropdown-menu\" dropdown-nested ng-if=\"isOpen\" style=\"display: block\" ng-style=\"{top: position.top+'px', left: position.left+'px'}\" ng-keydown=\"keydown($event)\" ng-click=\"$event.stopPropagation()\">\n" +
	    "	<li ng-transclude></li>\n" +
	    "	<li ng-if=\"showButtonBar\" style=\"padding:10px 9px 2px\">\n" +
	    "		<span class=\"btn-group pull-left\">\n" +
	    "			<button type=\"button\" class=\"btn btn-sm btn-info\" ng-click=\"select('today')\" ng-disabled=\"isDisabled('today')\">{{ getText('current') }}</button>\n" +
	    "			<button type=\"button\" class=\"btn btn-sm btn-danger\" ng-click=\"select(null)\">{{ getText('clear') }}</button>\n" +
	    "		</span>\n" +
	    "		<button type=\"button\" class=\"btn btn-sm btn-success pull-right\" ng-click=\"close()\">{{ getText('close') }}</button>\n" +
	    "	</li>\n" +
	    "</ul>\n" +
	    "");
	}]);
	
	angular.module("template/datepicker/year.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/datepicker/year.html",
	    "<table role=\"grid\" aria-labelledby=\"{{::uniqueId}}-title\" aria-activedescendant=\"{{activeDateId}}\">\n" +
	    "  <thead>\n" +
	    "    <tr>\n" +
	    "      <th><button type=\"button\" class=\"btn btn-default btn-sm pull-left\" ng-click=\"move(-1)\" tabindex=\"-1\"><i class=\"glyphicon glyphicon-chevron-left\"></i></button></th>\n" +
	    "      <th colspan=\"3\"><button id=\"{{::uniqueId}}-title\" role=\"heading\" aria-live=\"assertive\" aria-atomic=\"true\" type=\"button\" class=\"btn btn-default btn-sm\" ng-click=\"toggleMode()\" ng-disabled=\"datepickerMode === maxMode\" tabindex=\"-1\" style=\"width:100%;\"><strong>{{title}}</strong></button></th>\n" +
	    "      <th><button type=\"button\" class=\"btn btn-default btn-sm pull-right\" ng-click=\"move(1)\" tabindex=\"-1\"><i class=\"glyphicon glyphicon-chevron-right\"></i></button></th>\n" +
	    "    </tr>\n" +
	    "  </thead>\n" +
	    "  <tbody>\n" +
	    "    <tr ng-repeat=\"row in rows track by $index\">\n" +
	    "      <td ng-repeat=\"dt in row track by dt.date\" class=\"text-center\" role=\"gridcell\" id=\"{{::dt.uid}}\" ng-class=\"::dt.customClass\">\n" +
	    "        <button type=\"button\" style=\"min-width:100%;\" class=\"btn btn-default\" ng-class=\"{'btn-info': dt.selected, active: isActive(dt)}\" ng-click=\"select(dt.date)\" ng-disabled=\"dt.disabled\" tabindex=\"-1\"><span ng-class=\"::{'text-info': dt.current}\">{{::dt.label}}</span></button>\n" +
	    "      </td>\n" +
	    "    </tr>\n" +
	    "  </tbody>\n" +
	    "</table>\n" +
	    "");
	}]);
	
	angular.module("template/modal/backdrop.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/modal/backdrop.html",
	    "<div uib-modal-animation-class=\"fade\"\n" +
	    "     modal-in-class=\"in\"\n" +
	    "     ng-style=\"{'z-index': 1040 + (index && 1 || 0) + index*10}\"\n" +
	    "></div>\n" +
	    "");
	}]);
	
	angular.module("template/modal/window.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/modal/window.html",
	    "<div modal-render=\"{{$isRendered}}\" tabindex=\"-1\" role=\"dialog\" class=\"modal\"\n" +
	    "    uib-modal-animation-class=\"fade\"\n" +
	    "    modal-in-class=\"in\"\n" +
	    "    ng-style=\"{'z-index': 1050 + index*10, display: 'block'}\">\n" +
	    "    <div class=\"modal-dialog\" ng-class=\"size ? 'modal-' + size : ''\"><div class=\"modal-content\" uib-modal-transclude></div></div>\n" +
	    "</div>\n" +
	    "");
	}]);
	
	angular.module("template/pagination/pager.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/pagination/pager.html",
	    "<ul class=\"pager\">\n" +
	    "  <li ng-class=\"{disabled: noPrevious()||ngDisabled, previous: align}\"><a href ng-click=\"selectPage(page - 1, $event)\">{{::getText('previous')}}</a></li>\n" +
	    "  <li ng-class=\"{disabled: noNext()||ngDisabled, next: align}\"><a href ng-click=\"selectPage(page + 1, $event)\">{{::getText('next')}}</a></li>\n" +
	    "</ul>\n" +
	    "");
	}]);
	
	angular.module("template/pagination/pagination.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/pagination/pagination.html",
	    "<ul class=\"pagination\">\n" +
	    "  <li ng-if=\"::boundaryLinks\" ng-class=\"{disabled: noPrevious()||ngDisabled}\" class=\"pagination-first\"><a href ng-click=\"selectPage(1, $event)\">{{::getText('first')}}</a></li>\n" +
	    "  <li ng-if=\"::directionLinks\" ng-class=\"{disabled: noPrevious()||ngDisabled}\" class=\"pagination-prev\"><a href ng-click=\"selectPage(page - 1, $event)\">{{::getText('previous')}}</a></li>\n" +
	    "  <li ng-repeat=\"page in pages track by $index\" ng-class=\"{active: page.active,disabled: ngDisabled&&!page.active}\" class=\"pagination-page\"><a href ng-click=\"selectPage(page.number, $event)\">{{page.text}}</a></li>\n" +
	    "  <li ng-if=\"::directionLinks\" ng-class=\"{disabled: noNext()||ngDisabled}\" class=\"pagination-next\"><a href ng-click=\"selectPage(page + 1, $event)\">{{::getText('next')}}</a></li>\n" +
	    "  <li ng-if=\"::boundaryLinks\" ng-class=\"{disabled: noNext()||ngDisabled}\" class=\"pagination-last\"><a href ng-click=\"selectPage(totalPages, $event)\">{{::getText('last')}}</a></li>\n" +
	    "</ul>\n" +
	    "");
	}]);
	
	angular.module("template/tooltip/tooltip-html-popup.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/tooltip/tooltip-html-popup.html",
	    "<div\n" +
	    "  tooltip-animation-class=\"fade\"\n" +
	    "  uib-tooltip-classes\n" +
	    "  ng-class=\"{ in: isOpen() }\">\n" +
	    "  <div class=\"tooltip-arrow\"></div>\n" +
	    "  <div class=\"tooltip-inner\" ng-bind-html=\"contentExp()\"></div>\n" +
	    "</div>\n" +
	    "");
	}]);
	
	angular.module("template/tooltip/tooltip-popup.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/tooltip/tooltip-popup.html",
	    "<div\n" +
	    "  tooltip-animation-class=\"fade\"\n" +
	    "  uib-tooltip-classes\n" +
	    "  ng-class=\"{ in: isOpen() }\">\n" +
	    "  <div class=\"tooltip-arrow\"></div>\n" +
	    "  <div class=\"tooltip-inner\" ng-bind=\"content\"></div>\n" +
	    "</div>\n" +
	    "");
	}]);
	
	angular.module("template/tooltip/tooltip-template-popup.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/tooltip/tooltip-template-popup.html",
	    "<div\n" +
	    "  tooltip-animation-class=\"fade\"\n" +
	    "  uib-tooltip-classes\n" +
	    "  ng-class=\"{ in: isOpen() }\">\n" +
	    "  <div class=\"tooltip-arrow\"></div>\n" +
	    "  <div class=\"tooltip-inner\"\n" +
	    "    uib-tooltip-template-transclude=\"contentExp()\"\n" +
	    "    tooltip-template-transclude-scope=\"originScope()\"></div>\n" +
	    "</div>\n" +
	    "");
	}]);
	
	angular.module("template/popover/popover-html.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/popover/popover-html.html",
	    "<div tooltip-animation-class=\"fade\"\n" +
	    "  uib-tooltip-classes\n" +
	    "  ng-class=\"{ in: isOpen() }\">\n" +
	    "  <div class=\"arrow\"></div>\n" +
	    "\n" +
	    "  <div class=\"popover-inner\">\n" +
	    "      <h3 class=\"popover-title\" ng-bind=\"title\" ng-if=\"title\"></h3>\n" +
	    "      <div class=\"popover-content\" ng-bind-html=\"contentExp()\"></div>\n" +
	    "  </div>\n" +
	    "</div>\n" +
	    "");
	}]);
	
	angular.module("template/popover/popover-template.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/popover/popover-template.html",
	    "<div tooltip-animation-class=\"fade\"\n" +
	    "  uib-tooltip-classes\n" +
	    "  ng-class=\"{ in: isOpen() }\">\n" +
	    "  <div class=\"arrow\"></div>\n" +
	    "\n" +
	    "  <div class=\"popover-inner\">\n" +
	    "      <h3 class=\"popover-title\" ng-bind=\"title\" ng-if=\"title\"></h3>\n" +
	    "      <div class=\"popover-content\"\n" +
	    "        uib-tooltip-template-transclude=\"contentExp()\"\n" +
	    "        tooltip-template-transclude-scope=\"originScope()\"></div>\n" +
	    "  </div>\n" +
	    "</div>\n" +
	    "");
	}]);
	
	angular.module("template/popover/popover.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/popover/popover.html",
	    "<div tooltip-animation-class=\"fade\"\n" +
	    "  uib-tooltip-classes\n" +
	    "  ng-class=\"{ in: isOpen() }\">\n" +
	    "  <div class=\"arrow\"></div>\n" +
	    "\n" +
	    "  <div class=\"popover-inner\">\n" +
	    "      <h3 class=\"popover-title\" ng-bind=\"title\" ng-if=\"title\"></h3>\n" +
	    "      <div class=\"popover-content\" ng-bind=\"content\"></div>\n" +
	    "  </div>\n" +
	    "</div>\n" +
	    "");
	}]);
	
	angular.module("template/progressbar/bar.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/progressbar/bar.html",
	    "<div class=\"progress-bar\" ng-class=\"type && 'progress-bar-' + type\" role=\"progressbar\" aria-valuenow=\"{{value}}\" aria-valuemin=\"0\" aria-valuemax=\"{{max}}\" ng-style=\"{width: (percent < 100 ? percent : 100) + '%'}\" aria-valuetext=\"{{percent | number:0}}%\" aria-labelledby=\"{{::title}}\" style=\"min-width: 0;\" ng-transclude></div>\n" +
	    "");
	}]);
	
	angular.module("template/progressbar/progress.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/progressbar/progress.html",
	    "<div class=\"progress\" ng-transclude aria-labelledby=\"{{::title}}\"></div>");
	}]);
	
	angular.module("template/progressbar/progressbar.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/progressbar/progressbar.html",
	    "<div class=\"progress\">\n" +
	    "  <div class=\"progress-bar\" ng-class=\"type && 'progress-bar-' + type\" role=\"progressbar\" aria-valuenow=\"{{value}}\" aria-valuemin=\"0\" aria-valuemax=\"{{max}}\" ng-style=\"{width: (percent < 100 ? percent : 100) + '%'}\" aria-valuetext=\"{{percent | number:0}}%\" aria-labelledby=\"{{::title}}\" style=\"min-width: 0;\" ng-transclude></div>\n" +
	    "</div>\n" +
	    "");
	}]);
	
	angular.module("template/rating/rating.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/rating/rating.html",
	    "<span ng-mouseleave=\"reset()\" ng-keydown=\"onKeydown($event)\" tabindex=\"0\" role=\"slider\" aria-valuemin=\"0\" aria-valuemax=\"{{range.length}}\" aria-valuenow=\"{{value}}\">\n" +
	    "    <span ng-repeat-start=\"r in range track by $index\" class=\"sr-only\">({{ $index < value ? '*' : ' ' }})</span>\n" +
	    "    <i ng-repeat-end ng-mouseenter=\"enter($index + 1)\" ng-click=\"rate($index + 1)\" class=\"glyphicon\" ng-class=\"$index < value && (r.stateOn || 'glyphicon-star') || (r.stateOff || 'glyphicon-star-empty')\" ng-attr-title=\"{{r.title}}\" aria-valuetext=\"{{r.title}}\"></i>\n" +
	    "</span>\n" +
	    "");
	}]);
	
	angular.module("template/tabs/tab.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/tabs/tab.html",
	    "<li ng-class=\"{active: active, disabled: disabled}\">\n" +
	    "  <a href ng-click=\"select()\" uib-tab-heading-transclude>{{heading}}</a>\n" +
	    "</li>\n" +
	    "");
	}]);
	
	angular.module("template/tabs/tabset.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/tabs/tabset.html",
	    "<div>\n" +
	    "  <ul class=\"nav nav-{{type || 'tabs'}}\" ng-class=\"{'nav-stacked': vertical, 'nav-justified': justified}\" ng-transclude></ul>\n" +
	    "  <div class=\"tab-content\">\n" +
	    "    <div class=\"tab-pane\" \n" +
	    "         ng-repeat=\"tab in tabs\" \n" +
	    "         ng-class=\"{active: tab.active}\"\n" +
	    "         uib-tab-content-transclude=\"tab\">\n" +
	    "    </div>\n" +
	    "  </div>\n" +
	    "</div>\n" +
	    "");
	}]);
	
	angular.module("template/timepicker/timepicker.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/timepicker/timepicker.html",
	    "<table>\n" +
	    "  <tbody>\n" +
	    "    <tr class=\"text-center\" ng-show=\"::showSpinners\">\n" +
	    "      <td><a ng-click=\"incrementHours()\" ng-class=\"{disabled: noIncrementHours()}\" class=\"btn btn-link\" ng-disabled=\"noIncrementHours()\" tabindex=\"{{::tabindex}}\"><span class=\"glyphicon glyphicon-chevron-up\"></span></a></td>\n" +
	    "      <td>&nbsp;</td>\n" +
	    "      <td><a ng-click=\"incrementMinutes()\" ng-class=\"{disabled: noIncrementMinutes()}\" class=\"btn btn-link\" ng-disabled=\"noIncrementMinutes()\" tabindex=\"{{::tabindex}}\"><span class=\"glyphicon glyphicon-chevron-up\"></span></a></td>\n" +
	    "      <td ng-show=\"showMeridian\"></td>\n" +
	    "    </tr>\n" +
	    "    <tr>\n" +
	    "      <td class=\"form-group\" ng-class=\"{'has-error': invalidHours}\">\n" +
	    "        <input style=\"width:50px;\" type=\"text\" ng-model=\"hours\" ng-change=\"updateHours()\" class=\"form-control text-center\" ng-readonly=\"::readonlyInput\" maxlength=\"2\" tabindex=\"{{::tabindex}}\">\n" +
	    "      </td>\n" +
	    "      <td>:</td>\n" +
	    "      <td class=\"form-group\" ng-class=\"{'has-error': invalidMinutes}\">\n" +
	    "        <input style=\"width:50px;\" type=\"text\" ng-model=\"minutes\" ng-change=\"updateMinutes()\" class=\"form-control text-center\" ng-readonly=\"::readonlyInput\" maxlength=\"2\" tabindex=\"{{::tabindex}}\">\n" +
	    "      </td>\n" +
	    "      <td ng-show=\"showMeridian\"><button type=\"button\" ng-class=\"{disabled: noToggleMeridian()}\" class=\"btn btn-default text-center\" ng-click=\"toggleMeridian()\" ng-disabled=\"noToggleMeridian()\" tabindex=\"{{::tabindex}}\">{{meridian}}</button></td>\n" +
	    "    </tr>\n" +
	    "    <tr class=\"text-center\" ng-show=\"::showSpinners\">\n" +
	    "      <td><a ng-click=\"decrementHours()\" ng-class=\"{disabled: noDecrementHours()}\" class=\"btn btn-link\" ng-disabled=\"noDecrementHours()\" tabindex=\"{{::tabindex}}\"><span class=\"glyphicon glyphicon-chevron-down\"></span></a></td>\n" +
	    "      <td>&nbsp;</td>\n" +
	    "      <td><a ng-click=\"decrementMinutes()\" ng-class=\"{disabled: noDecrementMinutes()}\" class=\"btn btn-link\" ng-disabled=\"noDecrementMinutes()\" tabindex=\"{{::tabindex}}\"><span class=\"glyphicon glyphicon-chevron-down\"></span></a></td>\n" +
	    "      <td ng-show=\"showMeridian\"></td>\n" +
	    "    </tr>\n" +
	    "  </tbody>\n" +
	    "</table>\n" +
	    "");
	}]);
	
	angular.module("template/typeahead/typeahead-match.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/typeahead/typeahead-match.html",
	    "<a href tabindex=\"-1\" ng-bind-html=\"match.label | uibTypeaheadHighlight:query\"></a>\n" +
	    "");
	}]);
	
	angular.module("template/typeahead/typeahead-popup.html", []).run(["$templateCache", function($templateCache) {
	  $templateCache.put("template/typeahead/typeahead-popup.html",
	    "<ul class=\"dropdown-menu\" ng-show=\"isOpen() && !moveInProgress\" ng-style=\"{top: position().top+'px', left: position().left+'px'}\" style=\"display: block;\" role=\"listbox\" aria-hidden=\"{{!isOpen()}}\">\n" +
	    "    <li ng-repeat=\"match in matches track by $index\" ng-class=\"{active: isActive($index) }\" ng-mouseenter=\"selectActive($index)\" ng-click=\"selectMatch($index)\" role=\"option\" id=\"{{::match.id}}\">\n" +
	    "        <div uib-typeahead-match index=\"$index\" match=\"match\" query=\"query\" template-url=\"templateUrl\"></div>\n" +
	    "    </li>\n" +
	    "</ul>\n" +
	    "");
	}]);
	!angular.$$csp() && angular.element(document).find('head').prepend('<style type="text/css">.ng-animate.item:not(.left):not(.right){-webkit-transition:0s ease-in-out left;transition:0s ease-in-out left}</style>');

/***/ },

/***/ 187:
/***/ function(module, exports, __webpack_require__) {

	__webpack_require__(188);
	module.exports = 'ngSanitize';


/***/ },

/***/ 188:
/***/ function(module, exports) {

	/**
	 * @license AngularJS v1.5.3
	 * (c) 2010-2016 Google, Inc. http://angularjs.org
	 * License: MIT
	 */
	(function(window, angular, undefined) {'use strict';
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *     Any commits to this file should be reviewed with security in mind.  *
	 *   Changes to this file can potentially create security vulnerabilities. *
	 *          An approval from 2 Core members with history of modifying      *
	 *                         this file is required.                          *
	 *                                                                         *
	 *  Does the change somehow allow for arbitrary javascript to be executed? *
	 *    Or allows for someone to change the prototype of built-in objects?   *
	 *     Or gives undesired access to variables likes document or window?    *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	var $sanitizeMinErr = angular.$$minErr('$sanitize');
	
	/**
	 * @ngdoc module
	 * @name ngSanitize
	 * @description
	 *
	 * # ngSanitize
	 *
	 * The `ngSanitize` module provides functionality to sanitize HTML.
	 *
	 *
	 * <div doc-module-components="ngSanitize"></div>
	 *
	 * See {@link ngSanitize.$sanitize `$sanitize`} for usage.
	 */
	
	/**
	 * @ngdoc service
	 * @name $sanitize
	 * @kind function
	 *
	 * @description
	 *   Sanitizes an html string by stripping all potentially dangerous tokens.
	 *
	 *   The input is sanitized by parsing the HTML into tokens. All safe tokens (from a whitelist) are
	 *   then serialized back to properly escaped html string. This means that no unsafe input can make
	 *   it into the returned string.
	 *
	 *   The whitelist for URL sanitization of attribute values is configured using the functions
	 *   `aHrefSanitizationWhitelist` and `imgSrcSanitizationWhitelist` of {@link ng.$compileProvider
	 *   `$compileProvider`}.
	 *
	 *   The input may also contain SVG markup if this is enabled via {@link $sanitizeProvider}.
	 *
	 * @param {string} html HTML input.
	 * @returns {string} Sanitized HTML.
	 *
	 * @example
	   <example module="sanitizeExample" deps="angular-sanitize.js">
	   <file name="index.html">
	     <script>
	         angular.module('sanitizeExample', ['ngSanitize'])
	           .controller('ExampleController', ['$scope', '$sce', function($scope, $sce) {
	             $scope.snippet =
	               '<p style="color:blue">an html\n' +
	               '<em onmouseover="this.textContent=\'PWN3D!\'">click here</em>\n' +
	               'snippet</p>';
	             $scope.deliberatelyTrustDangerousSnippet = function() {
	               return $sce.trustAsHtml($scope.snippet);
	             };
	           }]);
	     </script>
	     <div ng-controller="ExampleController">
	        Snippet: <textarea ng-model="snippet" cols="60" rows="3"></textarea>
	       <table>
	         <tr>
	           <td>Directive</td>
	           <td>How</td>
	           <td>Source</td>
	           <td>Rendered</td>
	         </tr>
	         <tr id="bind-html-with-sanitize">
	           <td>ng-bind-html</td>
	           <td>Automatically uses $sanitize</td>
	           <td><pre>&lt;div ng-bind-html="snippet"&gt;<br/>&lt;/div&gt;</pre></td>
	           <td><div ng-bind-html="snippet"></div></td>
	         </tr>
	         <tr id="bind-html-with-trust">
	           <td>ng-bind-html</td>
	           <td>Bypass $sanitize by explicitly trusting the dangerous value</td>
	           <td>
	           <pre>&lt;div ng-bind-html="deliberatelyTrustDangerousSnippet()"&gt;
	&lt;/div&gt;</pre>
	           </td>
	           <td><div ng-bind-html="deliberatelyTrustDangerousSnippet()"></div></td>
	         </tr>
	         <tr id="bind-default">
	           <td>ng-bind</td>
	           <td>Automatically escapes</td>
	           <td><pre>&lt;div ng-bind="snippet"&gt;<br/>&lt;/div&gt;</pre></td>
	           <td><div ng-bind="snippet"></div></td>
	         </tr>
	       </table>
	       </div>
	   </file>
	   <file name="protractor.js" type="protractor">
	     it('should sanitize the html snippet by default', function() {
	       expect(element(by.css('#bind-html-with-sanitize div')).getInnerHtml()).
	         toBe('<p>an html\n<em>click here</em>\nsnippet</p>');
	     });
	
	     it('should inline raw snippet if bound to a trusted value', function() {
	       expect(element(by.css('#bind-html-with-trust div')).getInnerHtml()).
	         toBe("<p style=\"color:blue\">an html\n" +
	              "<em onmouseover=\"this.textContent='PWN3D!'\">click here</em>\n" +
	              "snippet</p>");
	     });
	
	     it('should escape snippet without any filter', function() {
	       expect(element(by.css('#bind-default div')).getInnerHtml()).
	         toBe("&lt;p style=\"color:blue\"&gt;an html\n" +
	              "&lt;em onmouseover=\"this.textContent='PWN3D!'\"&gt;click here&lt;/em&gt;\n" +
	              "snippet&lt;/p&gt;");
	     });
	
	     it('should update', function() {
	       element(by.model('snippet')).clear();
	       element(by.model('snippet')).sendKeys('new <b onclick="alert(1)">text</b>');
	       expect(element(by.css('#bind-html-with-sanitize div')).getInnerHtml()).
	         toBe('new <b>text</b>');
	       expect(element(by.css('#bind-html-with-trust div')).getInnerHtml()).toBe(
	         'new <b onclick="alert(1)">text</b>');
	       expect(element(by.css('#bind-default div')).getInnerHtml()).toBe(
	         "new &lt;b onclick=\"alert(1)\"&gt;text&lt;/b&gt;");
	     });
	   </file>
	   </example>
	 */
	
	
	/**
	 * @ngdoc provider
	 * @name $sanitizeProvider
	 *
	 * @description
	 * Creates and configures {@link $sanitize} instance.
	 */
	function $SanitizeProvider() {
	  var svgEnabled = false;
	
	  this.$get = ['$$sanitizeUri', function($$sanitizeUri) {
	    if (svgEnabled) {
	      angular.extend(validElements, svgElements);
	    }
	    return function(html) {
	      var buf = [];
	      htmlParser(html, htmlSanitizeWriter(buf, function(uri, isImage) {
	        return !/^unsafe:/.test($$sanitizeUri(uri, isImage));
	      }));
	      return buf.join('');
	    };
	  }];
	
	
	  /**
	   * @ngdoc method
	   * @name $sanitizeProvider#enableSvg
	   * @kind function
	   *
	   * @description
	   * Enables a subset of svg to be supported by the sanitizer.
	   *
	   * <div class="alert alert-warning">
	   *   <p>By enabling this setting without taking other precautions, you might expose your
	   *   application to click-hijacking attacks. In these attacks, sanitized svg elements could be positioned
	   *   outside of the containing element and be rendered over other elements on the page (e.g. a login
	   *   link). Such behavior can then result in phishing incidents.</p>
	   *
	   *   <p>To protect against these, explicitly setup `overflow: hidden` css rule for all potential svg
	   *   tags within the sanitized content:</p>
	   *
	   *   <br>
	   *
	   *   <pre><code>
	   *   .rootOfTheIncludedContent svg {
	   *     overflow: hidden !important;
	   *   }
	   *   </code></pre>
	   * </div>
	   *
	   * @param {boolean=} regexp New regexp to whitelist urls with.
	   * @returns {boolean|ng.$sanitizeProvider} Returns the currently configured value if called
	   *    without an argument or self for chaining otherwise.
	   */
	  this.enableSvg = function(enableSvg) {
	    if (angular.isDefined(enableSvg)) {
	      svgEnabled = enableSvg;
	      return this;
	    } else {
	      return svgEnabled;
	    }
	  };
	}
	
	function sanitizeText(chars) {
	  var buf = [];
	  var writer = htmlSanitizeWriter(buf, angular.noop);
	  writer.chars(chars);
	  return buf.join('');
	}
	
	
	// Regular Expressions for parsing tags and attributes
	var SURROGATE_PAIR_REGEXP = /[\uD800-\uDBFF][\uDC00-\uDFFF]/g,
	  // Match everything outside of normal chars and " (quote character)
	  NON_ALPHANUMERIC_REGEXP = /([^\#-~ |!])/g;
	
	
	// Good source of info about elements and attributes
	// http://dev.w3.org/html5/spec/Overview.html#semantics
	// http://simon.html5.org/html-elements
	
	// Safe Void Elements - HTML5
	// http://dev.w3.org/html5/spec/Overview.html#void-elements
	var voidElements = toMap("area,br,col,hr,img,wbr");
	
	// Elements that you can, intentionally, leave open (and which close themselves)
	// http://dev.w3.org/html5/spec/Overview.html#optional-tags
	var optionalEndTagBlockElements = toMap("colgroup,dd,dt,li,p,tbody,td,tfoot,th,thead,tr"),
	    optionalEndTagInlineElements = toMap("rp,rt"),
	    optionalEndTagElements = angular.extend({},
	                                            optionalEndTagInlineElements,
	                                            optionalEndTagBlockElements);
	
	// Safe Block Elements - HTML5
	var blockElements = angular.extend({}, optionalEndTagBlockElements, toMap("address,article," +
	        "aside,blockquote,caption,center,del,dir,div,dl,figure,figcaption,footer,h1,h2,h3,h4,h5," +
	        "h6,header,hgroup,hr,ins,map,menu,nav,ol,pre,section,table,ul"));
	
	// Inline Elements - HTML5
	var inlineElements = angular.extend({}, optionalEndTagInlineElements, toMap("a,abbr,acronym,b," +
	        "bdi,bdo,big,br,cite,code,del,dfn,em,font,i,img,ins,kbd,label,map,mark,q,ruby,rp,rt,s," +
	        "samp,small,span,strike,strong,sub,sup,time,tt,u,var"));
	
	// SVG Elements
	// https://wiki.whatwg.org/wiki/Sanitization_rules#svg_Elements
	// Note: the elements animate,animateColor,animateMotion,animateTransform,set are intentionally omitted.
	// They can potentially allow for arbitrary javascript to be executed. See #11290
	var svgElements = toMap("circle,defs,desc,ellipse,font-face,font-face-name,font-face-src,g,glyph," +
	        "hkern,image,linearGradient,line,marker,metadata,missing-glyph,mpath,path,polygon,polyline," +
	        "radialGradient,rect,stop,svg,switch,text,title,tspan");
	
	// Blocked Elements (will be stripped)
	var blockedElements = toMap("script,style");
	
	var validElements = angular.extend({},
	                                   voidElements,
	                                   blockElements,
	                                   inlineElements,
	                                   optionalEndTagElements);
	
	//Attributes that have href and hence need to be sanitized
	var uriAttrs = toMap("background,cite,href,longdesc,src,xlink:href");
	
	var htmlAttrs = toMap('abbr,align,alt,axis,bgcolor,border,cellpadding,cellspacing,class,clear,' +
	    'color,cols,colspan,compact,coords,dir,face,headers,height,hreflang,hspace,' +
	    'ismap,lang,language,nohref,nowrap,rel,rev,rows,rowspan,rules,' +
	    'scope,scrolling,shape,size,span,start,summary,tabindex,target,title,type,' +
	    'valign,value,vspace,width');
	
	// SVG attributes (without "id" and "name" attributes)
	// https://wiki.whatwg.org/wiki/Sanitization_rules#svg_Attributes
	var svgAttrs = toMap('accent-height,accumulate,additive,alphabetic,arabic-form,ascent,' +
	    'baseProfile,bbox,begin,by,calcMode,cap-height,class,color,color-rendering,content,' +
	    'cx,cy,d,dx,dy,descent,display,dur,end,fill,fill-rule,font-family,font-size,font-stretch,' +
	    'font-style,font-variant,font-weight,from,fx,fy,g1,g2,glyph-name,gradientUnits,hanging,' +
	    'height,horiz-adv-x,horiz-origin-x,ideographic,k,keyPoints,keySplines,keyTimes,lang,' +
	    'marker-end,marker-mid,marker-start,markerHeight,markerUnits,markerWidth,mathematical,' +
	    'max,min,offset,opacity,orient,origin,overline-position,overline-thickness,panose-1,' +
	    'path,pathLength,points,preserveAspectRatio,r,refX,refY,repeatCount,repeatDur,' +
	    'requiredExtensions,requiredFeatures,restart,rotate,rx,ry,slope,stemh,stemv,stop-color,' +
	    'stop-opacity,strikethrough-position,strikethrough-thickness,stroke,stroke-dasharray,' +
	    'stroke-dashoffset,stroke-linecap,stroke-linejoin,stroke-miterlimit,stroke-opacity,' +
	    'stroke-width,systemLanguage,target,text-anchor,to,transform,type,u1,u2,underline-position,' +
	    'underline-thickness,unicode,unicode-range,units-per-em,values,version,viewBox,visibility,' +
	    'width,widths,x,x-height,x1,x2,xlink:actuate,xlink:arcrole,xlink:role,xlink:show,xlink:title,' +
	    'xlink:type,xml:base,xml:lang,xml:space,xmlns,xmlns:xlink,y,y1,y2,zoomAndPan', true);
	
	var validAttrs = angular.extend({},
	                                uriAttrs,
	                                svgAttrs,
	                                htmlAttrs);
	
	function toMap(str, lowercaseKeys) {
	  var obj = {}, items = str.split(','), i;
	  for (i = 0; i < items.length; i++) {
	    obj[lowercaseKeys ? angular.lowercase(items[i]) : items[i]] = true;
	  }
	  return obj;
	}
	
	var inertBodyElement;
	(function(window) {
	  var doc;
	  if (window.document && window.document.implementation) {
	    doc = window.document.implementation.createHTMLDocument("inert");
	  } else {
	    throw $sanitizeMinErr('noinert', "Can't create an inert html document");
	  }
	  var docElement = doc.documentElement || doc.getDocumentElement();
	  var bodyElements = docElement.getElementsByTagName('body');
	
	  // usually there should be only one body element in the document, but IE doesn't have any, so we need to create one
	  if (bodyElements.length === 1) {
	    inertBodyElement = bodyElements[0];
	  } else {
	    var html = doc.createElement('html');
	    inertBodyElement = doc.createElement('body');
	    html.appendChild(inertBodyElement);
	    doc.appendChild(html);
	  }
	})(window);
	
	/**
	 * @example
	 * htmlParser(htmlString, {
	 *     start: function(tag, attrs) {},
	 *     end: function(tag) {},
	 *     chars: function(text) {},
	 *     comment: function(text) {}
	 * });
	 *
	 * @param {string} html string
	 * @param {object} handler
	 */
	function htmlParser(html, handler) {
	  if (html === null || html === undefined) {
	    html = '';
	  } else if (typeof html !== 'string') {
	    html = '' + html;
	  }
	  inertBodyElement.innerHTML = html;
	
	  //mXSS protection
	  var mXSSAttempts = 5;
	  do {
	    if (mXSSAttempts === 0) {
	      throw $sanitizeMinErr('uinput', "Failed to sanitize html because the input is unstable");
	    }
	    mXSSAttempts--;
	
	    // strip custom-namespaced attributes on IE<=11
	    if (document.documentMode <= 11) {
	      stripCustomNsAttrs(inertBodyElement);
	    }
	    html = inertBodyElement.innerHTML; //trigger mXSS
	    inertBodyElement.innerHTML = html;
	  } while (html !== inertBodyElement.innerHTML);
	
	  var node = inertBodyElement.firstChild;
	  while (node) {
	    switch (node.nodeType) {
	      case 1: // ELEMENT_NODE
	        handler.start(node.nodeName.toLowerCase(), attrToMap(node.attributes));
	        break;
	      case 3: // TEXT NODE
	        handler.chars(node.textContent);
	        break;
	    }
	
	    var nextNode;
	    if (!(nextNode = node.firstChild)) {
	      if (node.nodeType == 1) {
	        handler.end(node.nodeName.toLowerCase());
	      }
	      nextNode = node.nextSibling;
	      if (!nextNode) {
	        while (nextNode == null) {
	          node = node.parentNode;
	          if (node === inertBodyElement) break;
	          nextNode = node.nextSibling;
	          if (node.nodeType == 1) {
	            handler.end(node.nodeName.toLowerCase());
	          }
	        }
	      }
	    }
	    node = nextNode;
	  }
	
	  while (node = inertBodyElement.firstChild) {
	    inertBodyElement.removeChild(node);
	  }
	}
	
	function attrToMap(attrs) {
	  var map = {};
	  for (var i = 0, ii = attrs.length; i < ii; i++) {
	    var attr = attrs[i];
	    map[attr.name] = attr.value;
	  }
	  return map;
	}
	
	
	/**
	 * Escapes all potentially dangerous characters, so that the
	 * resulting string can be safely inserted into attribute or
	 * element text.
	 * @param value
	 * @returns {string} escaped text
	 */
	function encodeEntities(value) {
	  return value.
	    replace(/&/g, '&amp;').
	    replace(SURROGATE_PAIR_REGEXP, function(value) {
	      var hi = value.charCodeAt(0);
	      var low = value.charCodeAt(1);
	      return '&#' + (((hi - 0xD800) * 0x400) + (low - 0xDC00) + 0x10000) + ';';
	    }).
	    replace(NON_ALPHANUMERIC_REGEXP, function(value) {
	      return '&#' + value.charCodeAt(0) + ';';
	    }).
	    replace(/</g, '&lt;').
	    replace(/>/g, '&gt;');
	}
	
	/**
	 * create an HTML/XML writer which writes to buffer
	 * @param {Array} buf use buf.join('') to get out sanitized html string
	 * @returns {object} in the form of {
	 *     start: function(tag, attrs) {},
	 *     end: function(tag) {},
	 *     chars: function(text) {},
	 *     comment: function(text) {}
	 * }
	 */
	function htmlSanitizeWriter(buf, uriValidator) {
	  var ignoreCurrentElement = false;
	  var out = angular.bind(buf, buf.push);
	  return {
	    start: function(tag, attrs) {
	      tag = angular.lowercase(tag);
	      if (!ignoreCurrentElement && blockedElements[tag]) {
	        ignoreCurrentElement = tag;
	      }
	      if (!ignoreCurrentElement && validElements[tag] === true) {
	        out('<');
	        out(tag);
	        angular.forEach(attrs, function(value, key) {
	          var lkey=angular.lowercase(key);
	          var isImage = (tag === 'img' && lkey === 'src') || (lkey === 'background');
	          if (validAttrs[lkey] === true &&
	            (uriAttrs[lkey] !== true || uriValidator(value, isImage))) {
	            out(' ');
	            out(key);
	            out('="');
	            out(encodeEntities(value));
	            out('"');
	          }
	        });
	        out('>');
	      }
	    },
	    end: function(tag) {
	      tag = angular.lowercase(tag);
	      if (!ignoreCurrentElement && validElements[tag] === true && voidElements[tag] !== true) {
	        out('</');
	        out(tag);
	        out('>');
	      }
	      if (tag == ignoreCurrentElement) {
	        ignoreCurrentElement = false;
	      }
	    },
	    chars: function(chars) {
	      if (!ignoreCurrentElement) {
	        out(encodeEntities(chars));
	      }
	    }
	  };
	}
	
	
	/**
	 * When IE9-11 comes across an unknown namespaced attribute e.g. 'xlink:foo' it adds 'xmlns:ns1' attribute to declare
	 * ns1 namespace and prefixes the attribute with 'ns1' (e.g. 'ns1:xlink:foo'). This is undesirable since we don't want
	 * to allow any of these custom attributes. This method strips them all.
	 *
	 * @param node Root element to process
	 */
	function stripCustomNsAttrs(node) {
	  if (node.nodeType === Node.ELEMENT_NODE) {
	    var attrs = node.attributes;
	    for (var i = 0, l = attrs.length; i < l; i++) {
	      var attrNode = attrs[i];
	      var attrName = attrNode.name.toLowerCase();
	      if (attrName === 'xmlns:ns1' || attrName.indexOf('ns1:') === 0) {
	        node.removeAttributeNode(attrNode);
	        i--;
	        l--;
	      }
	    }
	  }
	
	  var nextNode = node.firstChild;
	  if (nextNode) {
	    stripCustomNsAttrs(nextNode);
	  }
	
	  nextNode = node.nextSibling;
	  if (nextNode) {
	    stripCustomNsAttrs(nextNode);
	  }
	}
	
	
	
	// define ngSanitize module and register $sanitize service
	angular.module('ngSanitize', []).provider('$sanitize', $SanitizeProvider);
	
	/* global sanitizeText: false */
	
	/**
	 * @ngdoc filter
	 * @name linky
	 * @kind function
	 *
	 * @description
	 * Finds links in text input and turns them into html links. Supports `http/https/ftp/mailto` and
	 * plain email address links.
	 *
	 * Requires the {@link ngSanitize `ngSanitize`} module to be installed.
	 *
	 * @param {string} text Input text.
	 * @param {string} target Window (`_blank|_self|_parent|_top`) or named frame to open links in.
	 * @param {object|function(url)} [attributes] Add custom attributes to the link element.
	 *
	 *    Can be one of:
	 *
	 *    - `object`: A map of attributes
	 *    - `function`: Takes the url as a parameter and returns a map of attributes
	 *
	 *    If the map of attributes contains a value for `target`, it overrides the value of
	 *    the target parameter.
	 *
	 *
	 * @returns {string} Html-linkified and {@link $sanitize sanitized} text.
	 *
	 * @usage
	   <span ng-bind-html="linky_expression | linky"></span>
	 *
	 * @example
	   <example module="linkyExample" deps="angular-sanitize.js">
	     <file name="index.html">
	       <div ng-controller="ExampleController">
	       Snippet: <textarea ng-model="snippet" cols="60" rows="3"></textarea>
	       <table>
	         <tr>
	           <th>Filter</th>
	           <th>Source</th>
	           <th>Rendered</th>
	         </tr>
	         <tr id="linky-filter">
	           <td>linky filter</td>
	           <td>
	             <pre>&lt;div ng-bind-html="snippet | linky"&gt;<br>&lt;/div&gt;</pre>
	           </td>
	           <td>
	             <div ng-bind-html="snippet | linky"></div>
	           </td>
	         </tr>
	         <tr id="linky-target">
	          <td>linky target</td>
	          <td>
	            <pre>&lt;div ng-bind-html="snippetWithSingleURL | linky:'_blank'"&gt;<br>&lt;/div&gt;</pre>
	          </td>
	          <td>
	            <div ng-bind-html="snippetWithSingleURL | linky:'_blank'"></div>
	          </td>
	         </tr>
	         <tr id="linky-custom-attributes">
	          <td>linky custom attributes</td>
	          <td>
	            <pre>&lt;div ng-bind-html="snippetWithSingleURL | linky:'_self':{rel: 'nofollow'}"&gt;<br>&lt;/div&gt;</pre>
	          </td>
	          <td>
	            <div ng-bind-html="snippetWithSingleURL | linky:'_self':{rel: 'nofollow'}"></div>
	          </td>
	         </tr>
	         <tr id="escaped-html">
	           <td>no filter</td>
	           <td><pre>&lt;div ng-bind="snippet"&gt;<br>&lt;/div&gt;</pre></td>
	           <td><div ng-bind="snippet"></div></td>
	         </tr>
	       </table>
	     </file>
	     <file name="script.js">
	       angular.module('linkyExample', ['ngSanitize'])
	         .controller('ExampleController', ['$scope', function($scope) {
	           $scope.snippet =
	             'Pretty text with some links:\n'+
	             'http://angularjs.org/,\n'+
	             'mailto:us@somewhere.org,\n'+
	             'another@somewhere.org,\n'+
	             'and one more: ftp://127.0.0.1/.';
	           $scope.snippetWithSingleURL = 'http://angularjs.org/';
	         }]);
	     </file>
	     <file name="protractor.js" type="protractor">
	       it('should linkify the snippet with urls', function() {
	         expect(element(by.id('linky-filter')).element(by.binding('snippet | linky')).getText()).
	             toBe('Pretty text with some links: http://angularjs.org/, us@somewhere.org, ' +
	                  'another@somewhere.org, and one more: ftp://127.0.0.1/.');
	         expect(element.all(by.css('#linky-filter a')).count()).toEqual(4);
	       });
	
	       it('should not linkify snippet without the linky filter', function() {
	         expect(element(by.id('escaped-html')).element(by.binding('snippet')).getText()).
	             toBe('Pretty text with some links: http://angularjs.org/, mailto:us@somewhere.org, ' +
	                  'another@somewhere.org, and one more: ftp://127.0.0.1/.');
	         expect(element.all(by.css('#escaped-html a')).count()).toEqual(0);
	       });
	
	       it('should update', function() {
	         element(by.model('snippet')).clear();
	         element(by.model('snippet')).sendKeys('new http://link.');
	         expect(element(by.id('linky-filter')).element(by.binding('snippet | linky')).getText()).
	             toBe('new http://link.');
	         expect(element.all(by.css('#linky-filter a')).count()).toEqual(1);
	         expect(element(by.id('escaped-html')).element(by.binding('snippet')).getText())
	             .toBe('new http://link.');
	       });
	
	       it('should work with the target property', function() {
	        expect(element(by.id('linky-target')).
	            element(by.binding("snippetWithSingleURL | linky:'_blank'")).getText()).
	            toBe('http://angularjs.org/');
	        expect(element(by.css('#linky-target a')).getAttribute('target')).toEqual('_blank');
	       });
	
	       it('should optionally add custom attributes', function() {
	        expect(element(by.id('linky-custom-attributes')).
	            element(by.binding("snippetWithSingleURL | linky:'_self':{rel: 'nofollow'}")).getText()).
	            toBe('http://angularjs.org/');
	        expect(element(by.css('#linky-custom-attributes a')).getAttribute('rel')).toEqual('nofollow');
	       });
	     </file>
	   </example>
	 */
	angular.module('ngSanitize').filter('linky', ['$sanitize', function($sanitize) {
	  var LINKY_URL_REGEXP =
	        /((ftp|https?):\/\/|(www\.)|(mailto:)?[A-Za-z0-9._%+-]+@)\S*[^\s.;,(){}<>"\u201d\u2019]/i,
	      MAILTO_REGEXP = /^mailto:/i;
	
	  var linkyMinErr = angular.$$minErr('linky');
	  var isString = angular.isString;
	
	  return function(text, target, attributes) {
	    if (text == null || text === '') return text;
	    if (!isString(text)) throw linkyMinErr('notstring', 'Expected string but received: {0}', text);
	
	    var match;
	    var raw = text;
	    var html = [];
	    var url;
	    var i;
	    while ((match = raw.match(LINKY_URL_REGEXP))) {
	      // We can not end in these as they are sometimes found at the end of the sentence
	      url = match[0];
	      // if we did not match ftp/http/www/mailto then assume mailto
	      if (!match[2] && !match[4]) {
	        url = (match[3] ? 'http://' : 'mailto:') + url;
	      }
	      i = match.index;
	      addText(raw.substr(0, i));
	      addLink(url, match[0].replace(MAILTO_REGEXP, ''));
	      raw = raw.substring(i + match[0].length);
	    }
	    addText(raw);
	    return $sanitize(html.join(''));
	
	    function addText(text) {
	      if (!text) {
	        return;
	      }
	      html.push(sanitizeText(text));
	    }
	
	    function addLink(url, text) {
	      var key;
	      html.push('<a ');
	      if (angular.isFunction(attributes)) {
	        attributes = attributes(url);
	      }
	      if (angular.isObject(attributes)) {
	        for (key in attributes) {
	          html.push(key + '="' + attributes[key] + '" ');
	        }
	      } else {
	        attributes = {};
	      }
	      if (angular.isDefined(target) && !('target' in attributes)) {
	        html.push('target="',
	                  target,
	                  '" ');
	      }
	      html.push('href="',
	                url.replace(/"/g, '&quot;'),
	                '">');
	      addText(text);
	      html.push('</a>');
	    }
	  };
	}]);
	
	
	})(window, window.angular);


/***/ }

});
//# sourceMappingURL=data:application/json;base64,eyJ2ZXJzaW9uIjozLCJzb3VyY2VzIjpbIndlYnBhY2s6Ly8vLi9jcmVhdGVMaWIuanMiLCJ3ZWJwYWNrOi8vLy4uL2NyZWF0ZUxpYi5odG1sIiwid2VicGFjazovLy8uLi9zdHlsZXMvY3JlYXRlTGliLnNjc3MiLCJ3ZWJwYWNrOi8vLy9Vc2Vycy94dWdlL3Byb2plY3QvcXVhbnNoaS11aS9+L3VuZGVyc2NvcmUvdW5kZXJzY29yZS5qcz82ZjE0Iiwid2VicGFjazovLy8vVXNlcnMveHVnZS9wcm9qZWN0L3F1YW5zaGktdWkvfi91bmRlcnNjb3JlL3VuZGVyc2NvcmUuanM/YWNmZCIsIndlYnBhY2s6Ly8vL1VzZXJzL3h1Z2UvcHJvamVjdC9xdWFuc2hpLXVpL34vYW5ndWxhci11aS1ib290c3RyYXAvaW5kZXguanMiLCJ3ZWJwYWNrOi8vLy9Vc2Vycy94dWdlL3Byb2plY3QvcXVhbnNoaS11aS9+L2FuZ3VsYXItdWktYm9vdHN0cmFwL3VpLWJvb3RzdHJhcC10cGxzLmpzIiwid2VicGFjazovLy8vVXNlcnMveHVnZS9wcm9qZWN0L3F1YW5zaGktdWkvfi9hbmd1bGFyLXNhbml0aXplL2luZGV4LmpzIiwid2VicGFjazovLy8vVXNlcnMveHVnZS9wcm9qZWN0L3F1YW5zaGktdWkvfi9hbmd1bGFyLXNhbml0aXplL2FuZ3VsYXItc2FuaXRpemUuanMiXSwibmFtZXMiOltdLCJtYXBwaW5ncyI6Ijs7Ozs7QUFBQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7O0FBRUE7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0Esb0JBQW1CO0FBQ25CO0FBQ0E7QUFDQSxrQkFBaUI7O0FBRWpCO0FBQ0E7QUFDQTtBQUNBOztBQUVBOztBQUVBO0FBQ0E7QUFDQTtBQUNBLGtCQUFpQjs7QUFFakI7O0FBRUE7QUFDQTtBQUNBO0FB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QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0Esd0JBQXVCLGdCQUFnQjtBQUN2QztBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLHdCQUF1QixnQkFBZ0I7QUFDdkM7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsMkNBQTBDLFlBQVk7QUFDdEQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFFBQU87QUFDUDtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsMkNBQTBDLFlBQVk7QUFDdEQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFFBQU87QUFDUDtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsOEJBQTZCLGdCQUFnQjtBQUM3QztBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBLHFEQUFvRDtBQUNwRCxJQUFHOztBQUVIO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsSUFBRzs7QUFFSDtBQUNBO0FBQ0E7QUFDQTtBQUNBLDJDQUEwQztBQUMxQyxJQUFHOztBQUVIO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQSw2REFBNEQsWUFBWTtBQUN4RTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSwrQ0FBOEMsWUFBWTtBQUMxRDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsUUFBTztBQUNQO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsUUFBTztBQUNQO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsK0NBQThDLFlBQVk7QUFDMUQ7QUFDQTtBQUNBLHNCQUFxQixnQkFBZ0I7QUFDckM7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUEsd0JBQXVCLGdCQUFnQjtBQUN2QztBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsOENBQTZDLFlBQVk7QUFDekQ7QUFDQTtBQUNBLFFBQU87QUFDUDtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLGFBQVksOEJBQThCO0FBQzFDO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSx1REFBc0Q7QUFDdEQ7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsVUFBUztBQUNUO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSwyQ0FBMEMsMEJBQTBCO0FBQ3BFO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBOztBQUVBLHNCQUFxQixjQUFjO0FBQ25DO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLHNCQUFxQixZQUFZO0FBQ2pDO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLGdCQUFlLFlBQVk7QUFDM0I7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPLGVBQWU7QUFDdEI7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBO0FBQ0EsUUFBTztBQUNQO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7O0FBRUE7QUFDQSxzQkFBcUIsZUFBZTtBQUNwQztBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0Esb0JBQW1CLFlBQVk7QUFDL0I7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsdUJBQXNCO0FBQ3RCO0FBQ0EsMEJBQXlCLGdCQUFnQjtBQUN6QztBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxvQkFBbUIsWUFBWTtBQUMvQjtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLDBDQUF5QyxZQUFZO0FBQ3JEO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLDBDQUF5QyxZQUFZO0FBQ3JEO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQSxvQkFBbUI7QUFDbkI7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQSw2Q0FBNEMsbUJBQW1CO0FBQy9EO0FBQ0E7QUFDQSwwQ0FBeUMsWUFBWTtBQUNyRDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBLHNEQUFxRDtBQUNyRDs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0Esb0JBQW1CLFlBQVk7QUFDL0I7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7O0FBR0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsOEVBQTZFO0FBQzdFO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsSUFBRzs7QUFFSDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FB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QUFDQTtBQUNBO0FBQ0EsRUFBQzs7QUFFRDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPOztBQUVQO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0Esa0VBQWlFO0FBQ2pFLGdDQUErQixpREFBaUQsRUFBRTtBQUNsRjtBQUNBO0FBQ0E7QUFDQTtBQUNBLFFBQU87QUFDUDtBQUNBO0FBQ0EsRUFBQzs7QUFFRDs7QUFFQTs7QUFFQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0wsSUFBRzs7QUFFSDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsSUFBRzs7QUFFSDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsUUFBTztBQUNQO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsUUFBTztBQUNQO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsUUFBTztBQUNQO0FBQ0E7QUFDQTtBQUNBOztBQUVBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsVUFBUzs7QUFFVDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxJQUFHOztBQUVIO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtB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Q1QsUUFBTztBQUNQO0FBQ0E7QUFDQSxVQUFTO0FBQ1Q7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0EsSUFBRzs7QUFFSDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsZ0JBQWUsbUJBQW1CO0FBQ2xDO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBLElBQUc7O0FBRUgsRUFBQzs7QUFFRDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsWUFBVyxRQUFRO0FBQ25CLFlBQVcsU0FBUztBQUNwQixZQUFXLFNBQVM7QUFDcEI7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0Esc0VBQXFFO0FBQ3JFO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxzRUFBcUU7QUFDckU7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsOEJBQTZCLHdEQUF3RDtBQUNyRjtBQUNBLFlBQVcsU0FBUztBQUNwQixZQUFXLFFBQVE7QUFDbkI7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxzQkFBcUIsYUFBYSxxQkFBcUI7QUFDdkQ7QUFDQSxxQkFBb0IsUUFBUTtBQUM1QixjQUFhLFlBQVk7QUFDekI7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPOztBQUVQO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsUUFBTztBQUNQO0FBQ0E7QUFDQSxFQUFDOztBQUVEO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQSxpQ0FBZ0MseUJBQXlCO0FBQ3pEO0FBQ0E7QUFDQSxVQUFTO0FBQ1Q7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFlBQVc7QUFDWDs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQSxpQ0FBZ0MseUJBQXlCO0FBQ3pEO0FBQ0E7QUFDQSxVQUFTO0FBQ1Q7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFlBQVc7QUFDWDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsRUFBQzs7QUFFRDs7QUFFQTs7QUFFQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQSxJQUFHO0FBQ0gsRUFBQzs7QUFFRDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxFQUFDOztBQUVEO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsUUFBTzs7QUFFUDtBQUNBO0FBQ0E7QUFDQTtBQUNBLFFBQU87QUFDUDtBQUNBO0FBQ0EsRUFBQzs7QUFFRDs7QUFFQTtBQUNBO0FBQ0EsK0RBQThELEVBQUU7O0FBRWhFO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTs7QUFFQTtBQUNBO0FBQ0EscUJBQW9CLEVBQUU7QUFDdEIsaUNBQWdDLG9CQUFvQjtBQUNwRCxRQUFPO0FBQ1A7QUFDQSxxQkFBb0IsRUFBRTtBQUN0QixpQ0FBZ0MsMkJBQTJCO0FBQzNELFFBQU87QUFDUDtBQUNBLHFCQUFvQixJQUFJO0FBQ3hCLGlDQUFnQyxvQkFBb0I7QUFDcEQsUUFBTztBQUNQO0FBQ0E7QUFDQSxpQ0FBZ0MsNERBQTREO0FBQzVGLFFBQU87QUFDUDtBQUNBO0FBQ0EsaUNBQWdDLGlFQUFpRTtBQUNqRyxRQUFPO0FBQ1A7QUFDQTtBQUNBLGlDQUFnQyx3QkFBd0I7QUFDeEQsUUFBTztBQUNQO0FBQ0E7QUFDQSxpQ0FBZ0Msd0JBQXdCO0FBQ3hELFFBQU87QUFDUDtBQUNBLDRCQUEyQixFQUFFLFFBQVEsRUFBRTtBQUN2QyxpQ0FBZ0Msb0JBQW9CO0FBQ3BELFFBQU87QUFDUDtBQUNBLDZCQUE0QixFQUFFLFFBQVEsRUFBRTtBQUN4QyxpQ0FBZ0Msb0JBQW9CO0FBQ3BELFFBQU87QUFDUDtBQUNBO0FBQ0EsUUFBTztBQUNQO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7QUFDQTtBQUNBLGlDQUFnQyxxQkFBcUI7QUFDckQsUUFBTztBQUNQO0FBQ0E7QUFDQSxpQ0FBZ0MscUJBQXFCO0FBQ3JELFFBQU87QUFDUDtBQUNBO0FBQ0EsaUNBQWdDLHFCQUFxQjtBQUNyRCxRQUFPO0FBQ1A7QUFDQTtBQUNBLGlDQUFnQyxxQkFBcUI7QUFDckQsUUFBTztBQUNQO0FBQ0E7QUFDQSxpQ0FBZ0MsdUJBQXVCO0FBQ3ZELFFBQU87QUFDUDtBQUNBO0FBQ0EsaUNBQWdDLHVCQUF1QjtBQUN2RCxRQUFPO0FBQ1A7QUFDQTtBQUNBLGlDQUFnQyw0QkFBNEI7QUFDNUQsUUFBTztBQUNQO0FBQ0E7QUFDQSxpQ0FBZ0MsdUJBQXVCO0FBQ3ZELFFBQU87QUFDUDtBQUNBO0FBQ0EsaUNBQWdDLHVCQUF1QjtBQUN2RCxRQUFPO0FBQ1A7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBOztBQUVBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0EsNkJBQTRCO0FBQzVCLHlEQUF3RCxPQUFPO0FBQy9EO0FBQ0E7QUFDQTtBQUNBOztBQUVBLG1CQUFrQixrQ0FBa0M7QUFDcEQ7QUFDQSxNQUFLOztBQUVMO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFFBQU87QUFDUDtBQUNBO0FBQ0E7QUFDQSxtQkFBa0I7QUFDbEI7O0FBRUEsMENBQXlDLE9BQU87QUFDaEQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxVQUFTO0FBQ1Q7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7O0FBRUE7O0FBRUE7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQSxFQUFDOztBQUVEOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSw2QkFBNEI7QUFDNUI7QUFDQSxRQUFPO0FBQ1A7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxnQ0FBK0I7QUFDL0I7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsUUFBTzs7QUFFUDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPOztBQUVQO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBLFlBQVc7QUFDWDtBQUNBO0FBQ0EsWUFBVztBQUNYO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBLFlBQVc7QUFDWDtBQUNBO0FBQ0EsWUFBVztBQUNYO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0EsSUFBRzs7QUFFSDs7QUFFQTs7QUFFQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBLEVBQUM7O0FBRUQ7O0FBRUE7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxFQUFDOztBQUVEO0FBQ0E7QUFDQSxzQkFBcUIsK0JBQStCOztBQUVwRDtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsSUFBRzs7QUFFSDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPO0FBQ1AsTUFBSztBQUNMO0FBQ0E7QUFDQSxJQUFHOztBQUVIO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPO0FBQ1AsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBLElBQUc7O0FBRUg7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTCxJQUFHO0FBQ0g7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFD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QTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0EsRUFBQzs7QUFFRDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsSUFBRztBQUNIO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsRUFBQzs7QUFFRDtBQUNBO0FBQ0E7QUFDQSxpQkFBZ0I7QUFDaEI7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7O0FBRUE7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLOztBQUVMO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsUUFBTztBQUNQOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxVQUFTO0FBQ1Q7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxZQUFXO0FBQ1g7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBLHlEQUF3RCx5QkFBeUI7QUFDakY7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0EsdURB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QUFFQTtBQUNBLGdDQUErQix3Q0FBd0M7QUFDdkU7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7O0FBRUE7QUFDQSxrQkFBaUI7O0FBRWpCO0FBQ0E7QUFDQTs7QUFFQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0EsZ0JBQWU7QUFDZjtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsRUFBQzs7QUFFRDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7O0FBRUE7QUFDQTtBQUNBO0FBQ0EsRUFBQzs7QUFFRDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQSxFQUFDOztBQUVEO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQSxFQUFDOztBQUVEO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQSxFQUFDOztBQUVEO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxFQUFDOztBQUVEOztBQUVBO0FBQ0E7QUFDQSxFQUFDOztBQUVEO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQSxzQkFBcUIsUUFBUTs7QUFFN0IsMkRBQTBELFNBQVM7O0FBRW5FO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7O0FBR0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBLFFBQU87QUFDUDs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsUUFBTztBQUNQO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBLHlDQUF3QztBQUN4Qzs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsVUFBUztBQUNUO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFVBQVM7QUFDVDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7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Q0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0EsY0FBYTtBQUNiO0FBQ0EsY0FBYTtBQUNiLFlBQVc7QUFDWDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsZ0JBQWU7QUFDZjtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxnQkFBZTtBQUNmO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPOztBQUVQO0FBQ0E7QUFDQTs7QUFFQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsVUFBUzs7QUFFVDs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFVBQVM7QUFDVDtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtB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QTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSzs7QUFFTDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsVUFBUztBQUNUO0FBQ0E7QUFDQSxJQUFHOztBQUVIO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQSxNQUFLOztBQUVMO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBLFFBQU87QUFDUCxJQUFHOztBQUVIO0FBQ0E7QUFDQTtBQUNBLHNCQUFxQiw4QkFBOEI7QUFDbkQ7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFFBQU87QUFDUCxNQUFLO0FBQ0w7QUFDQTs7QUFFQTtBQUNBO0FBQ0EsTUFBSzs7QUFFTDtBQUNBLDBDQUF5Qzs7QUFFekM7QUFDQTtBQUNBLFFBQU87QUFDUDtBQUNBO0FBQ0EsTUFBSztBQUNMOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQSxFQUFDOztBQUVEO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7O0FBRUE7QUFDQSxpQkFBZ0I7QUFDaEI7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFVBQVM7QUFDVDs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsWUFBVztBQUNYO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQSxxQ0FBb0MsbUJBQW1CO0FBQ3ZEO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTs7QUFFQTtBQUNBLGlCQUFnQjtBQUNoQjs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0Esb0JBQW1CLDhCQUE4QjtBQUNqRDs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsUUFBTztBQUNQLE1BQUs7QUFDTDtBQUNBOztBQUVBO0FBQ0E7QUFDQSxNQUFLOztBQUVMO0FBQ0EsMENBQXlDOztBQUV6QztBQUNBO0FBQ0EsUUFBTztBQUNQO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FB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QTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsZ0JBQWU7QUFDZjtBQUNBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLGdCQUFlO0FBQ2Y7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxnQkFBZTtBQUNmOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0Esa0JBQWlCO0FBQ2pCO0FBQ0E7QUFDQSxnQkFBZTtBQUNmOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQSxrQkFBaUI7QUFDakI7QUFDQTtBQUNBLGdCQUFlOztBQUVmO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxnQkFBZTtBQUNmO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBLGNBQWE7O0FBRWI7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxnQkFBZTtBQUNmOztBQUVBO0FB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QUFDQSxFQUFDOztBQUVEO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0EsMkVBQTBFLG1CQUFtQjtBQUM3RjtBQUNBO0FBQ0EsRUFBQzs7QUFFRDs7QUFFQTs7QUFFQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBLG9CQUFtQixxQkFBcUI7QUFDeEM7O0FBRUE7O0FBRUE7QUFDQTs7QUFFQTtBQUNBO0FBQ0EsTUFBSzs7QUFFTDtBQUNBOztBQUVBO0FBQ0E7QUFDQSxRQUFPOztBQUVQO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0wsSUFBRztBQUNILEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsRUFBQzs7QUFFRDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxFQUFDOztBQUVEO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsMkVBQTBFLG1CQUFtQjtBQUM3RjtBQUNBO0FBQ0EsRUFBQzs7QUFFRDs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsRUFBQzs7QUFFRDtBQUNBLHVCQUFzQjs7QUFFdEI7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLOztBQUVMO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBLHVDQUFzQyxPQUFPO0FBQzdDLG1DQUFrQyxXQUFXLEdBQUcsMEVBQTBFO0FBQzFIO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLHFCQUFvQixhQUFhO0FBQ2pDOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7O0FBRUE7O0FBRUE7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0EsSUFBRztBQUNILEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7OztBQUdEO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQSxNQUFLO0FBQ0w7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBLElBQUc7QUFDSCxFQUFDOztBQUVEO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxZQUFXLFNBQVM7QUFDcEIsWUFBVyxTQUFTO0FBQ3BCO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFlBQVcsUUFBUSw0RUFBNEUsd0RBQXdEO0FBQ3ZKLFlBQVcsUUFBUTtBQUNuQixZQUFXLFNBQVM7QUFDcEIsWUFBVyxTQUFTO0FBQ3BCO0FBQ0E7QUFDQSx1RUFBc0UsZ0RBQWdEO0FBQ3RIO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0Esc0JBQXFCLFlBQVk7QUFDakM7QUFDQTtBQUNBLGFBQVk7QUFDWjtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFVBQVMsb0RBQW9EO0FBQzdELFVBQVM7QUFDVDs7QUFFQTtBQUNBO0FBQ0E7QUFDQSxVQUFTO0FBQ1Q7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxtQ0FBa0MsMENBQTBDO0FBQzVFO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPOztBQUVQO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsVUFBUztBQUNUOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0EsUUFBTzs7QUFFUDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsRUFBQzs7QUFFRDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFFBQU87QUFDUDtBQUNBO0FBQ0EsRUFBQzs7QUFFRDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxZQUFXO0FBQ1g7QUFDQTtBQUNBLFVBQVM7QUFDVCxRQUFPO0FBQ1A7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7O0FBRUE7O0FBRUE7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBLE1BQUs7QUFDTCxJQUFHOztBQUVIO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsUUFBTztBQUNQO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLElBQUc7O0FBRUg7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7QUFDQTtBQUNBLFFBQU87QUFDUDtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFVBQVM7O0FBRVQ7QUFDQTtBQUNBO0FBQ0E7QUFDQSxZQUFXO0FBQ1g7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQSxVQUFTOztBQUVUO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxJQUFHOztBQUVIO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFVBQVM7QUFDVDtBQUNBO0FBQ0EsSUFBRzs7QUFFSDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxZQUFXO0FBQ1gsVUFBUztBQUNUO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxJQUFHOztBQUVIOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7QUFDQTtBQUNBLHNCQUFxQiw4QkFBOEI7QUFDbkQ7O0FBRUE7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBLE1BQUs7O0FBRUw7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQSxJQUFHOztBQUVIO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsSUFBRzs7QUFFSDtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFFBQU87QUFDUDtBQUNBO0FBQ0EsTUFBSztBQUNMOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQSxNQUFLOztBQUVMO0FBQ0E7QUFDQTtBQUNBLE1BQUs7O0FBRUw7O0FBRUE7QUFDQTtBQUNBO0FBQ0EsNEJBQTJCO0FBQzNCO0FBQ0E7QUFDQTtBQUNBLFFBQU8sMkJBQTJCO0FBQ2xDO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSzs7QUFFTDtBQUNBLDRCQUEyQjtBQUMzQjtBQUNBO0FBQ0E7QUFDQSxRQUFPLDJCQUEyQjtBQUNsQztBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7QUFDTDs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsVUFBUztBQUNUO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsVUFBUztBQUNUO0FBQ0EsTUFBSzs7QUFFTDtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQSxVQUFTO0FBQ1Q7QUFDQTtBQUNBLFFBQU87QUFDUDtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQSxVQUFTO0FBQ1Q7QUFDQSxNQUFLOztBQUVMOztBQUVBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFFBQU87QUFDUDtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7O0FBRUE7QUFDQSwrREFBOEQ7QUFDOUQ7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxjQUFhO0FBQ2I7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxFQUFDOztBQUVEOztBQUVBOztBQUVBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsSUFBRztBQUNILEVBQUM7O0FBRUQ7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxjQUFhO0FBQ2I7QUFDQTtBQUNBLE1BQUs7QUFDTDtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxFQUFDOztBQUVEOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsSUFBRzs7QUFFSDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTs7QUFFQTs7QUFFQTs7QUFFQTs7QUFFQTtBQUNBOztBQUVBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLDBDQUF5QyxlQUFlO0FBQ3hELFFBQU87QUFDUDtBQUNBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSztBQUNMOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLE1BQUs7O0FBRUw7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxNQUFLO0FBQ0w7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7QUFDQTtBQUNBLE1BQUs7O0FBRUw7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBLHFCQUFvQjtBQUNwQjtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0EsNEJBQTJCLG9CQUFvQjtBQUMvQztBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsZ0JBQWU7QUFDZjs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsWUFBVztBQUNYO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsUUFBTztBQUNQO0FBQ0E7QUFDQTtBQUNBLFFBQU87QUFDUDs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBLFFBQU87QUFDUDs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTs7QUFFQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBLFFBQU87QUFDUDs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFFBQU87O0FBRVA7O0FBRUE7QUFDQTtBQUNBO0FBQ0EsOEJBQTZCLG9CQUFvQixFQUFFO0FBQ25EO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7QUFDQTtBQUNBLFFBQU87QUFDUDtBQUNBO0FBQ0EsVUFBUztBQUNULFFBQU87QUFDUDs7QUFFQTtBQUNBO0FBQ0E7QUFDQSxNQUFLOztBQUVMO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxVQUFTO0FBQ1Q7QUFDQTtBQUNBO0FBQ0EsTUFBSzs7QUFFTDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsTUFBSzs7QUFFTDs7QUFFQTtBQUNBO0FBQ0EsTUFBSztBQUNMO0FBQ0EsTUFBSztBQUNMO0FBQ0E7O0FBRUE7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsWUFBVztBQUNYO0FBQ0E7QUFDQSxVQUFTO0FBQ1Q7QUFDQTtBQUNBO0FBQ0E7O0FBRUE7QUFDQTtBQUNBLFVBQVM7QUFDVDtBQUNBO0FBQ0E7QUFDQTtBQUNBLFlBQVc7QUFDWDtBQUNBO0FBQ0E7QUFDQTtBQUNBLFFBQU87O0FBRVA7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQSxVQUFTO0FBQ1Q7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQSxRQUFPO0FBQ1A7QUFDQSxJQUFHOztBQUVIO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxJQUFH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QUFDQTtBQUNBLHNCQUFxQix3REFBd0QsZ0JBQWdCLEtBQUssUUFBUSxRQUFRO0FBQ2xIO0FBQ0E7QUFDQSxzRUFBcUUsZ0JBQWdCO0FBQ3JGO0FBQ0E7QUFDQTtBQUNBO0FBQ0EseUJBQXdCLHlCQUF5QixRQUFRLFFBQVE7QUFDakU7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFdBQVU7QUFDVjtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFNBQVE7O0FBRVI7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFNBQVE7O0FBRVI7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFNBQVE7O0FBRVI7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFNBQVE7O0FBRVI7QUFDQTtBQUNBLHVFQUFzRSxnQkFBZ0I7QUFDdEY7QUFDQTtBQUNBLFNBQVE7QUFDUjtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsMkVBQTBFLEtBQUs7QUFDL0U7O0FBRUE7QUFDQTs7QUFFQTtBQUNBO0FBQ0EseUZBQXdGLEVBQUU7O0FBRTFGO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQUVBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUFFQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBLFFBQU87QUFDUDtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EsMENBQXlDO0FBQ3pDO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSxFQUFDOzs7QUFHRCxFQUFDIiwiZmlsZSI6InNjcmlwdHMvY3JlYXRlTGliLWJ1bmRsZS5qcyIsInNvdXJjZXNDb250ZW50IjpbIi8v5YWl5Y+j5paH5Lu2XG5yZXF1aXJlKCcuLi9jcmVhdGVMaWIuaHRtbCcpO1xuXG5yZXF1aXJlKCdmb250LWF3ZXNvbWUvY3NzL2ZvbnQtYXdlc29tZS5jc3MnKTtcbnJlcXVpcmUoJ2Jvb3RzdHJhcC9kaXN0L2Nzcy9ib290c3RyYXAuY3NzJyk7XG5yZXF1aXJlKCcuLi9zdHlsZXMvY3JlYXRlTGliLnNjc3MnKTtcblxucmVxdWlyZSgnLi9jb21tb24nKTtcbnJlcXVpcmUoJy4vZ2tDbGllbnRDYWxsYmFjay5qcycpO1xuXG4vL+esrOS4ieaWueS+nei1luaooeWdl1xucmVxdWlyZSgnZXhwb3NlP18hdW5kZXJzY29yZScpO1xucmVxdWlyZSgnYW5ndWxhci11aS1ib290c3RyYXAnKTtcbnJlcXVpcmUoJ2FuZ3VsYXItc2FuaXRpemUnKTtcbnJlcXVpcmUoJ2V4cG9zZT8kIWV4cG9zZT9qUXVlcnkhanF1ZXJ5Jyk7XG5cbmFuZ3VsYXIubW9kdWxlKCdnb3VrdWFpLmNyZWF0ZUxpYicsIFtcbiAgICAgICAgJ3VpLmJvb3RzdHJhcCcsXG4gICAgICAgICdnb3VrdWFpLmNvbW1vbidcbiAgICBdKVxuXG4gICAgLmNvbnRyb2xsZXIoJ2luaXRDdHJsJywgWyckc2NvcGUnLCAnJHEnLCAnZ2tBcGknLCAnZ2tFeGNlcHRpb24nLCAnJHdpbmRvdycsIGZ1bmN0aW9uICgkc2NvcGUsICRxLCBna0FwaSwgZ2tFeGNlcHRpb24sICR3aW5kb3cpIHtcblxuICAgICAgICAvL+WIpOaWreWuouaIt+err+aVsOaNruWKoOi9veWujOavlVxuXG4gICAgICAgIHZhciBsb2FkQ29tcGxldGVDYWxsYmFjayA9IGZ1bmN0aW9uICgpIHtcbiAgICAgICAgICAgIGdrQXBpLmVudEluZm8oKS50aGVuKGZ1bmN0aW9uIChyZSkge1xuICAgICAgICAgICAgICAgIFV0aWwuTG9nLmxvZygnZ2tBcGkuZW50SW5mbzpyZScsIHJlKTtcbiAgICAgICAgICAgICAgICB2YXIgZW50cyA9IHJlICYmIGFuZ3VsYXIuaXNBcnJheShyZS5saXN0KSA/IHJlLmxpc3QgOiBbXTtcbiAgICAgICAgICAgICAgICBpZiAoIWVudHMgfHwgIWVudHMubGVuZ3RoKSB7XG4gICAgICAgICAgICAgICAgICAgICRzY29wZS5lcnJvclRleHQgPSAn5L2g6L+Y5rKh5Yqg5YWl5Lu75L2V5LyB5LiaLOS4jeiDveWIm+W7uuW6kyc7XG4gICAgICAgICAgICAgICAgICAgIHJldHVybjtcbiAgICAgICAgICAgICAgICB9XG5cbiAgICAgICAgICAgICAgICAvKlxuICAgICAgICAgICAgICAgICDov5nph4zlj6/og73ov5Tlm57lpJrkuKrkvIHkuJos5YWo5pe26L+Z6L655Y+q5ou/5LiA5LiqXG4gICAgICAgICAgICAgICAgICovXG4gICAgICAgICAgICAgICAgLy8kc2NvcGUuZW50cyA9IF8ud2hlcmUoZW50cywge1xuICAgICAgICAgICAgICAgIC8vICAgIGVuYWJsZV9jcmVhdGVfb3JnOiAxXG4gICAgICAgICAgICAgICAgLy99KTtcbiAgICAgICAgICAgICAgICB2YXIgZW50ID0gXy5maW5kV2hlcmUoZW50cywge1xuICAgICAgICAgICAgICAgICAgICBlbmFibGVfY3JlYXRlX29yZzogMVxuICAgICAgICAgICAgICAgIH0pO1xuXG4gICAgICAgICAgICAgICAgaWYgKCFlbnQpIHtcbiAgICAgICAgICAgICAgICAgICAgJHNjb3BlLmVycm9yVGV4dCA9ICfkvaDov5jmsqHliqDlhaXku7vkvZXkvIHkuJos5LiN6IO95Yib5bu65bqTJztcbiAgICAgICAgICAgICAgICAgICAgcmV0dXJuO1xuICAgICAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgICAgICRzY29wZS5lbnRzID0gW2VudF07XG5cbiAgICAgICAgICAgICAgICB2YXIgcHJvbWlzZUFyciA9IFtdO1xuICAgICAgICAgICAgICAgIGFuZ3VsYXIuZm9yRWFjaCgkc2NvcGUuZW50cywgZnVuY3Rpb24gKGVudCkge1xuICAgICAgICAgICAgICAgICAgICBwcm9taXNlQXJyLnB1c2goZ2tBcGkuZ2V0U3RvcmFnZVBvaW50cyhlbnQuZW50X2lkKSk7XG4gICAgICAgICAgICAgICAgfSk7XG5cbiAgICAgICAgICAgICAgICB2YXIgT25HS0Nsb3VkTGliQ3JlYXRlTG9hZENvbXBsZXRlID0gdG9wLndpbmRvdy51Yy5tb2R1bGVzLmNsb3VkLkNsb3VkTGliQ3JlYXRlLk9uR0tDbG91ZExpYkNyZWF0ZUxvYWRDb21wbGV0ZTtcblxuICAgICAgICAgICAgICAgICRxLmFsbChwcm9taXNlQXJyKS50aGVuKGZ1bmN0aW9uIChyZXQpIHtcbiAgICAgICAgICAgICAgICAgICAgYW5ndWxhci5pc0Z1bmN0aW9uKE9uR0tDbG91ZExpYkNyZWF0ZUxvYWRDb21wbGV0ZSkgJiYgT25HS0Nsb3VkTGliQ3JlYXRlTG9hZENvbXBsZXRlKCk7XG4gICAgICAgICAgICAgICAgICAgIGFuZ3VsYXIuZm9yRWFjaCgkc2NvcGUuZW50cywgZnVuY3Rpb24gKGVudCwgaSkge1xuICAgICAgICAgICAgICAgICAgICAgICAgZW50LnN0b3JhZ2VQb2ludHMgPSByZXRbaV0ubGlzdDtcbiAgICAgICAgICAgICAgICAgICAgfSk7XG5cbiAgICAgICAgICAgICAgICAgICAgJHNjb3BlLmxpYiA9IHtcbiAgICAgICAgICAgICAgICAgICAgICAgIGVudDogJHNjb3BlLmVudHNbMF0sXG4gICAgICAgICAgICAgICAgICAgICAgICBuYW1lOiAnJyxcbiAgICAgICAgICAgICAgICAgICAgICAgIHN0b3JhZ2VQb2ludDogJHNjb3BlLmVudHNbMF0uc3RvcmFnZVBvaW50c1swXVxuICAgICAgICAgICAgICAgICAgICB9O1xuXG4gICAgICAgICAgICAgICAgICAgICRzY29wZS5zaG93U3RvcmFnZVBvaW50ID0gJHNjb3BlLmxpYi5lbnQgJiYgJHNjb3BlLmxpYi5lbnQuc3RvcmFnZVBvaW50cyAmJiAkc2NvcGUubGliLmVudC5zdG9yYWdlUG9pbnRzLmxlbmd0aCA+IDE7XG4gICAgICAgICAgICAgICAgICAgICRzY29wZS5zaG93RW50ID0gJHNjb3BlLmVudHMgJiYgJHNjb3BlLmVudHMubGVuZ3RoID4gMTtcblxuICAgICAgICAgICAgICAgIH0sIGZ1bmN0aW9uICgpIHtcbiAgICAgICAgICAgICAgICAgICAgYW5ndWxhci5pc0Z1bmN0aW9uKE9uR0tDbG91ZExpYkNyZWF0ZUxvYWRDb21wbGV0ZSkgJiYgT25HS0Nsb3VkTGliQ3JlYXRlTG9hZENvbXBsZXRlKCk7XG4gICAgICAgICAgICAgICAgICAgICRzY29wZS5lcnJvclRleHQgPSAn5peg5rOV6I635Y+W5LyB5Lia5a2Y5YKo6IqC54K55pWw5o2uJztcbiAgICAgICAgICAgICAgICB9KTtcbiAgICAgICAgICAgIH0pO1xuICAgICAgICB9O1xuXG4gICAgICAgIGdrQ2xpZW50SW50ZXJmYWNlLm5lZWRMb2FkaW5nKGZ1bmN0aW9uIChyZSkge1xuICAgICAgICAgICAgVXRpbC5Mb2cubG9nKCduZWVkTG9hZGluZycsIHJlKTtcbiAgICAgICAgICAgIGlmIChyZSA9PSAwKSB7XG4gICAgICAgICAgICAgICAgbG9hZENvbXBsZXRlQ2FsbGJhY2soKTtcbiAgICAgICAgICAgIH0gZWxzZSBpZiAocmUgPT0gMSkge1xuICAgICAgICAgICAgICAgIGxvYWRDb21wbGV0ZUNhbGxiYWNrKCk7XG4gICAgICAgICAgICB9XG4gICAgICAgIH0pO1xuXG4gICAgICAgIHZhciBtb3VudHM7XG4gICAgICAgIHZhciBjaGVja01vdW50TmFtZUF2YWlhYmxlID0gZnVuY3Rpb24gKG5hbWUpIHtcbiAgICAgICAgICAgIHZhciBkZWZlcnJlZCA9ICRxLmRlZmVyKCk7XG4gICAgICAgICAgICB2YXIgY2hlY2sgPSBmdW5jdGlvbiAoKSB7XG4gICAgICAgICAgICAgICAgaWYoXy5maW5kV2hlcmUobW91bnRzLCB7XG4gICAgICAgICAgICAgICAgICAgICAgICBuYW1lOiBuYW1lXG4gICAgICAgICAgICAgICAgICAgIH0pKXtcbiAgICAgICAgICAgICAgICAgICAgZGVmZXJyZWQucmVqZWN0KCk7XG4gICAgICAgICAgICAgICAgfWVsc2V7XG4gICAgICAgICAgICAgICAgICAgIGRlZmVycmVkLnJlc29sdmUoKTtcbiAgICAgICAgICAgICAgICB9XG4gICAgICAgICAgICB9O1xuICAgICAgICAgICAgaWYgKGFuZ3VsYXIuaXNEZWZpbmVkKG1vdW50cykpIHtcbiAgICAgICAgICAgICAgICBjaGVjaygpO1xuICAgICAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICAgICAgICBna0FwaS5nZXRNb3VudHMoe1xuICAgICAgICAgICAgICAgICAgICBzaWRldHlwZTonb3JnJ1xuICAgICAgICAgICAgICAgIH0pLnRoZW4oZnVuY3Rpb24gKGRhdGEpIHtcbiAgICAgICAgICAgICAgICAgICAgbW91bnRzID0gZGF0YS5saXN0IHx8IFtdO1xuICAgICAgICAgICAgICAgICAgICBjaGVjaygpO1xuICAgICAgICAgICAgICAgIH0pO1xuICAgICAgICAgICAgfVxuICAgICAgICAgICAgcmV0dXJuIGRlZmVycmVkLnByb21pc2U7XG4gICAgICAgIH07XG5cblxuICAgICAgICAkc2NvcGUuY3JlYXRlTG9hZGluZyA9IGZhbHNlO1xuICAgICAgICAkc2NvcGUuY3JlYXRlTGliID0gZnVuY3Rpb24gKGxpYikge1xuICAgICAgICAgICAgaWYgKCRzY29wZS5jcmVhdGVMb2FkaW5nKSB7XG4gICAgICAgICAgICAgICAgcmV0dXJuO1xuICAgICAgICAgICAgfVxuICAgICAgICAgICAgJHNjb3BlLmNyZWF0ZUxvYWRpbmcgPSB0cnVlO1xuICAgICAgICAgICAgY2hlY2tNb3VudE5hbWVBdmFpYWJsZShsaWIubmFtZSkudGhlbihmdW5jdGlvbigpe1xuICAgICAgICAgICAgICAgIGdrQXBpLmNyZWF0ZUxpYihsaWIuZW50LmVudF9pZCwgbGliLm5hbWUsXG4gICAgICAgICAgICAgICAgICAgIGxpYi5zdG9yYWdlUG9pbnQuc3RvcmFnZV9wb2ludCwgdW5kZWZpbmVkXG4gICAgICAgICAgICAgICAgKS50aGVuKGZ1bmN0aW9uIChyZSkge1xuICAgICAgICAgICAgICAgICAgICBVdGlsLkxvZy5sb2coJ2drQXBpLmVudEluZm86c3VjY2VzczpyZScsIHJlKTtcbiAgICAgICAgICAgICAgICAgICAgdmFyIG1vdW50X2lkID0gcmUubW91bnRfaWQ7XG4gICAgICAgICAgICAgICAgICAgIGdrQXBpLm5vdGljZSh7XG4gICAgICAgICAgICAgICAgICAgICAgICB0eXBlOiAnZ2V0T3JnJyxcbiAgICAgICAgICAgICAgICAgICAgICAgICdvcmdfaWQnOiBOdW1iZXIocmUub3JnX2lkKVxuICAgICAgICAgICAgICAgICAgICB9KS50aGVuKGZ1bmN0aW9uICgpIHtcbiAgICAgICAgICAgICAgICAgICAgICAgIHRvcC53aW5kb3cudWMubW9kdWxlcy5jbG91ZC5na0V2ZW50TGlzdGVuZXIuT25Ha0V2ZW50TGlzdGVuZXIoXCJjcmVhdGVsaWJTdWNjZXNzXCIsIHtcIm1vdW50SWRcIjogbW91bnRfaWR9KTtcbiAgICAgICAgICAgICAgICAgICAgfSk7XG5cbiAgICAgICAgICAgICAgICB9LCBmdW5jdGlvbiAocmVxdWVzdCwgdGV4dFN0YXR1cywgZXJyb3JUaHJvd24pIHtcbiAgICAgICAgICAgICAgICAgICAgVXRpbC5Mb2cubG9nKCdna0FwaS5lbnRJbmZvOmVycm9yOmVycm9ycycsIHJlcXVlc3QsIHRleHRTdGF0dXMsIGVycm9yVGhyb3duKTtcbiAgICAgICAgICAgICAgICAgICAgJHNjb3BlLmNyZWF0ZUxvYWRpbmcgPSBmYWxzZTtcbiAgICAgICAgICAgICAgICAgICAgdmFyIGVycm9yID0gZ2tFeGNlcHRpb24uZ2V0QWpheEVycm9yKHJlcXVlc3QsIHRleHRTdGF0dXMsIGVycm9yVGhyb3duKTtcbiAgICAgICAgICAgICAgICAgICAgd2luZG93LnRvcC51Yy5tb2R1bGVzLmNvbXBvbmVudC5Qcm9tcHRNZXNzYWdlQm94Lm9wZW4oe1xuICAgICAgICAgICAgICAgICAgICAgICAgICAgIHRpdGxlOiAn5o+Q56S6JyxcbiAgICAgICAgICAgICAgICAgICAgICAgICAgICBtZXNzYWdlOiBlcnJvci5tc2csXG4gICAgICAgICAgICAgICAgICAgICAgICAgICAgYnV0dG9uTW9kZTogXCJva1wiLFxuICAgICAgICAgICAgICAgICAgICAgICAgICAgIE9rVGV4dDogJ+ehruWumidcbiAgICAgICAgICAgICAgICAgICAgICAgIH0sXG4gICAgICAgICAgICAgICAgICAgICAgICBmdW5jdGlvbiAocmVzdWx0KSB7XG4gICAgICAgICAgICAgICAgICAgICAgICAgICAgdGhpcy5jbG9zZSgpO1xuICAgICAgICAgICAgICAgICAgICAgICAgfSk7XG4gICAgICAgICAgICAgICAgfSk7XG4gICAgICAgICAgICB9LGZ1bmN0aW9uKCl7XG4gICAgICAgICAgICAgICAgJHNjb3BlLmNyZWF0ZUxvYWRpbmcgPSBmYWxzZTtcbiAgICAgICAgICAgICAgICB3aW5kb3cudG9wLnVjLm1vZHVsZXMuY29tcG9uZW50LlByb21wdE1lc3NhZ2VCb3gub3Blbih7XG4gICAgICAgICAgICAgICAgICAgICAgICB0aXRsZTogJ+aPkOekuicsXG4gICAgICAgICAgICAgICAgICAgICAgICBtZXNzYWdlOiflt7LlrZjlnKjnm7jlkIzlkI3np7Dmlofku7blupMnLFxuICAgICAgICAgICAgICAgICAgICAgICAgYnV0dG9uTW9kZTogXCJva1wiLFxuICAgICAgICAgICAgICAgICAgICAgICAgT2tUZXh0OiAn56Gu5a6aJ1xuICAgICAgICAgICAgICAgICAgICB9LFxuICAgICAgICAgICAgICAgICAgICBmdW5jdGlvbiAocmVzdWx0KSB7XG4gICAgICAgICAgICAgICAgICAgICAgICB0aGlzLmNsb3NlKCk7XG4gICAgICAgICAgICAgICAgICAgIH0pO1xuICAgICAgICAgICAgfSk7XG4gICAgICAgIH07XG5cbiAgICAgICAgJHNjb3BlLmRpc2FibGVDcmVhdGVCdG4gPSB0cnVlO1xuICAgICAgICAkc2NvcGUuJHdhdGNoKCdsaWIubmFtZScsIGZ1bmN0aW9uICh2YWwpIHtcbiAgICAgICAgICAgICRzY29wZS5kaXNhYmxlQ3JlYXRlQnRuID0gIXZhbCB8fCAhdmFsLmxlbmd0aDtcbiAgICAgICAgfSk7XG5cbiAgICAgICAgJHNjb3BlLmNsb3NlTW9kYWwgPSBmdW5jdGlvbiAoKSB7XG4gICAgICAgICAgICB0b3Aud2luZG93LnVjLm1vZHVsZXMuY2xvdWQuZ2tFdmVudExpc3RlbmVyLk9uR2tFdmVudExpc3RlbmVyKFwiY2FuY2VsXCIsIG51bGwpO1xuICAgICAgICB9O1xuXG4gICAgfV0pXG4gICAgLmZhY3RvcnkoJ2drTW9kYWwnLCByZXF1aXJlKCcuL3NlcnZpY2UvZ2tNb2RhbCcpKVxuO1xuXG5cbi8qKioqKioqKioqKioqKioqKlxuICoqIFdFQlBBQ0sgRk9PVEVSXG4gKiogLi9jcmVhdGVMaWIuanNcbiAqKiBtb2R1bGUgaWQgPSAwXG4gKiogbW9kdWxlIGNodW5rcyA9IDBcbiAqKi8iLCJcbm1vZHVsZS5leHBvcnRzID0gXCJjcmVhdGVMaWIuaHRtbFwiXG5cblxuLyoqKioqKioqKioqKioqKioqXG4gKiogV0VCUEFDSyBGT09URVJcbiAqKiAuLi9jcmVhdGVMaWIuaHRtbFxuICoqIG1vZHVsZSBpZCA9IDk3XG4gKiogbW9kdWxlIGNodW5rcyA9IDBcbiAqKi8iLCIvLyByZW1vdmVkIGJ5IGV4dHJhY3QtdGV4dC13ZWJwYWNrLXBsdWdpblxuXG5cbi8qKioqKioqKioqKioqKioqKlxuICoqIFdFQlBBQ0sgRk9PVEVSXG4gKiogLi4vc3R5bGVzL2NyZWF0ZUxpYi5zY3NzXG4gKiogbW9kdWxlIGlkID0gMTEyXG4gKiogbW9kdWxlIGNodW5rcyA9IDBcbiAqKi8iLCJtb2R1bGUuZXhwb3J0cyA9IGdsb2JhbFtcIl9cIl0gPSByZXF1aXJlKFwiLSEvVXNlcnMveHVnZS9wcm9qZWN0L3F1YW5zaGktdWkvbm9kZV9tb2R1bGVzL3VuZGVyc2NvcmUvdW5kZXJzY29yZS5qc1wiKTtcblxuXG4vKioqKioqKioqKioqKioqKipcbiAqKiBXRUJQQUNLIEZPT1RFUlxuICoqIC9Vc2Vycy94dWdlL3Byb2plY3QvcXVhbnNoaS11aS9+L2V4cG9zZS1sb2FkZXI/XyEvVXNlcnMveHVnZS9wcm9qZWN0L3F1YW5zaGktdWkvfi91bmRlcnNjb3JlL3VuZGVyc2NvcmUuanNcbiAqKiBtb2R1bGUgaWQgPSAxODNcbiAqKiBtb2R1bGUgY2h1bmtzID0gMCAyIDMgNCA1IDYgN1xuICoqLyIsIi8vICAgICBVbmRlcnNjb3JlLmpzIDEuOC4zXG4vLyAgICAgaHR0cDovL3VuZGVyc2NvcmVqcy5vcmdcbi8vICAgICAoYykgMjAwOS0yMDE1IEplcmVteSBBc2hrZW5hcywgRG9jdW1lbnRDbG91ZCBhbmQgSW52ZXN0aWdhdGl2ZSBSZXBvcnRlcnMgJiBFZGl0b3JzXG4vLyAgICAgVW5kZXJzY29yZSBtYXkgYmUgZnJlZWx5IGRpc3RyaWJ1dGVkIHVuZGVyIHRoZSBNSVQgbGljZW5zZS5cblxuKGZ1bmN0aW9uKCkge1xuXG4gIC8vIEJhc2VsaW5lIHNldHVwXG4gIC8vIC0tLS0tLS0tLS0tLS0tXG5cbiAgLy8gRXN0YWJsaXNoIHRoZSByb290IG9iamVjdCwgYHdpbmRvd2AgaW4gdGhlIGJyb3dzZXIsIG9yIGBleHBvcnRzYCBvbiB0aGUgc2VydmVyLlxuICB2YXIgcm9vdCA9IHRoaXM7XG5cbiAgLy8gU2F2ZSB0aGUgcHJldmlvdXMgdmFsdWUgb2YgdGhlIGBfYCB2YXJpYWJsZS5cbiAgdmFyIHByZXZpb3VzVW5kZXJzY29yZSA9IHJvb3QuXztcblxuICAvLyBTYXZlIGJ5dGVzIGluIHRoZSBtaW5pZmllZCAoYnV0IG5vdCBnemlwcGVkKSB2ZXJzaW9uOlxuICB2YXIgQXJyYXlQcm90byA9IEFycmF5LnByb3RvdHlwZSwgT2JqUHJvdG8gPSBPYmplY3QucHJvdG90eXBlLCBGdW5jUHJvdG8gPSBGdW5jdGlvbi5wcm90b3R5cGU7XG5cbiAgLy8gQ3JlYXRlIHF1aWNrIHJlZmVyZW5jZSB2YXJpYWJsZXMgZm9yIHNwZWVkIGFjY2VzcyB0byBjb3JlIHByb3RvdHlwZXMuXG4gIHZhclxuICAgIHB1c2ggICAgICAgICAgICAgPSBBcnJheVByb3RvLnB1c2gsXG4gICAgc2xpY2UgICAgICAgICAgICA9IEFycmF5UHJvdG8uc2xpY2UsXG4gICAgdG9TdHJpbmcgICAgICAgICA9IE9ialByb3RvLnRvU3RyaW5nLFxuICAgIGhhc093blByb3BlcnR5ICAgPSBPYmpQcm90by5oYXNPd25Qcm9wZXJ0eTtcblxuICAvLyBBbGwgKipFQ01BU2NyaXB0IDUqKiBuYXRpdmUgZnVuY3Rpb24gaW1wbGVtZW50YXRpb25zIHRoYXQgd2UgaG9wZSB0byB1c2VcbiAgLy8gYXJlIGRlY2xhcmVkIGhlcmUuXG4gIHZhclxuICAgIG5hdGl2ZUlzQXJyYXkgICAgICA9IEFycmF5LmlzQXJyYXksXG4gICAgbmF0aXZlS2V5cyAgICAgICAgID0gT2JqZWN0LmtleXMsXG4gICAgbmF0aXZlQmluZCAgICAgICAgID0gRnVuY1Byb3RvLmJpbmQsXG4gICAgbmF0aXZlQ3JlYXRlICAgICAgID0gT2JqZWN0LmNyZWF0ZTtcblxuICAvLyBOYWtlZCBmdW5jdGlvbiByZWZlcmVuY2UgZm9yIHN1cnJvZ2F0ZS1wcm90b3R5cGUtc3dhcHBpbmcuXG4gIHZhciBDdG9yID0gZnVuY3Rpb24oKXt9O1xuXG4gIC8vIENyZWF0ZSBhIHNhZmUgcmVmZXJlbmNlIHRvIHRoZSBVbmRlcnNjb3JlIG9iamVjdCBmb3IgdXNlIGJlbG93LlxuICB2YXIgXyA9IGZ1bmN0aW9uKG9iaikge1xuICAgIGlmIChvYmogaW5zdGFuY2VvZiBfKSByZXR1cm4gb2JqO1xuICAgIGlmICghKHRoaXMgaW5zdGFuY2VvZiBfKSkgcmV0dXJuIG5ldyBfKG9iaik7XG4gICAgdGhpcy5fd3JhcHBlZCA9IG9iajtcbiAgfTtcblxuICAvLyBFeHBvcnQgdGhlIFVuZGVyc2NvcmUgb2JqZWN0IGZvciAqKk5vZGUuanMqKiwgd2l0aFxuICAvLyBiYWNrd2FyZHMtY29tcGF0aWJpbGl0eSBmb3IgdGhlIG9sZCBgcmVxdWlyZSgpYCBBUEkuIElmIHdlJ3JlIGluXG4gIC8vIHRoZSBicm93c2VyLCBhZGQgYF9gIGFzIGEgZ2xvYmFsIG9iamVjdC5cbiAgaWYgKHR5cGVvZiBleHBvcnRzICE9PSAndW5kZWZpbmVkJykge1xuICAgIGlmICh0eXBlb2YgbW9kdWxlICE9PSAndW5kZWZpbmVkJyAmJiBtb2R1bGUuZXhwb3J0cykge1xuICAgICAgZXhwb3J0cyA9IG1vZHVsZS5leHBvcnRzID0gXztcbiAgICB9XG4gICAgZXhwb3J0cy5fID0gXztcbiAgfSBlbHNlIHtcbiAgICByb290Ll8gPSBfO1xuICB9XG5cbiAgLy8gQ3VycmVudCB2ZXJzaW9uLlxuICBfLlZFUlNJT04gPSAnMS44LjMnO1xuXG4gIC8vIEludGVybmFsIGZ1bmN0aW9uIHRoYXQgcmV0dXJucyBhbiBlZmZpY2llbnQgKGZvciBjdXJyZW50IGVuZ2luZXMpIHZlcnNpb25cbiAgLy8gb2YgdGhlIHBhc3NlZC1pbiBjYWxsYmFjaywgdG8gYmUgcmVwZWF0ZWRseSBhcHBsaWVkIGluIG90aGVyIFVuZGVyc2NvcmVcbiAgLy8gZnVuY3Rpb25zLlxuICB2YXIgb3B0aW1pemVDYiA9IGZ1bmN0aW9uKGZ1bmMsIGNvbnRleHQsIGFyZ0NvdW50KSB7XG4gICAgaWYgKGNvbnRleHQgPT09IHZvaWQgMCkgcmV0dXJuIGZ1bmM7XG4gICAgc3dpdGNoIChhcmdDb3VudCA9PSBudWxsID8gMyA6IGFyZ0NvdW50KSB7XG4gICAgICBjYXNlIDE6IHJldHVybiBmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgICByZXR1cm4gZnVuYy5jYWxsKGNvbnRleHQsIHZhbHVlKTtcbiAgICAgIH07XG4gICAgICBjYXNlIDI6IHJldHVybiBmdW5jdGlvbih2YWx1ZSwgb3RoZXIpIHtcbiAgICAgICAgcmV0dXJuIGZ1bmMuY2FsbChjb250ZXh0LCB2YWx1ZSwgb3RoZXIpO1xuICAgICAgfTtcbiAgICAgIGNhc2UgMzogcmV0dXJuIGZ1bmN0aW9uKHZhbHVlLCBpbmRleCwgY29sbGVjdGlvbikge1xuICAgICAgICByZXR1cm4gZnVuYy5jYWxsKGNvbnRleHQsIHZhbHVlLCBpbmRleCwgY29sbGVjdGlvbik7XG4gICAgICB9O1xuICAgICAgY2FzZSA0OiByZXR1cm4gZnVuY3Rpb24oYWNjdW11bGF0b3IsIHZhbHVlLCBpbmRleCwgY29sbGVjdGlvbikge1xuICAgICAgICByZXR1cm4gZnVuYy5jYWxsKGNvbnRleHQsIGFjY3VtdWxhdG9yLCB2YWx1ZSwgaW5kZXgsIGNvbGxlY3Rpb24pO1xuICAgICAgfTtcbiAgICB9XG4gICAgcmV0dXJuIGZ1bmN0aW9uKCkge1xuICAgICAgcmV0dXJuIGZ1bmMuYXBwbHkoY29udGV4dCwgYXJndW1lbnRzKTtcbiAgICB9O1xuICB9O1xuXG4gIC8vIEEgbW9zdGx5LWludGVybmFsIGZ1bmN0aW9uIHRvIGdlbmVyYXRlIGNhbGxiYWNrcyB0aGF0IGNhbiBiZSBhcHBsaWVkXG4gIC8vIHRvIGVhY2ggZWxlbWVudCBpbiBhIGNvbGxlY3Rpb24sIHJldHVybmluZyB0aGUgZGVzaXJlZCByZXN1bHQg4oCUIGVpdGhlclxuICAvLyBpZGVudGl0eSwgYW4gYXJiaXRyYXJ5IGNhbGxiYWNrLCBhIHByb3BlcnR5IG1hdGNoZXIsIG9yIGEgcHJvcGVydHkgYWNjZXNzb3IuXG4gIHZhciBjYiA9IGZ1bmN0aW9uKHZhbHVlLCBjb250ZXh0LCBhcmdDb3VudCkge1xuICAgIGlmICh2YWx1ZSA9PSBudWxsKSByZXR1cm4gXy5pZGVudGl0eTtcbiAgICBpZiAoXy5pc0Z1bmN0aW9uKHZhbHVlKSkgcmV0dXJuIG9wdGltaXplQ2IodmFsdWUsIGNvbnRleHQsIGFyZ0NvdW50KTtcbiAgICBpZiAoXy5pc09iamVjdCh2YWx1ZSkpIHJldHVybiBfLm1hdGNoZXIodmFsdWUpO1xuICAgIHJldHVybiBfLnByb3BlcnR5KHZhbHVlKTtcbiAgfTtcbiAgXy5pdGVyYXRlZSA9IGZ1bmN0aW9uKHZhbHVlLCBjb250ZXh0KSB7XG4gICAgcmV0dXJuIGNiKHZhbHVlLCBjb250ZXh0LCBJbmZpbml0eSk7XG4gIH07XG5cbiAgLy8gQW4gaW50ZXJuYWwgZnVuY3Rpb24gZm9yIGNyZWF0aW5nIGFzc2lnbmVyIGZ1bmN0aW9ucy5cbiAgdmFyIGNyZWF0ZUFzc2lnbmVyID0gZnVuY3Rpb24oa2V5c0Z1bmMsIHVuZGVmaW5lZE9ubHkpIHtcbiAgICByZXR1cm4gZnVuY3Rpb24ob2JqKSB7XG4gICAgICB2YXIgbGVuZ3RoID0gYXJndW1lbnRzLmxlbmd0aDtcbiAgICAgIGlmIChsZW5ndGggPCAyIHx8IG9iaiA9PSBudWxsKSByZXR1cm4gb2JqO1xuICAgICAgZm9yICh2YXIgaW5kZXggPSAxOyBpbmRleCA8IGxlbmd0aDsgaW5kZXgrKykge1xuICAgICAgICB2YXIgc291cmNlID0gYXJndW1lbnRzW2luZGV4XSxcbiAgICAgICAgICAgIGtleXMgPSBrZXlzRnVuYyhzb3VyY2UpLFxuICAgICAgICAgICAgbCA9IGtleXMubGVuZ3RoO1xuICAgICAgICBmb3IgKHZhciBpID0gMDsgaSA8IGw7IGkrKykge1xuICAgICAgICAgIHZhciBrZXkgPSBrZXlzW2ldO1xuICAgICAgICAgIGlmICghdW5kZWZpbmVkT25seSB8fCBvYmpba2V5XSA9PT0gdm9pZCAwKSBvYmpba2V5XSA9IHNvdXJjZVtrZXldO1xuICAgICAgICB9XG4gICAgICB9XG4gICAgICByZXR1cm4gb2JqO1xuICAgIH07XG4gIH07XG5cbiAgLy8gQW4gaW50ZXJuYWwgZnVuY3Rpb24gZm9yIGNyZWF0aW5nIGEgbmV3IG9iamVjdCB0aGF0IGluaGVyaXRzIGZyb20gYW5vdGhlci5cbiAgdmFyIGJhc2VDcmVhdGUgPSBmdW5jdGlvbihwcm90b3R5cGUpIHtcbiAgICBpZiAoIV8uaXNPYmplY3QocHJvdG90eXBlKSkgcmV0dXJuIHt9O1xuICAgIGlmIChuYXRpdmVDcmVhdGUpIHJldHVybiBuYXRpdmVDcmVhdGUocHJvdG90eXBlKTtcbiAgICBDdG9yLnByb3RvdHlwZSA9IHByb3RvdHlwZTtcbiAgICB2YXIgcmVzdWx0ID0gbmV3IEN0b3I7XG4gICAgQ3Rvci5wcm90b3R5cGUgPSBudWxsO1xuICAgIHJldHVybiByZXN1bHQ7XG4gIH07XG5cbiAgdmFyIHByb3BlcnR5ID0gZnVuY3Rpb24oa2V5KSB7XG4gICAgcmV0dXJuIGZ1bmN0aW9uKG9iaikge1xuICAgICAgcmV0dXJuIG9iaiA9PSBudWxsID8gdm9pZCAwIDogb2JqW2tleV07XG4gICAgfTtcbiAgfTtcblxuICAvLyBIZWxwZXIgZm9yIGNvbGxlY3Rpb24gbWV0aG9kcyB0byBkZXRlcm1pbmUgd2hldGhlciBhIGNvbGxlY3Rpb25cbiAgLy8gc2hvdWxkIGJlIGl0ZXJhdGVkIGFzIGFuIGFycmF5IG9yIGFzIGFuIG9iamVjdFxuICAvLyBSZWxhdGVkOiBodHRwOi8vcGVvcGxlLm1vemlsbGEub3JnL35qb3JlbmRvcmZmL2VzNi1kcmFmdC5odG1sI3NlYy10b2xlbmd0aFxuICAvLyBBdm9pZHMgYSB2ZXJ5IG5hc3R5IGlPUyA4IEpJVCBidWcgb24gQVJNLTY0LiAjMjA5NFxuICB2YXIgTUFYX0FSUkFZX0lOREVYID0gTWF0aC5wb3coMiwgNTMpIC0gMTtcbiAgdmFyIGdldExlbmd0aCA9IHByb3BlcnR5KCdsZW5ndGgnKTtcbiAgdmFyIGlzQXJyYXlMaWtlID0gZnVuY3Rpb24oY29sbGVjdGlvbikge1xuICAgIHZhciBsZW5ndGggPSBnZXRMZW5ndGgoY29sbGVjdGlvbik7XG4gICAgcmV0dXJuIHR5cGVvZiBsZW5ndGggPT0gJ251bWJlcicgJiYgbGVuZ3RoID49IDAgJiYgbGVuZ3RoIDw9IE1BWF9BUlJBWV9JTkRFWDtcbiAgfTtcblxuICAvLyBDb2xsZWN0aW9uIEZ1bmN0aW9uc1xuICAvLyAtLS0tLS0tLS0tLS0tLS0tLS0tLVxuXG4gIC8vIFRoZSBjb3JuZXJzdG9uZSwgYW4gYGVhY2hgIGltcGxlbWVudGF0aW9uLCBha2EgYGZvckVhY2hgLlxuICAvLyBIYW5kbGVzIHJhdyBvYmplY3RzIGluIGFkZGl0aW9uIHRvIGFycmF5LWxpa2VzLiBUcmVhdHMgYWxsXG4gIC8vIHNwYXJzZSBhcnJheS1saWtlcyBhcyBpZiB0aGV5IHdlcmUgZGVuc2UuXG4gIF8uZWFjaCA9IF8uZm9yRWFjaCA9IGZ1bmN0aW9uKG9iaiwgaXRlcmF0ZWUsIGNvbnRleHQpIHtcbiAgICBpdGVyYXRlZSA9IG9wdGltaXplQ2IoaXRlcmF0ZWUsIGNvbnRleHQpO1xuICAgIHZhciBpLCBsZW5ndGg7XG4gICAgaWYgKGlzQXJyYXlMaWtlKG9iaikpIHtcbiAgICAgIGZvciAoaSA9IDAsIGxlbmd0aCA9IG9iai5sZW5ndGg7IGkgPCBsZW5ndGg7IGkrKykge1xuICAgICAgICBpdGVyYXRlZShvYmpbaV0sIGksIG9iaik7XG4gICAgICB9XG4gICAgfSBlbHNlIHtcbiAgICAgIHZhciBrZXlzID0gXy5rZXlzKG9iaik7XG4gICAgICBmb3IgKGkgPSAwLCBsZW5ndGggPSBrZXlzLmxlbmd0aDsgaSA8IGxlbmd0aDsgaSsrKSB7XG4gICAgICAgIGl0ZXJhdGVlKG9ialtrZXlzW2ldXSwga2V5c1tpXSwgb2JqKTtcbiAgICAgIH1cbiAgICB9XG4gICAgcmV0dXJuIG9iajtcbiAgfTtcblxuICAvLyBSZXR1cm4gdGhlIHJlc3VsdHMgb2YgYXBwbHlpbmcgdGhlIGl0ZXJhdGVlIHRvIGVhY2ggZWxlbWVudC5cbiAgXy5tYXAgPSBfLmNvbGxlY3QgPSBmdW5jdGlvbihvYmosIGl0ZXJhdGVlLCBjb250ZXh0KSB7XG4gICAgaXRlcmF0ZWUgPSBjYihpdGVyYXRlZSwgY29udGV4dCk7XG4gICAgdmFyIGtleXMgPSAhaXNBcnJheUxpa2Uob2JqKSAmJiBfLmtleXMob2JqKSxcbiAgICAgICAgbGVuZ3RoID0gKGtleXMgfHwgb2JqKS5sZW5ndGgsXG4gICAgICAgIHJlc3VsdHMgPSBBcnJheShsZW5ndGgpO1xuICAgIGZvciAodmFyIGluZGV4ID0gMDsgaW5kZXggPCBsZW5ndGg7IGluZGV4KyspIHtcbiAgICAgIHZhciBjdXJyZW50S2V5ID0ga2V5cyA/IGtleXNbaW5kZXhdIDogaW5kZXg7XG4gICAgICByZXN1bHRzW2luZGV4XSA9IGl0ZXJhdGVlKG9ialtjdXJyZW50S2V5XSwgY3VycmVudEtleSwgb2JqKTtcbiAgICB9XG4gICAgcmV0dXJuIHJlc3VsdHM7XG4gIH07XG5cbiAgLy8gQ3JlYXRlIGEgcmVkdWNpbmcgZnVuY3Rpb24gaXRlcmF0aW5nIGxlZnQgb3IgcmlnaHQuXG4gIGZ1bmN0aW9uIGNyZWF0ZVJlZHVjZShkaXIpIHtcbiAgICAvLyBPcHRpbWl6ZWQgaXRlcmF0b3IgZnVuY3Rpb24gYXMgdXNpbmcgYXJndW1lbnRzLmxlbmd0aFxuICAgIC8vIGluIHRoZSBtYWluIGZ1bmN0aW9uIHdpbGwgZGVvcHRpbWl6ZSB0aGUsIHNlZSAjMTk5MS5cbiAgICBmdW5jdGlvbiBpdGVyYXRvcihvYmosIGl0ZXJhdGVlLCBtZW1vLCBrZXlzLCBpbmRleCwgbGVuZ3RoKSB7XG4gICAgICBmb3IgKDsgaW5kZXggPj0gMCAmJiBpbmRleCA8IGxlbmd0aDsgaW5kZXggKz0gZGlyKSB7XG4gICAgICAgIHZhciBjdXJyZW50S2V5ID0ga2V5cyA/IGtleXNbaW5kZXhdIDogaW5kZXg7XG4gICAgICAgIG1lbW8gPSBpdGVyYXRlZShtZW1vLCBvYmpbY3VycmVudEtleV0sIGN1cnJlbnRLZXksIG9iaik7XG4gICAgICB9XG4gICAgICByZXR1cm4gbWVtbztcbiAgICB9XG5cbiAgICByZXR1cm4gZnVuY3Rpb24ob2JqLCBpdGVyYXRlZSwgbWVtbywgY29udGV4dCkge1xuICAgICAgaXRlcmF0ZWUgPSBvcHRpbWl6ZUNiKGl0ZXJhdGVlLCBjb250ZXh0LCA0KTtcbiAgICAgIHZhciBrZXlzID0gIWlzQXJyYXlMaWtlKG9iaikgJiYgXy5rZXlzKG9iaiksXG4gICAgICAgICAgbGVuZ3RoID0gKGtleXMgfHwgb2JqKS5sZW5ndGgsXG4gICAgICAgICAgaW5kZXggPSBkaXIgPiAwID8gMCA6IGxlbmd0aCAtIDE7XG4gICAgICAvLyBEZXRlcm1pbmUgdGhlIGluaXRpYWwgdmFsdWUgaWYgbm9uZSBpcyBwcm92aWRlZC5cbiAgICAgIGlmIChhcmd1bWVudHMubGVuZ3RoIDwgMykge1xuICAgICAgICBtZW1vID0gb2JqW2tleXMgPyBrZXlzW2luZGV4XSA6IGluZGV4XTtcbiAgICAgICAgaW5kZXggKz0gZGlyO1xuICAgICAgfVxuICAgICAgcmV0dXJuIGl0ZXJhdG9yKG9iaiwgaXRlcmF0ZWUsIG1lbW8sIGtleXMsIGluZGV4LCBsZW5ndGgpO1xuICAgIH07XG4gIH1cblxuICAvLyAqKlJlZHVjZSoqIGJ1aWxkcyB1cCBhIHNpbmdsZSByZXN1bHQgZnJvbSBhIGxpc3Qgb2YgdmFsdWVzLCBha2EgYGluamVjdGAsXG4gIC8vIG9yIGBmb2xkbGAuXG4gIF8ucmVkdWNlID0gXy5mb2xkbCA9IF8uaW5qZWN0ID0gY3JlYXRlUmVkdWNlKDEpO1xuXG4gIC8vIFRoZSByaWdodC1hc3NvY2lhdGl2ZSB2ZXJzaW9uIG9mIHJlZHVjZSwgYWxzbyBrbm93biBhcyBgZm9sZHJgLlxuICBfLnJlZHVjZVJpZ2h0ID0gXy5mb2xkciA9IGNyZWF0ZVJlZHVjZSgtMSk7XG5cbiAgLy8gUmV0dXJuIHRoZSBmaXJzdCB2YWx1ZSB3aGljaCBwYXNzZXMgYSB0cnV0aCB0ZXN0LiBBbGlhc2VkIGFzIGBkZXRlY3RgLlxuICBfLmZpbmQgPSBfLmRldGVjdCA9IGZ1bmN0aW9uKG9iaiwgcHJlZGljYXRlLCBjb250ZXh0KSB7XG4gICAgdmFyIGtleTtcbiAgICBpZiAoaXNBcnJheUxpa2Uob2JqKSkge1xuICAgICAga2V5ID0gXy5maW5kSW5kZXgob2JqLCBwcmVkaWNhdGUsIGNvbnRleHQpO1xuICAgIH0gZWxzZSB7XG4gICAgICBrZXkgPSBfLmZpbmRLZXkob2JqLCBwcmVkaWNhdGUsIGNvbnRleHQpO1xuICAgIH1cbiAgICBpZiAoa2V5ICE9PSB2b2lkIDAgJiYga2V5ICE9PSAtMSkgcmV0dXJuIG9ialtrZXldO1xuICB9O1xuXG4gIC8vIFJldHVybiBhbGwgdGhlIGVsZW1lbnRzIHRoYXQgcGFzcyBhIHRydXRoIHRlc3QuXG4gIC8vIEFsaWFzZWQgYXMgYHNlbGVjdGAuXG4gIF8uZmlsdGVyID0gXy5zZWxlY3QgPSBmdW5jdGlvbihvYmosIHByZWRpY2F0ZSwgY29udGV4dCkge1xuICAgIHZhciByZXN1bHRzID0gW107XG4gICAgcHJlZGljYXRlID0gY2IocHJlZGljYXRlLCBjb250ZXh0KTtcbiAgICBfLmVhY2gob2JqLCBmdW5jdGlvbih2YWx1ZSwgaW5kZXgsIGxpc3QpIHtcbiAgICAgIGlmIChwcmVkaWNhdGUodmFsdWUsIGluZGV4LCBsaXN0KSkgcmVzdWx0cy5wdXNoKHZhbHVlKTtcbiAgICB9KTtcbiAgICByZXR1cm4gcmVzdWx0cztcbiAgfTtcblxuICAvLyBSZXR1cm4gYWxsIHRoZSBlbGVtZW50cyBmb3Igd2hpY2ggYSB0cnV0aCB0ZXN0IGZhaWxzLlxuICBfLnJlamVjdCA9IGZ1bmN0aW9uKG9iaiwgcHJlZGljYXRlLCBjb250ZXh0KSB7XG4gICAgcmV0dXJuIF8uZmlsdGVyKG9iaiwgXy5uZWdhdGUoY2IocHJlZGljYXRlKSksIGNvbnRleHQpO1xuICB9O1xuXG4gIC8vIERldGVybWluZSB3aGV0aGVyIGFsbCBvZiB0aGUgZWxlbWVudHMgbWF0Y2ggYSB0cnV0aCB0ZXN0LlxuICAvLyBBbGlhc2VkIGFzIGBhbGxgLlxuICBfLmV2ZXJ5ID0gXy5hbGwgPSBmdW5jdGlvbihvYmosIHByZWRpY2F0ZSwgY29udGV4dCkge1xuICAgIHByZWRpY2F0ZSA9IGNiKHByZWRpY2F0ZSwgY29udGV4dCk7XG4gICAgdmFyIGtleXMgPSAhaXNBcnJheUxpa2Uob2JqKSAmJiBfLmtleXMob2JqKSxcbiAgICAgICAgbGVuZ3RoID0gKGtleXMgfHwgb2JqKS5sZW5ndGg7XG4gICAgZm9yICh2YXIgaW5kZXggPSAwOyBpbmRleCA8IGxlbmd0aDsgaW5kZXgrKykge1xuICAgICAgdmFyIGN1cnJlbnRLZXkgPSBrZXlzID8ga2V5c1tpbmRleF0gOiBpbmRleDtcbiAgICAgIGlmICghcHJlZGljYXRlKG9ialtjdXJyZW50S2V5XSwgY3VycmVudEtleSwgb2JqKSkgcmV0dXJuIGZhbHNlO1xuICAgIH1cbiAgICByZXR1cm4gdHJ1ZTtcbiAgfTtcblxuICAvLyBEZXRlcm1pbmUgaWYgYXQgbGVhc3Qgb25lIGVsZW1lbnQgaW4gdGhlIG9iamVjdCBtYXRjaGVzIGEgdHJ1dGggdGVzdC5cbiAgLy8gQWxpYXNlZCBhcyBgYW55YC5cbiAgXy5zb21lID0gXy5hbnkgPSBmdW5jdGlvbihvYmosIHByZWRpY2F0ZSwgY29udGV4dCkge1xuICAgIHByZWRpY2F0ZSA9IGNiKHByZWRpY2F0ZSwgY29udGV4dCk7XG4gICAgdmFyIGtleXMgPSAhaXNBcnJheUxpa2Uob2JqKSAmJiBfLmtleXMob2JqKSxcbiAgICAgICAgbGVuZ3RoID0gKGtleXMgfHwgb2JqKS5sZW5ndGg7XG4gICAgZm9yICh2YXIgaW5kZXggPSAwOyBpbmRleCA8IGxlbmd0aDsgaW5kZXgrKykge1xuICAgICAgdmFyIGN1cnJlbnRLZXkgPSBrZXlzID8ga2V5c1tpbmRleF0gOiBpbmRleDtcbiAgICAgIGlmIChwcmVkaWNhdGUob2JqW2N1cnJlbnRLZXldLCBjdXJyZW50S2V5LCBvYmopKSByZXR1cm4gdHJ1ZTtcbiAgICB9XG4gICAgcmV0dXJuIGZhbHNlO1xuICB9O1xuXG4gIC8vIERldGVybWluZSBpZiB0aGUgYXJyYXkgb3Igb2JqZWN0IGNvbnRhaW5zIGEgZ2l2ZW4gaXRlbSAodXNpbmcgYD09PWApLlxuICAvLyBBbGlhc2VkIGFzIGBpbmNsdWRlc2AgYW5kIGBpbmNsdWRlYC5cbiAgXy5jb250YWlucyA9IF8uaW5jbHVkZXMgPSBfLmluY2x1ZGUgPSBmdW5jdGlvbihvYmosIGl0ZW0sIGZyb21JbmRleCwgZ3VhcmQpIHtcbiAgICBpZiAoIWlzQXJyYXlMaWtlKG9iaikpIG9iaiA9IF8udmFsdWVzKG9iaik7XG4gICAgaWYgKHR5cGVvZiBmcm9tSW5kZXggIT0gJ251bWJlcicgfHwgZ3VhcmQpIGZyb21JbmRleCA9IDA7XG4gICAgcmV0dXJuIF8uaW5kZXhPZihvYmosIGl0ZW0sIGZyb21JbmRleCkgPj0gMDtcbiAgfTtcblxuICAvLyBJbnZva2UgYSBtZXRob2QgKHdpdGggYXJndW1lbnRzKSBvbiBldmVyeSBpdGVtIGluIGEgY29sbGVjdGlvbi5cbiAgXy5pbnZva2UgPSBmdW5jdGlvbihvYmosIG1ldGhvZCkge1xuICAgIHZhciBhcmdzID0gc2xpY2UuY2FsbChhcmd1bWVudHMsIDIpO1xuICAgIHZhciBpc0Z1bmMgPSBfLmlzRnVuY3Rpb24obWV0aG9kKTtcbiAgICByZXR1cm4gXy5tYXAob2JqLCBmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgdmFyIGZ1bmMgPSBpc0Z1bmMgPyBtZXRob2QgOiB2YWx1ZVttZXRob2RdO1xuICAgICAgcmV0dXJuIGZ1bmMgPT0gbnVsbCA/IGZ1bmMgOiBmdW5jLmFwcGx5KHZhbHVlLCBhcmdzKTtcbiAgICB9KTtcbiAgfTtcblxuICAvLyBDb252ZW5pZW5jZSB2ZXJzaW9uIG9mIGEgY29tbW9uIHVzZSBjYXNlIG9mIGBtYXBgOiBmZXRjaGluZyBhIHByb3BlcnR5LlxuICBfLnBsdWNrID0gZnVuY3Rpb24ob2JqLCBrZXkpIHtcbiAgICByZXR1cm4gXy5tYXAob2JqLCBfLnByb3BlcnR5KGtleSkpO1xuICB9O1xuXG4gIC8vIENvbnZlbmllbmNlIHZlcnNpb24gb2YgYSBjb21tb24gdXNlIGNhc2Ugb2YgYGZpbHRlcmA6IHNlbGVjdGluZyBvbmx5IG9iamVjdHNcbiAgLy8gY29udGFpbmluZyBzcGVjaWZpYyBga2V5OnZhbHVlYCBwYWlycy5cbiAgXy53aGVyZSA9IGZ1bmN0aW9uKG9iaiwgYXR0cnMpIHtcbiAgICByZXR1cm4gXy5maWx0ZXIob2JqLCBfLm1hdGNoZXIoYXR0cnMpKTtcbiAgfTtcblxuICAvLyBDb252ZW5pZW5jZSB2ZXJzaW9uIG9mIGEgY29tbW9uIHVzZSBjYXNlIG9mIGBmaW5kYDogZ2V0dGluZyB0aGUgZmlyc3Qgb2JqZWN0XG4gIC8vIGNvbnRhaW5pbmcgc3BlY2lmaWMgYGtleTp2YWx1ZWAgcGFpcnMuXG4gIF8uZmluZFdoZXJlID0gZnVuY3Rpb24ob2JqLCBhdHRycykge1xuICAgIHJldHVybiBfLmZpbmQob2JqLCBfLm1hdGNoZXIoYXR0cnMpKTtcbiAgfTtcblxuICAvLyBSZXR1cm4gdGhlIG1heGltdW0gZWxlbWVudCAob3IgZWxlbWVudC1iYXNlZCBjb21wdXRhdGlvbikuXG4gIF8ubWF4ID0gZnVuY3Rpb24ob2JqLCBpdGVyYXRlZSwgY29udGV4dCkge1xuICAgIHZhciByZXN1bHQgPSAtSW5maW5pdHksIGxhc3RDb21wdXRlZCA9IC1JbmZpbml0eSxcbiAgICAgICAgdmFsdWUsIGNvbXB1dGVkO1xuICAgIGlmIChpdGVyYXRlZSA9PSBudWxsICYmIG9iaiAhPSBudWxsKSB7XG4gICAgICBvYmogPSBpc0FycmF5TGlrZShvYmopID8gb2JqIDogXy52YWx1ZXMob2JqKTtcbiAgICAgIGZvciAodmFyIGkgPSAwLCBsZW5ndGggPSBvYmoubGVuZ3RoOyBpIDwgbGVuZ3RoOyBpKyspIHtcbiAgICAgICAgdmFsdWUgPSBvYmpbaV07XG4gICAgICAgIGlmICh2YWx1ZSA+IHJlc3VsdCkge1xuICAgICAgICAgIHJlc3VsdCA9IHZhbHVlO1xuICAgICAgICB9XG4gICAgICB9XG4gICAgfSBlbHNlIHtcbiAgICAgIGl0ZXJhdGVlID0gY2IoaXRlcmF0ZWUsIGNvbnRleHQpO1xuICAgICAgXy5lYWNoKG9iaiwgZnVuY3Rpb24odmFsdWUsIGluZGV4LCBsaXN0KSB7XG4gICAgICAgIGNvbXB1dGVkID0gaXRlcmF0ZWUodmFsdWUsIGluZGV4LCBsaXN0KTtcbiAgICAgICAgaWYgKGNvbXB1dGVkID4gbGFzdENvbXB1dGVkIHx8IGNvbXB1dGVkID09PSAtSW5maW5pdHkgJiYgcmVzdWx0ID09PSAtSW5maW5pdHkpIHtcbiAgICAgICAgICByZXN1bHQgPSB2YWx1ZTtcbiAgICAgICAgICBsYXN0Q29tcHV0ZWQgPSBjb21wdXRlZDtcbiAgICAgICAgfVxuICAgICAgfSk7XG4gICAgfVxuICAgIHJldHVybiByZXN1bHQ7XG4gIH07XG5cbiAgLy8gUmV0dXJuIHRoZSBtaW5pbXVtIGVsZW1lbnQgKG9yIGVsZW1lbnQtYmFzZWQgY29tcHV0YXRpb24pLlxuICBfLm1pbiA9IGZ1bmN0aW9uKG9iaiwgaXRlcmF0ZWUsIGNvbnRleHQpIHtcbiAgICB2YXIgcmVzdWx0ID0gSW5maW5pdHksIGxhc3RDb21wdXRlZCA9IEluZmluaXR5LFxuICAgICAgICB2YWx1ZSwgY29tcHV0ZWQ7XG4gICAgaWYgKGl0ZXJhdGVlID09IG51bGwgJiYgb2JqICE9IG51bGwpIHtcbiAgICAgIG9iaiA9IGlzQXJyYXlMaWtlKG9iaikgPyBvYmogOiBfLnZhbHVlcyhvYmopO1xuICAgICAgZm9yICh2YXIgaSA9IDAsIGxlbmd0aCA9IG9iai5sZW5ndGg7IGkgPCBsZW5ndGg7IGkrKykge1xuICAgICAgICB2YWx1ZSA9IG9ialtpXTtcbiAgICAgICAgaWYgKHZhbHVlIDwgcmVzdWx0KSB7XG4gICAgICAgICAgcmVzdWx0ID0gdmFsdWU7XG4gICAgICAgIH1cbiAgICAgIH1cbiAgICB9IGVsc2Uge1xuICAgICAgaXRlcmF0ZWUgPSBjYihpdGVyYXRlZSwgY29udGV4dCk7XG4gICAgICBfLmVhY2gob2JqLCBmdW5jdGlvbih2YWx1ZSwgaW5kZXgsIGxpc3QpIHtcbiAgICAgICAgY29tcHV0ZWQgPSBpdGVyYXRlZSh2YWx1ZSwgaW5kZXgsIGxpc3QpO1xuICAgICAgICBpZiAoY29tcHV0ZWQgPCBsYXN0Q29tcHV0ZWQgfHwgY29tcHV0ZWQgPT09IEluZmluaXR5ICYmIHJlc3VsdCA9PT0gSW5maW5pdHkpIHtcbiAgICAgICAgICByZXN1bHQgPSB2YWx1ZTtcbiAgICAgICAgICBsYXN0Q29tcHV0ZWQgPSBjb21wdXRlZDtcbiAgICAgICAgfVxuICAgICAgfSk7XG4gICAgfVxuICAgIHJldHVybiByZXN1bHQ7XG4gIH07XG5cbiAgLy8gU2h1ZmZsZSBhIGNvbGxlY3Rpb24sIHVzaW5nIHRoZSBtb2Rlcm4gdmVyc2lvbiBvZiB0aGVcbiAgLy8gW0Zpc2hlci1ZYXRlcyBzaHVmZmxlXShodHRwOi8vZW4ud2lraXBlZGlhLm9yZy93aWtpL0Zpc2hlcuKAk1lhdGVzX3NodWZmbGUpLlxuICBfLnNodWZmbGUgPSBmdW5jdGlvbihvYmopIHtcbiAgICB2YXIgc2V0ID0gaXNBcnJheUxpa2Uob2JqKSA/IG9iaiA6IF8udmFsdWVzKG9iaik7XG4gICAgdmFyIGxlbmd0aCA9IHNldC5sZW5ndGg7XG4gICAgdmFyIHNodWZmbGVkID0gQXJyYXkobGVuZ3RoKTtcbiAgICBmb3IgKHZhciBpbmRleCA9IDAsIHJhbmQ7IGluZGV4IDwgbGVuZ3RoOyBpbmRleCsrKSB7XG4gICAgICByYW5kID0gXy5yYW5kb20oMCwgaW5kZXgpO1xuICAgICAgaWYgKHJhbmQgIT09IGluZGV4KSBzaHVmZmxlZFtpbmRleF0gPSBzaHVmZmxlZFtyYW5kXTtcbiAgICAgIHNodWZmbGVkW3JhbmRdID0gc2V0W2luZGV4XTtcbiAgICB9XG4gICAgcmV0dXJuIHNodWZmbGVkO1xuICB9O1xuXG4gIC8vIFNhbXBsZSAqKm4qKiByYW5kb20gdmFsdWVzIGZyb20gYSBjb2xsZWN0aW9uLlxuICAvLyBJZiAqKm4qKiBpcyBub3Qgc3BlY2lmaWVkLCByZXR1cm5zIGEgc2luZ2xlIHJhbmRvbSBlbGVtZW50LlxuICAvLyBUaGUgaW50ZXJuYWwgYGd1YXJkYCBhcmd1bWVudCBhbGxvd3MgaXQgdG8gd29yayB3aXRoIGBtYXBgLlxuICBfLnNhbXBsZSA9IGZ1bmN0aW9uKG9iaiwgbiwgZ3VhcmQpIHtcbiAgICBpZiAobiA9PSBudWxsIHx8IGd1YXJkKSB7XG4gICAgICBpZiAoIWlzQXJyYXlMaWtlKG9iaikpIG9iaiA9IF8udmFsdWVzKG9iaik7XG4gICAgICByZXR1cm4gb2JqW18ucmFuZG9tKG9iai5sZW5ndGggLSAxKV07XG4gICAgfVxuICAgIHJldHVybiBfLnNodWZmbGUob2JqKS5zbGljZSgwLCBNYXRoLm1heCgwLCBuKSk7XG4gIH07XG5cbiAgLy8gU29ydCB0aGUgb2JqZWN0J3MgdmFsdWVzIGJ5IGEgY3JpdGVyaW9uIHByb2R1Y2VkIGJ5IGFuIGl0ZXJhdGVlLlxuICBfLnNvcnRCeSA9IGZ1bmN0aW9uKG9iaiwgaXRlcmF0ZWUsIGNvbnRleHQpIHtcbiAgICBpdGVyYXRlZSA9IGNiKGl0ZXJhdGVlLCBjb250ZXh0KTtcbiAgICByZXR1cm4gXy5wbHVjayhfLm1hcChvYmosIGZ1bmN0aW9uKHZhbHVlLCBpbmRleCwgbGlzdCkge1xuICAgICAgcmV0dXJuIHtcbiAgICAgICAgdmFsdWU6IHZhbHVlLFxuICAgICAgICBpbmRleDogaW5kZXgsXG4gICAgICAgIGNyaXRlcmlhOiBpdGVyYXRlZSh2YWx1ZSwgaW5kZXgsIGxpc3QpXG4gICAgICB9O1xuICAgIH0pLnNvcnQoZnVuY3Rpb24obGVmdCwgcmlnaHQpIHtcbiAgICAgIHZhciBhID0gbGVmdC5jcml0ZXJpYTtcbiAgICAgIHZhciBiID0gcmlnaHQuY3JpdGVyaWE7XG4gICAgICBpZiAoYSAhPT0gYikge1xuICAgICAgICBpZiAoYSA+IGIgfHwgYSA9PT0gdm9pZCAwKSByZXR1cm4gMTtcbiAgICAgICAgaWYgKGEgPCBiIHx8IGIgPT09IHZvaWQgMCkgcmV0dXJuIC0xO1xuICAgICAgfVxuICAgICAgcmV0dXJuIGxlZnQuaW5kZXggLSByaWdodC5pbmRleDtcbiAgICB9KSwgJ3ZhbHVlJyk7XG4gIH07XG5cbiAgLy8gQW4gaW50ZXJuYWwgZnVuY3Rpb24gdXNlZCBmb3IgYWdncmVnYXRlIFwiZ3JvdXAgYnlcIiBvcGVyYXRpb25zLlxuICB2YXIgZ3JvdXAgPSBmdW5jdGlvbihiZWhhdmlvcikge1xuICAgIHJldHVybiBmdW5jdGlvbihvYmosIGl0ZXJhdGVlLCBjb250ZXh0KSB7XG4gICAgICB2YXIgcmVzdWx0ID0ge307XG4gICAgICBpdGVyYXRlZSA9IGNiKGl0ZXJhdGVlLCBjb250ZXh0KTtcbiAgICAgIF8uZWFjaChvYmosIGZ1bmN0aW9uKHZhbHVlLCBpbmRleCkge1xuICAgICAgICB2YXIga2V5ID0gaXRlcmF0ZWUodmFsdWUsIGluZGV4LCBvYmopO1xuICAgICAgICBiZWhhdmlvcihyZXN1bHQsIHZhbHVlLCBrZXkpO1xuICAgICAgfSk7XG4gICAgICByZXR1cm4gcmVzdWx0O1xuICAgIH07XG4gIH07XG5cbiAgLy8gR3JvdXBzIHRoZSBvYmplY3QncyB2YWx1ZXMgYnkgYSBjcml0ZXJpb24uIFBhc3MgZWl0aGVyIGEgc3RyaW5nIGF0dHJpYnV0ZVxuICAvLyB0byBncm91cCBieSwgb3IgYSBmdW5jdGlvbiB0aGF0IHJldHVybnMgdGhlIGNyaXRlcmlvbi5cbiAgXy5ncm91cEJ5ID0gZ3JvdXAoZnVuY3Rpb24ocmVzdWx0LCB2YWx1ZSwga2V5KSB7XG4gICAgaWYgKF8uaGFzKHJlc3VsdCwga2V5KSkgcmVzdWx0W2tleV0ucHVzaCh2YWx1ZSk7IGVsc2UgcmVzdWx0W2tleV0gPSBbdmFsdWVdO1xuICB9KTtcblxuICAvLyBJbmRleGVzIHRoZSBvYmplY3QncyB2YWx1ZXMgYnkgYSBjcml0ZXJpb24sIHNpbWlsYXIgdG8gYGdyb3VwQnlgLCBidXQgZm9yXG4gIC8vIHdoZW4geW91IGtub3cgdGhhdCB5b3VyIGluZGV4IHZhbHVlcyB3aWxsIGJlIHVuaXF1ZS5cbiAgXy5pbmRleEJ5ID0gZ3JvdXAoZnVuY3Rpb24ocmVzdWx0LCB2YWx1ZSwga2V5KSB7XG4gICAgcmVzdWx0W2tleV0gPSB2YWx1ZTtcbiAgfSk7XG5cbiAgLy8gQ291bnRzIGluc3RhbmNlcyBvZiBhbiBvYmplY3QgdGhhdCBncm91cCBieSBhIGNlcnRhaW4gY3JpdGVyaW9uLiBQYXNzXG4gIC8vIGVpdGhlciBhIHN0cmluZyBhdHRyaWJ1dGUgdG8gY291bnQgYnksIG9yIGEgZnVuY3Rpb24gdGhhdCByZXR1cm5zIHRoZVxuICAvLyBjcml0ZXJpb24uXG4gIF8uY291bnRCeSA9IGdyb3VwKGZ1bmN0aW9uKHJlc3VsdCwgdmFsdWUsIGtleSkge1xuICAgIGlmIChfLmhhcyhyZXN1bHQsIGtleSkpIHJlc3VsdFtrZXldKys7IGVsc2UgcmVzdWx0W2tleV0gPSAxO1xuICB9KTtcblxuICAvLyBTYWZlbHkgY3JlYXRlIGEgcmVhbCwgbGl2ZSBhcnJheSBmcm9tIGFueXRoaW5nIGl0ZXJhYmxlLlxuICBfLnRvQXJyYXkgPSBmdW5jdGlvbihvYmopIHtcbiAgICBpZiAoIW9iaikgcmV0dXJuIFtdO1xuICAgIGlmIChfLmlzQXJyYXkob2JqKSkgcmV0dXJuIHNsaWNlLmNhbGwob2JqKTtcbiAgICBpZiAoaXNBcnJheUxpa2Uob2JqKSkgcmV0dXJuIF8ubWFwKG9iaiwgXy5pZGVudGl0eSk7XG4gICAgcmV0dXJuIF8udmFsdWVzKG9iaik7XG4gIH07XG5cbiAgLy8gUmV0dXJuIHRoZSBudW1iZXIgb2YgZWxlbWVudHMgaW4gYW4gb2JqZWN0LlxuICBfLnNpemUgPSBmdW5jdGlvbihvYmopIHtcbiAgICBpZiAob2JqID09IG51bGwpIHJldHVybiAwO1xuICAgIHJldHVybiBpc0FycmF5TGlrZShvYmopID8gb2JqLmxlbmd0aCA6IF8ua2V5cyhvYmopLmxlbmd0aDtcbiAgfTtcblxuICAvLyBTcGxpdCBhIGNvbGxlY3Rpb24gaW50byB0d28gYXJyYXlzOiBvbmUgd2hvc2UgZWxlbWVudHMgYWxsIHNhdGlzZnkgdGhlIGdpdmVuXG4gIC8vIHByZWRpY2F0ZSwgYW5kIG9uZSB3aG9zZSBlbGVtZW50cyBhbGwgZG8gbm90IHNhdGlzZnkgdGhlIHByZWRpY2F0ZS5cbiAgXy5wYXJ0aXRpb24gPSBmdW5jdGlvbihvYmosIHByZWRpY2F0ZSwgY29udGV4dCkge1xuICAgIHByZWRpY2F0ZSA9IGNiKHByZWRpY2F0ZSwgY29udGV4dCk7XG4gICAgdmFyIHBhc3MgPSBbXSwgZmFpbCA9IFtdO1xuICAgIF8uZWFjaChvYmosIGZ1bmN0aW9uKHZhbHVlLCBrZXksIG9iaikge1xuICAgICAgKHByZWRpY2F0ZSh2YWx1ZSwga2V5LCBvYmopID8gcGFzcyA6IGZhaWwpLnB1c2godmFsdWUpO1xuICAgIH0pO1xuICAgIHJldHVybiBbcGFzcywgZmFpbF07XG4gIH07XG5cbiAgLy8gQXJyYXkgRnVuY3Rpb25zXG4gIC8vIC0tLS0tLS0tLS0tLS0tLVxuXG4gIC8vIEdldCB0aGUgZmlyc3QgZWxlbWVudCBvZiBhbiBhcnJheS4gUGFzc2luZyAqKm4qKiB3aWxsIHJldHVybiB0aGUgZmlyc3QgTlxuICAvLyB2YWx1ZXMgaW4gdGhlIGFycmF5LiBBbGlhc2VkIGFzIGBoZWFkYCBhbmQgYHRha2VgLiBUaGUgKipndWFyZCoqIGNoZWNrXG4gIC8vIGFsbG93cyBpdCB0byB3b3JrIHdpdGggYF8ubWFwYC5cbiAgXy5maXJzdCA9IF8uaGVhZCA9IF8udGFrZSA9IGZ1bmN0aW9uKGFycmF5LCBuLCBndWFyZCkge1xuICAgIGlmIChhcnJheSA9PSBudWxsKSByZXR1cm4gdm9pZCAwO1xuICAgIGlmIChuID09IG51bGwgfHwgZ3VhcmQpIHJldHVybiBhcnJheVswXTtcbiAgICByZXR1cm4gXy5pbml0aWFsKGFycmF5LCBhcnJheS5sZW5ndGggLSBuKTtcbiAgfTtcblxuICAvLyBSZXR1cm5zIGV2ZXJ5dGhpbmcgYnV0IHRoZSBsYXN0IGVudHJ5IG9mIHRoZSBhcnJheS4gRXNwZWNpYWxseSB1c2VmdWwgb25cbiAgLy8gdGhlIGFyZ3VtZW50cyBvYmplY3QuIFBhc3NpbmcgKipuKiogd2lsbCByZXR1cm4gYWxsIHRoZSB2YWx1ZXMgaW5cbiAgLy8gdGhlIGFycmF5LCBleGNsdWRpbmcgdGhlIGxhc3QgTi5cbiAgXy5pbml0aWFsID0gZnVuY3Rpb24oYXJyYXksIG4sIGd1YXJkKSB7XG4gICAgcmV0dXJuIHNsaWNlLmNhbGwoYXJyYXksIDAsIE1hdGgubWF4KDAsIGFycmF5Lmxlbmd0aCAtIChuID09IG51bGwgfHwgZ3VhcmQgPyAxIDogbikpKTtcbiAgfTtcblxuICAvLyBHZXQgdGhlIGxhc3QgZWxlbWVudCBvZiBhbiBhcnJheS4gUGFzc2luZyAqKm4qKiB3aWxsIHJldHVybiB0aGUgbGFzdCBOXG4gIC8vIHZhbHVlcyBpbiB0aGUgYXJyYXkuXG4gIF8ubGFzdCA9IGZ1bmN0aW9uKGFycmF5LCBuLCBndWFyZCkge1xuICAgIGlmIChhcnJheSA9PSBudWxsKSByZXR1cm4gdm9pZCAwO1xuICAgIGlmIChuID09IG51bGwgfHwgZ3VhcmQpIHJldHVybiBhcnJheVthcnJheS5sZW5ndGggLSAxXTtcbiAgICByZXR1cm4gXy5yZXN0KGFycmF5LCBNYXRoLm1heCgwLCBhcnJheS5sZW5ndGggLSBuKSk7XG4gIH07XG5cbiAgLy8gUmV0dXJucyBldmVyeXRoaW5nIGJ1dCB0aGUgZmlyc3QgZW50cnkgb2YgdGhlIGFycmF5LiBBbGlhc2VkIGFzIGB0YWlsYCBhbmQgYGRyb3BgLlxuICAvLyBFc3BlY2lhbGx5IHVzZWZ1bCBvbiB0aGUgYXJndW1lbnRzIG9iamVjdC4gUGFzc2luZyBhbiAqKm4qKiB3aWxsIHJldHVyblxuICAvLyB0aGUgcmVzdCBOIHZhbHVlcyBpbiB0aGUgYXJyYXkuXG4gIF8ucmVzdCA9IF8udGFpbCA9IF8uZHJvcCA9IGZ1bmN0aW9uKGFycmF5LCBuLCBndWFyZCkge1xuICAgIHJldHVybiBzbGljZS5jYWxsKGFycmF5LCBuID09IG51bGwgfHwgZ3VhcmQgPyAxIDogbik7XG4gIH07XG5cbiAgLy8gVHJpbSBvdXQgYWxsIGZhbHN5IHZhbHVlcyBmcm9tIGFuIGFycmF5LlxuICBfLmNvbXBhY3QgPSBmdW5jdGlvbihhcnJheSkge1xuICAgIHJldHVybiBfLmZpbHRlcihhcnJheSwgXy5pZGVudGl0eSk7XG4gIH07XG5cbiAgLy8gSW50ZXJuYWwgaW1wbGVtZW50YXRpb24gb2YgYSByZWN1cnNpdmUgYGZsYXR0ZW5gIGZ1bmN0aW9uLlxuICB2YXIgZmxhdHRlbiA9IGZ1bmN0aW9uKGlucHV0LCBzaGFsbG93LCBzdHJpY3QsIHN0YXJ0SW5kZXgpIHtcbiAgICB2YXIgb3V0cHV0ID0gW10sIGlkeCA9IDA7XG4gICAgZm9yICh2YXIgaSA9IHN0YXJ0SW5kZXggfHwgMCwgbGVuZ3RoID0gZ2V0TGVuZ3RoKGlucHV0KTsgaSA8IGxlbmd0aDsgaSsrKSB7XG4gICAgICB2YXIgdmFsdWUgPSBpbnB1dFtpXTtcbiAgICAgIGlmIChpc0FycmF5TGlrZSh2YWx1ZSkgJiYgKF8uaXNBcnJheSh2YWx1ZSkgfHwgXy5pc0FyZ3VtZW50cyh2YWx1ZSkpKSB7XG4gICAgICAgIC8vZmxhdHRlbiBjdXJyZW50IGxldmVsIG9mIGFycmF5IG9yIGFyZ3VtZW50cyBvYmplY3RcbiAgICAgICAgaWYgKCFzaGFsbG93KSB2YWx1ZSA9IGZsYXR0ZW4odmFsdWUsIHNoYWxsb3csIHN0cmljdCk7XG4gICAgICAgIHZhciBqID0gMCwgbGVuID0gdmFsdWUubGVuZ3RoO1xuICAgICAgICBvdXRwdXQubGVuZ3RoICs9IGxlbjtcbiAgICAgICAgd2hpbGUgKGogPCBsZW4pIHtcbiAgICAgICAgICBvdXRwdXRbaWR4KytdID0gdmFsdWVbaisrXTtcbiAgICAgICAgfVxuICAgICAgfSBlbHNlIGlmICghc3RyaWN0KSB7XG4gICAgICAgIG91dHB1dFtpZHgrK10gPSB2YWx1ZTtcbiAgICAgIH1cbiAgICB9XG4gICAgcmV0dXJuIG91dHB1dDtcbiAgfTtcblxuICAvLyBGbGF0dGVuIG91dCBhbiBhcnJheSwgZWl0aGVyIHJlY3Vyc2l2ZWx5IChieSBkZWZhdWx0KSwgb3IganVzdCBvbmUgbGV2ZWwuXG4gIF8uZmxhdHRlbiA9IGZ1bmN0aW9uKGFycmF5LCBzaGFsbG93KSB7XG4gICAgcmV0dXJuIGZsYXR0ZW4oYXJyYXksIHNoYWxsb3csIGZhbHNlKTtcbiAgfTtcblxuICAvLyBSZXR1cm4gYSB2ZXJzaW9uIG9mIHRoZSBhcnJheSB0aGF0IGRvZXMgbm90IGNvbnRhaW4gdGhlIHNwZWNpZmllZCB2YWx1ZShzKS5cbiAgXy53aXRob3V0ID0gZnVuY3Rpb24oYXJyYXkpIHtcbiAgICByZXR1cm4gXy5kaWZmZXJlbmNlKGFycmF5LCBzbGljZS5jYWxsKGFyZ3VtZW50cywgMSkpO1xuICB9O1xuXG4gIC8vIFByb2R1Y2UgYSBkdXBsaWNhdGUtZnJlZSB2ZXJzaW9uIG9mIHRoZSBhcnJheS4gSWYgdGhlIGFycmF5IGhhcyBhbHJlYWR5XG4gIC8vIGJlZW4gc29ydGVkLCB5b3UgaGF2ZSB0aGUgb3B0aW9uIG9mIHVzaW5nIGEgZmFzdGVyIGFsZ29yaXRobS5cbiAgLy8gQWxpYXNlZCBhcyBgdW5pcXVlYC5cbiAgXy51bmlxID0gXy51bmlxdWUgPSBmdW5jdGlvbihhcnJheSwgaXNTb3J0ZWQsIGl0ZXJhdGVlLCBjb250ZXh0KSB7XG4gICAgaWYgKCFfLmlzQm9vbGVhbihpc1NvcnRlZCkpIHtcbiAgICAgIGNvbnRleHQgPSBpdGVyYXRlZTtcbiAgICAgIGl0ZXJhdGVlID0gaXNTb3J0ZWQ7XG4gICAgICBpc1NvcnRlZCA9IGZhbHNlO1xuICAgIH1cbiAgICBpZiAoaXRlcmF0ZWUgIT0gbnVsbCkgaXRlcmF0ZWUgPSBjYihpdGVyYXRlZSwgY29udGV4dCk7XG4gICAgdmFyIHJlc3VsdCA9IFtdO1xuICAgIHZhciBzZWVuID0gW107XG4gICAgZm9yICh2YXIgaSA9IDAsIGxlbmd0aCA9IGdldExlbmd0aChhcnJheSk7IGkgPCBsZW5ndGg7IGkrKykge1xuICAgICAgdmFyIHZhbHVlID0gYXJyYXlbaV0sXG4gICAgICAgICAgY29tcHV0ZWQgPSBpdGVyYXRlZSA/IGl0ZXJhdGVlKHZhbHVlLCBpLCBhcnJheSkgOiB2YWx1ZTtcbiAgICAgIGlmIChpc1NvcnRlZCkge1xuICAgICAgICBpZiAoIWkgfHwgc2VlbiAhPT0gY29tcHV0ZWQpIHJlc3VsdC5wdXNoKHZhbHVlKTtcbiAgICAgICAgc2VlbiA9IGNvbXB1dGVkO1xuICAgICAgfSBlbHNlIGlmIChpdGVyYXRlZSkge1xuICAgICAgICBpZiAoIV8uY29udGFpbnMoc2VlbiwgY29tcHV0ZWQpKSB7XG4gICAgICAgICAgc2Vlbi5wdXNoKGNvbXB1dGVkKTtcbiAgICAgICAgICByZXN1bHQucHVzaCh2YWx1ZSk7XG4gICAgICAgIH1cbiAgICAgIH0gZWxzZSBpZiAoIV8uY29udGFpbnMocmVzdWx0LCB2YWx1ZSkpIHtcbiAgICAgICAgcmVzdWx0LnB1c2godmFsdWUpO1xuICAgICAgfVxuICAgIH1cbiAgICByZXR1cm4gcmVzdWx0O1xuICB9O1xuXG4gIC8vIFByb2R1Y2UgYW4gYXJyYXkgdGhhdCBjb250YWlucyB0aGUgdW5pb246IGVhY2ggZGlzdGluY3QgZWxlbWVudCBmcm9tIGFsbCBvZlxuICAvLyB0aGUgcGFzc2VkLWluIGFycmF5cy5cbiAgXy51bmlvbiA9IGZ1bmN0aW9uKCkge1xuICAgIHJldHVybiBfLnVuaXEoZmxhdHRlbihhcmd1bWVudHMsIHRydWUsIHRydWUpKTtcbiAgfTtcblxuICAvLyBQcm9kdWNlIGFuIGFycmF5IHRoYXQgY29udGFpbnMgZXZlcnkgaXRlbSBzaGFyZWQgYmV0d2VlbiBhbGwgdGhlXG4gIC8vIHBhc3NlZC1pbiBhcnJheXMuXG4gIF8uaW50ZXJzZWN0aW9uID0gZnVuY3Rpb24oYXJyYXkpIHtcbiAgICB2YXIgcmVzdWx0ID0gW107XG4gICAgdmFyIGFyZ3NMZW5ndGggPSBhcmd1bWVudHMubGVuZ3RoO1xuICAgIGZvciAodmFyIGkgPSAwLCBsZW5ndGggPSBnZXRMZW5ndGgoYXJyYXkpOyBpIDwgbGVuZ3RoOyBpKyspIHtcbiAgICAgIHZhciBpdGVtID0gYXJyYXlbaV07XG4gICAgICBpZiAoXy5jb250YWlucyhyZXN1bHQsIGl0ZW0pKSBjb250aW51ZTtcbiAgICAgIGZvciAodmFyIGogPSAxOyBqIDwgYXJnc0xlbmd0aDsgaisrKSB7XG4gICAgICAgIGlmICghXy5jb250YWlucyhhcmd1bWVudHNbal0sIGl0ZW0pKSBicmVhaztcbiAgICAgIH1cbiAgICAgIGlmIChqID09PSBhcmdzTGVuZ3RoKSByZXN1bHQucHVzaChpdGVtKTtcbiAgICB9XG4gICAgcmV0dXJuIHJlc3VsdDtcbiAgfTtcblxuICAvLyBUYWtlIHRoZSBkaWZmZXJlbmNlIGJldHdlZW4gb25lIGFycmF5IGFuZCBhIG51bWJlciBvZiBvdGhlciBhcnJheXMuXG4gIC8vIE9ubHkgdGhlIGVsZW1lbnRzIHByZXNlbnQgaW4ganVzdCB0aGUgZmlyc3QgYXJyYXkgd2lsbCByZW1haW4uXG4gIF8uZGlmZmVyZW5jZSA9IGZ1bmN0aW9uKGFycmF5KSB7XG4gICAgdmFyIHJlc3QgPSBmbGF0dGVuKGFyZ3VtZW50cywgdHJ1ZSwgdHJ1ZSwgMSk7XG4gICAgcmV0dXJuIF8uZmlsdGVyKGFycmF5LCBmdW5jdGlvbih2YWx1ZSl7XG4gICAgICByZXR1cm4gIV8uY29udGFpbnMocmVzdCwgdmFsdWUpO1xuICAgIH0pO1xuICB9O1xuXG4gIC8vIFppcCB0b2dldGhlciBtdWx0aXBsZSBsaXN0cyBpbnRvIGEgc2luZ2xlIGFycmF5IC0tIGVsZW1lbnRzIHRoYXQgc2hhcmVcbiAgLy8gYW4gaW5kZXggZ28gdG9nZXRoZXIuXG4gIF8uemlwID0gZnVuY3Rpb24oKSB7XG4gICAgcmV0dXJuIF8udW56aXAoYXJndW1lbnRzKTtcbiAgfTtcblxuICAvLyBDb21wbGVtZW50IG9mIF8uemlwLiBVbnppcCBhY2NlcHRzIGFuIGFycmF5IG9mIGFycmF5cyBhbmQgZ3JvdXBzXG4gIC8vIGVhY2ggYXJyYXkncyBlbGVtZW50cyBvbiBzaGFyZWQgaW5kaWNlc1xuICBfLnVuemlwID0gZnVuY3Rpb24oYXJyYXkpIHtcbiAgICB2YXIgbGVuZ3RoID0gYXJyYXkgJiYgXy5tYXgoYXJyYXksIGdldExlbmd0aCkubGVuZ3RoIHx8IDA7XG4gICAgdmFyIHJlc3VsdCA9IEFycmF5KGxlbmd0aCk7XG5cbiAgICBmb3IgKHZhciBpbmRleCA9IDA7IGluZGV4IDwgbGVuZ3RoOyBpbmRleCsrKSB7XG4gICAgICByZXN1bHRbaW5kZXhdID0gXy5wbHVjayhhcnJheSwgaW5kZXgpO1xuICAgIH1cbiAgICByZXR1cm4gcmVzdWx0O1xuICB9O1xuXG4gIC8vIENvbnZlcnRzIGxpc3RzIGludG8gb2JqZWN0cy4gUGFzcyBlaXRoZXIgYSBzaW5nbGUgYXJyYXkgb2YgYFtrZXksIHZhbHVlXWBcbiAgLy8gcGFpcnMsIG9yIHR3byBwYXJhbGxlbCBhcnJheXMgb2YgdGhlIHNhbWUgbGVuZ3RoIC0tIG9uZSBvZiBrZXlzLCBhbmQgb25lIG9mXG4gIC8vIHRoZSBjb3JyZXNwb25kaW5nIHZhbHVlcy5cbiAgXy5vYmplY3QgPSBmdW5jdGlvbihsaXN0LCB2YWx1ZXMpIHtcbiAgICB2YXIgcmVzdWx0ID0ge307XG4gICAgZm9yICh2YXIgaSA9IDAsIGxlbmd0aCA9IGdldExlbmd0aChsaXN0KTsgaSA8IGxlbmd0aDsgaSsrKSB7XG4gICAgICBpZiAodmFsdWVzKSB7XG4gICAgICAgIHJlc3VsdFtsaXN0W2ldXSA9IHZhbHVlc1tpXTtcbiAgICAgIH0gZWxzZSB7XG4gICAgICAgIHJlc3VsdFtsaXN0W2ldWzBdXSA9IGxpc3RbaV1bMV07XG4gICAgICB9XG4gICAgfVxuICAgIHJldHVybiByZXN1bHQ7XG4gIH07XG5cbiAgLy8gR2VuZXJhdG9yIGZ1bmN0aW9uIHRvIGNyZWF0ZSB0aGUgZmluZEluZGV4IGFuZCBmaW5kTGFzdEluZGV4IGZ1bmN0aW9uc1xuICBmdW5jdGlvbiBjcmVhdGVQcmVkaWNhdGVJbmRleEZpbmRlcihkaXIpIHtcbiAgICByZXR1cm4gZnVuY3Rpb24oYXJyYXksIHByZWRpY2F0ZSwgY29udGV4dCkge1xuICAgICAgcHJlZGljYXRlID0gY2IocHJlZGljYXRlLCBjb250ZXh0KTtcbiAgICAgIHZhciBsZW5ndGggPSBnZXRMZW5ndGgoYXJyYXkpO1xuICAgICAgdmFyIGluZGV4ID0gZGlyID4gMCA/IDAgOiBsZW5ndGggLSAxO1xuICAgICAgZm9yICg7IGluZGV4ID49IDAgJiYgaW5kZXggPCBsZW5ndGg7IGluZGV4ICs9IGRpcikge1xuICAgICAgICBpZiAocHJlZGljYXRlKGFycmF5W2luZGV4XSwgaW5kZXgsIGFycmF5KSkgcmV0dXJuIGluZGV4O1xuICAgICAgfVxuICAgICAgcmV0dXJuIC0xO1xuICAgIH07XG4gIH1cblxuICAvLyBSZXR1cm5zIHRoZSBmaXJzdCBpbmRleCBvbiBhbiBhcnJheS1saWtlIHRoYXQgcGFzc2VzIGEgcHJlZGljYXRlIHRlc3RcbiAgXy5maW5kSW5kZXggPSBjcmVhdGVQcmVkaWNhdGVJbmRleEZpbmRlcigxKTtcbiAgXy5maW5kTGFzdEluZGV4ID0gY3JlYXRlUHJlZGljYXRlSW5kZXhGaW5kZXIoLTEpO1xuXG4gIC8vIFVzZSBhIGNvbXBhcmF0b3IgZnVuY3Rpb24gdG8gZmlndXJlIG91dCB0aGUgc21hbGxlc3QgaW5kZXggYXQgd2hpY2hcbiAgLy8gYW4gb2JqZWN0IHNob3VsZCBiZSBpbnNlcnRlZCBzbyBhcyB0byBtYWludGFpbiBvcmRlci4gVXNlcyBiaW5hcnkgc2VhcmNoLlxuICBfLnNvcnRlZEluZGV4ID0gZnVuY3Rpb24oYXJyYXksIG9iaiwgaXRlcmF0ZWUsIGNvbnRleHQpIHtcbiAgICBpdGVyYXRlZSA9IGNiKGl0ZXJhdGVlLCBjb250ZXh0LCAxKTtcbiAgICB2YXIgdmFsdWUgPSBpdGVyYXRlZShvYmopO1xuICAgIHZhciBsb3cgPSAwLCBoaWdoID0gZ2V0TGVuZ3RoKGFycmF5KTtcbiAgICB3aGlsZSAobG93IDwgaGlnaCkge1xuICAgICAgdmFyIG1pZCA9IE1hdGguZmxvb3IoKGxvdyArIGhpZ2gpIC8gMik7XG4gICAgICBpZiAoaXRlcmF0ZWUoYXJyYXlbbWlkXSkgPCB2YWx1ZSkgbG93ID0gbWlkICsgMTsgZWxzZSBoaWdoID0gbWlkO1xuICAgIH1cbiAgICByZXR1cm4gbG93O1xuICB9O1xuXG4gIC8vIEdlbmVyYXRvciBmdW5jdGlvbiB0byBjcmVhdGUgdGhlIGluZGV4T2YgYW5kIGxhc3RJbmRleE9mIGZ1bmN0aW9uc1xuICBmdW5jdGlvbiBjcmVhdGVJbmRleEZpbmRlcihkaXIsIHByZWRpY2F0ZUZpbmQsIHNvcnRlZEluZGV4KSB7XG4gICAgcmV0dXJuIGZ1bmN0aW9uKGFycmF5LCBpdGVtLCBpZHgpIHtcbiAgICAgIHZhciBpID0gMCwgbGVuZ3RoID0gZ2V0TGVuZ3RoKGFycmF5KTtcbiAgICAgIGlmICh0eXBlb2YgaWR4ID09ICdudW1iZXInKSB7XG4gICAgICAgIGlmIChkaXIgPiAwKSB7XG4gICAgICAgICAgICBpID0gaWR4ID49IDAgPyBpZHggOiBNYXRoLm1heChpZHggKyBsZW5ndGgsIGkpO1xuICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgbGVuZ3RoID0gaWR4ID49IDAgPyBNYXRoLm1pbihpZHggKyAxLCBsZW5ndGgpIDogaWR4ICsgbGVuZ3RoICsgMTtcbiAgICAgICAgfVxuICAgICAgfSBlbHNlIGlmIChzb3J0ZWRJbmRleCAmJiBpZHggJiYgbGVuZ3RoKSB7XG4gICAgICAgIGlkeCA9IHNvcnRlZEluZGV4KGFycmF5LCBpdGVtKTtcbiAgICAgICAgcmV0dXJuIGFycmF5W2lkeF0gPT09IGl0ZW0gPyBpZHggOiAtMTtcbiAgICAgIH1cbiAgICAgIGlmIChpdGVtICE9PSBpdGVtKSB7XG4gICAgICAgIGlkeCA9IHByZWRpY2F0ZUZpbmQoc2xpY2UuY2FsbChhcnJheSwgaSwgbGVuZ3RoKSwgXy5pc05hTik7XG4gICAgICAgIHJldHVybiBpZHggPj0gMCA/IGlkeCArIGkgOiAtMTtcbiAgICAgIH1cbiAgICAgIGZvciAoaWR4ID0gZGlyID4gMCA/IGkgOiBsZW5ndGggLSAxOyBpZHggPj0gMCAmJiBpZHggPCBsZW5ndGg7IGlkeCArPSBkaXIpIHtcbiAgICAgICAgaWYgKGFycmF5W2lkeF0gPT09IGl0ZW0pIHJldHVybiBpZHg7XG4gICAgICB9XG4gICAgICByZXR1cm4gLTE7XG4gICAgfTtcbiAgfVxuXG4gIC8vIFJldHVybiB0aGUgcG9zaXRpb24gb2YgdGhlIGZpcnN0IG9jY3VycmVuY2Ugb2YgYW4gaXRlbSBpbiBhbiBhcnJheSxcbiAgLy8gb3IgLTEgaWYgdGhlIGl0ZW0gaXMgbm90IGluY2x1ZGVkIGluIHRoZSBhcnJheS5cbiAgLy8gSWYgdGhlIGFycmF5IGlzIGxhcmdlIGFuZCBhbHJlYWR5IGluIHNvcnQgb3JkZXIsIHBhc3MgYHRydWVgXG4gIC8vIGZvciAqKmlzU29ydGVkKiogdG8gdXNlIGJpbmFyeSBzZWFyY2guXG4gIF8uaW5kZXhPZiA9IGNyZWF0ZUluZGV4RmluZGVyKDEsIF8uZmluZEluZGV4LCBfLnNvcnRlZEluZGV4KTtcbiAgXy5sYXN0SW5kZXhPZiA9IGNyZWF0ZUluZGV4RmluZGVyKC0xLCBfLmZpbmRMYXN0SW5kZXgpO1xuXG4gIC8vIEdlbmVyYXRlIGFuIGludGVnZXIgQXJyYXkgY29udGFpbmluZyBhbiBhcml0aG1ldGljIHByb2dyZXNzaW9uLiBBIHBvcnQgb2ZcbiAgLy8gdGhlIG5hdGl2ZSBQeXRob24gYHJhbmdlKClgIGZ1bmN0aW9uLiBTZWVcbiAgLy8gW3RoZSBQeXRob24gZG9jdW1lbnRhdGlvbl0oaHR0cDovL2RvY3MucHl0aG9uLm9yZy9saWJyYXJ5L2Z1bmN0aW9ucy5odG1sI3JhbmdlKS5cbiAgXy5yYW5nZSA9IGZ1bmN0aW9uKHN0YXJ0LCBzdG9wLCBzdGVwKSB7XG4gICAgaWYgKHN0b3AgPT0gbnVsbCkge1xuICAgICAgc3RvcCA9IHN0YXJ0IHx8IDA7XG4gICAgICBzdGFydCA9IDA7XG4gICAgfVxuICAgIHN0ZXAgPSBzdGVwIHx8IDE7XG5cbiAgICB2YXIgbGVuZ3RoID0gTWF0aC5tYXgoTWF0aC5jZWlsKChzdG9wIC0gc3RhcnQpIC8gc3RlcCksIDApO1xuICAgIHZhciByYW5nZSA9IEFycmF5KGxlbmd0aCk7XG5cbiAgICBmb3IgKHZhciBpZHggPSAwOyBpZHggPCBsZW5ndGg7IGlkeCsrLCBzdGFydCArPSBzdGVwKSB7XG4gICAgICByYW5nZVtpZHhdID0gc3RhcnQ7XG4gICAgfVxuXG4gICAgcmV0dXJuIHJhbmdlO1xuICB9O1xuXG4gIC8vIEZ1bmN0aW9uIChhaGVtKSBGdW5jdGlvbnNcbiAgLy8gLS0tLS0tLS0tLS0tLS0tLS0tXG5cbiAgLy8gRGV0ZXJtaW5lcyB3aGV0aGVyIHRvIGV4ZWN1dGUgYSBmdW5jdGlvbiBhcyBhIGNvbnN0cnVjdG9yXG4gIC8vIG9yIGEgbm9ybWFsIGZ1bmN0aW9uIHdpdGggdGhlIHByb3ZpZGVkIGFyZ3VtZW50c1xuICB2YXIgZXhlY3V0ZUJvdW5kID0gZnVuY3Rpb24oc291cmNlRnVuYywgYm91bmRGdW5jLCBjb250ZXh0LCBjYWxsaW5nQ29udGV4dCwgYXJncykge1xuICAgIGlmICghKGNhbGxpbmdDb250ZXh0IGluc3RhbmNlb2YgYm91bmRGdW5jKSkgcmV0dXJuIHNvdXJjZUZ1bmMuYXBwbHkoY29udGV4dCwgYXJncyk7XG4gICAgdmFyIHNlbGYgPSBiYXNlQ3JlYXRlKHNvdXJjZUZ1bmMucHJvdG90eXBlKTtcbiAgICB2YXIgcmVzdWx0ID0gc291cmNlRnVuYy5hcHBseShzZWxmLCBhcmdzKTtcbiAgICBpZiAoXy5pc09iamVjdChyZXN1bHQpKSByZXR1cm4gcmVzdWx0O1xuICAgIHJldHVybiBzZWxmO1xuICB9O1xuXG4gIC8vIENyZWF0ZSBhIGZ1bmN0aW9uIGJvdW5kIHRvIGEgZ2l2ZW4gb2JqZWN0IChhc3NpZ25pbmcgYHRoaXNgLCBhbmQgYXJndW1lbnRzLFxuICAvLyBvcHRpb25hbGx5KS4gRGVsZWdhdGVzIHRvICoqRUNNQVNjcmlwdCA1KioncyBuYXRpdmUgYEZ1bmN0aW9uLmJpbmRgIGlmXG4gIC8vIGF2YWlsYWJsZS5cbiAgXy5iaW5kID0gZnVuY3Rpb24oZnVuYywgY29udGV4dCkge1xuICAgIGlmIChuYXRpdmVCaW5kICYmIGZ1bmMuYmluZCA9PT0gbmF0aXZlQmluZCkgcmV0dXJuIG5hdGl2ZUJpbmQuYXBwbHkoZnVuYywgc2xpY2UuY2FsbChhcmd1bWVudHMsIDEpKTtcbiAgICBpZiAoIV8uaXNGdW5jdGlvbihmdW5jKSkgdGhyb3cgbmV3IFR5cGVFcnJvcignQmluZCBtdXN0IGJlIGNhbGxlZCBvbiBhIGZ1bmN0aW9uJyk7XG4gICAgdmFyIGFyZ3MgPSBzbGljZS5jYWxsKGFyZ3VtZW50cywgMik7XG4gICAgdmFyIGJvdW5kID0gZnVuY3Rpb24oKSB7XG4gICAgICByZXR1cm4gZXhlY3V0ZUJvdW5kKGZ1bmMsIGJvdW5kLCBjb250ZXh0LCB0aGlzLCBhcmdzLmNvbmNhdChzbGljZS5jYWxsKGFyZ3VtZW50cykpKTtcbiAgICB9O1xuICAgIHJldHVybiBib3VuZDtcbiAgfTtcblxuICAvLyBQYXJ0aWFsbHkgYXBwbHkgYSBmdW5jdGlvbiBieSBjcmVhdGluZyBhIHZlcnNpb24gdGhhdCBoYXMgaGFkIHNvbWUgb2YgaXRzXG4gIC8vIGFyZ3VtZW50cyBwcmUtZmlsbGVkLCB3aXRob3V0IGNoYW5naW5nIGl0cyBkeW5hbWljIGB0aGlzYCBjb250ZXh0LiBfIGFjdHNcbiAgLy8gYXMgYSBwbGFjZWhvbGRlciwgYWxsb3dpbmcgYW55IGNvbWJpbmF0aW9uIG9mIGFyZ3VtZW50cyB0byBiZSBwcmUtZmlsbGVkLlxuICBfLnBhcnRpYWwgPSBmdW5jdGlvbihmdW5jKSB7XG4gICAgdmFyIGJvdW5kQXJncyA9IHNsaWNlLmNhbGwoYXJndW1lbnRzLCAxKTtcbiAgICB2YXIgYm91bmQgPSBmdW5jdGlvbigpIHtcbiAgICAgIHZhciBwb3NpdGlvbiA9IDAsIGxlbmd0aCA9IGJvdW5kQXJncy5sZW5ndGg7XG4gICAgICB2YXIgYXJncyA9IEFycmF5KGxlbmd0aCk7XG4gICAgICBmb3IgKHZhciBpID0gMDsgaSA8IGxlbmd0aDsgaSsrKSB7XG4gICAgICAgIGFyZ3NbaV0gPSBib3VuZEFyZ3NbaV0gPT09IF8gPyBhcmd1bWVudHNbcG9zaXRpb24rK10gOiBib3VuZEFyZ3NbaV07XG4gICAgICB9XG4gICAgICB3aGlsZSAocG9zaXRpb24gPCBhcmd1bWVudHMubGVuZ3RoKSBhcmdzLnB1c2goYXJndW1lbnRzW3Bvc2l0aW9uKytdKTtcbiAgICAgIHJldHVybiBleGVjdXRlQm91bmQoZnVuYywgYm91bmQsIHRoaXMsIHRoaXMsIGFyZ3MpO1xuICAgIH07XG4gICAgcmV0dXJuIGJvdW5kO1xuICB9O1xuXG4gIC8vIEJpbmQgYSBudW1iZXIgb2YgYW4gb2JqZWN0J3MgbWV0aG9kcyB0byB0aGF0IG9iamVjdC4gUmVtYWluaW5nIGFyZ3VtZW50c1xuICAvLyBhcmUgdGhlIG1ldGhvZCBuYW1lcyB0byBiZSBib3VuZC4gVXNlZnVsIGZvciBlbnN1cmluZyB0aGF0IGFsbCBjYWxsYmFja3NcbiAgLy8gZGVmaW5lZCBvbiBhbiBvYmplY3QgYmVsb25nIHRvIGl0LlxuICBfLmJpbmRBbGwgPSBmdW5jdGlvbihvYmopIHtcbiAgICB2YXIgaSwgbGVuZ3RoID0gYXJndW1lbnRzLmxlbmd0aCwga2V5O1xuICAgIGlmIChsZW5ndGggPD0gMSkgdGhyb3cgbmV3IEVycm9yKCdiaW5kQWxsIG11c3QgYmUgcGFzc2VkIGZ1bmN0aW9uIG5hbWVzJyk7XG4gICAgZm9yIChpID0gMTsgaSA8IGxlbmd0aDsgaSsrKSB7XG4gICAgICBrZXkgPSBhcmd1bWVudHNbaV07XG4gICAgICBvYmpba2V5XSA9IF8uYmluZChvYmpba2V5XSwgb2JqKTtcbiAgICB9XG4gICAgcmV0dXJuIG9iajtcbiAgfTtcblxuICAvLyBNZW1vaXplIGFuIGV4cGVuc2l2ZSBmdW5jdGlvbiBieSBzdG9yaW5nIGl0cyByZXN1bHRzLlxuICBfLm1lbW9pemUgPSBmdW5jdGlvbihmdW5jLCBoYXNoZXIpIHtcbiAgICB2YXIgbWVtb2l6ZSA9IGZ1bmN0aW9uKGtleSkge1xuICAgICAgdmFyIGNhY2hlID0gbWVtb2l6ZS5jYWNoZTtcbiAgICAgIHZhciBhZGRyZXNzID0gJycgKyAoaGFzaGVyID8gaGFzaGVyLmFwcGx5KHRoaXMsIGFyZ3VtZW50cykgOiBrZXkpO1xuICAgICAgaWYgKCFfLmhhcyhjYWNoZSwgYWRkcmVzcykpIGNhY2hlW2FkZHJlc3NdID0gZnVuYy5hcHBseSh0aGlzLCBhcmd1bWVudHMpO1xuICAgICAgcmV0dXJuIGNhY2hlW2FkZHJlc3NdO1xuICAgIH07XG4gICAgbWVtb2l6ZS5jYWNoZSA9IHt9O1xuICAgIHJldHVybiBtZW1vaXplO1xuICB9O1xuXG4gIC8vIERlbGF5cyBhIGZ1bmN0aW9uIGZvciB0aGUgZ2l2ZW4gbnVtYmVyIG9mIG1pbGxpc2Vjb25kcywgYW5kIHRoZW4gY2FsbHNcbiAgLy8gaXQgd2l0aCB0aGUgYXJndW1lbnRzIHN1cHBsaWVkLlxuICBfLmRlbGF5ID0gZnVuY3Rpb24oZnVuYywgd2FpdCkge1xuICAgIHZhciBhcmdzID0gc2xpY2UuY2FsbChhcmd1bWVudHMsIDIpO1xuICAgIHJldHVybiBzZXRUaW1lb3V0KGZ1bmN0aW9uKCl7XG4gICAgICByZXR1cm4gZnVuYy5hcHBseShudWxsLCBhcmdzKTtcbiAgICB9LCB3YWl0KTtcbiAgfTtcblxuICAvLyBEZWZlcnMgYSBmdW5jdGlvbiwgc2NoZWR1bGluZyBpdCB0byBydW4gYWZ0ZXIgdGhlIGN1cnJlbnQgY2FsbCBzdGFjayBoYXNcbiAgLy8gY2xlYXJlZC5cbiAgXy5kZWZlciA9IF8ucGFydGlhbChfLmRlbGF5LCBfLCAxKTtcblxuICAvLyBSZXR1cm5zIGEgZnVuY3Rpb24sIHRoYXQsIHdoZW4gaW52b2tlZCwgd2lsbCBvbmx5IGJlIHRyaWdnZXJlZCBhdCBtb3N0IG9uY2VcbiAgLy8gZHVyaW5nIGEgZ2l2ZW4gd2luZG93IG9mIHRpbWUuIE5vcm1hbGx5LCB0aGUgdGhyb3R0bGVkIGZ1bmN0aW9uIHdpbGwgcnVuXG4gIC8vIGFzIG11Y2ggYXMgaXQgY2FuLCB3aXRob3V0IGV2ZXIgZ29pbmcgbW9yZSB0aGFuIG9uY2UgcGVyIGB3YWl0YCBkdXJhdGlvbjtcbiAgLy8gYnV0IGlmIHlvdSdkIGxpa2UgdG8gZGlzYWJsZSB0aGUgZXhlY3V0aW9uIG9uIHRoZSBsZWFkaW5nIGVkZ2UsIHBhc3NcbiAgLy8gYHtsZWFkaW5nOiBmYWxzZX1gLiBUbyBkaXNhYmxlIGV4ZWN1dGlvbiBvbiB0aGUgdHJhaWxpbmcgZWRnZSwgZGl0dG8uXG4gIF8udGhyb3R0bGUgPSBmdW5jdGlvbihmdW5jLCB3YWl0LCBvcHRpb25zKSB7XG4gICAgdmFyIGNvbnRleHQsIGFyZ3MsIHJlc3VsdDtcbiAgICB2YXIgdGltZW91dCA9IG51bGw7XG4gICAgdmFyIHByZXZpb3VzID0gMDtcbiAgICBpZiAoIW9wdGlvbnMpIG9wdGlvbnMgPSB7fTtcbiAgICB2YXIgbGF0ZXIgPSBmdW5jdGlvbigpIHtcbiAgICAgIHByZXZpb3VzID0gb3B0aW9ucy5sZWFkaW5nID09PSBmYWxzZSA/IDAgOiBfLm5vdygpO1xuICAgICAgdGltZW91dCA9IG51bGw7XG4gICAgICByZXN1bHQgPSBmdW5jLmFwcGx5KGNvbnRleHQsIGFyZ3MpO1xuICAgICAgaWYgKCF0aW1lb3V0KSBjb250ZXh0ID0gYXJncyA9IG51bGw7XG4gICAgfTtcbiAgICByZXR1cm4gZnVuY3Rpb24oKSB7XG4gICAgICB2YXIgbm93ID0gXy5ub3coKTtcbiAgICAgIGlmICghcHJldmlvdXMgJiYgb3B0aW9ucy5sZWFkaW5nID09PSBmYWxzZSkgcHJldmlvdXMgPSBub3c7XG4gICAgICB2YXIgcmVtYWluaW5nID0gd2FpdCAtIChub3cgLSBwcmV2aW91cyk7XG4gICAgICBjb250ZXh0ID0gdGhpcztcbiAgICAgIGFyZ3MgPSBhcmd1bWVudHM7XG4gICAgICBpZiAocmVtYWluaW5nIDw9IDAgfHwgcmVtYWluaW5nID4gd2FpdCkge1xuICAgICAgICBpZiAodGltZW91dCkge1xuICAgICAgICAgIGNsZWFyVGltZW91dCh0aW1lb3V0KTtcbiAgICAgICAgICB0aW1lb3V0ID0gbnVsbDtcbiAgICAgICAgfVxuICAgICAgICBwcmV2aW91cyA9IG5vdztcbiAgICAgICAgcmVzdWx0ID0gZnVuYy5hcHBseShjb250ZXh0LCBhcmdzKTtcbiAgICAgICAgaWYgKCF0aW1lb3V0KSBjb250ZXh0ID0gYXJncyA9IG51bGw7XG4gICAgICB9IGVsc2UgaWYgKCF0aW1lb3V0ICYmIG9wdGlvbnMudHJhaWxpbmcgIT09IGZhbHNlKSB7XG4gICAgICAgIHRpbWVvdXQgPSBzZXRUaW1lb3V0KGxhdGVyLCByZW1haW5pbmcpO1xuICAgICAgfVxuICAgICAgcmV0dXJuIHJlc3VsdDtcbiAgICB9O1xuICB9O1xuXG4gIC8vIFJldHVybnMgYSBmdW5jdGlvbiwgdGhhdCwgYXMgbG9uZyBhcyBpdCBjb250aW51ZXMgdG8gYmUgaW52b2tlZCwgd2lsbCBub3RcbiAgLy8gYmUgdHJpZ2dlcmVkLiBUaGUgZnVuY3Rpb24gd2lsbCBiZSBjYWxsZWQgYWZ0ZXIgaXQgc3RvcHMgYmVpbmcgY2FsbGVkIGZvclxuICAvLyBOIG1pbGxpc2Vjb25kcy4gSWYgYGltbWVkaWF0ZWAgaXMgcGFzc2VkLCB0cmlnZ2VyIHRoZSBmdW5jdGlvbiBvbiB0aGVcbiAgLy8gbGVhZGluZyBlZGdlLCBpbnN0ZWFkIG9mIHRoZSB0cmFpbGluZy5cbiAgXy5kZWJvdW5jZSA9IGZ1bmN0aW9uKGZ1bmMsIHdhaXQsIGltbWVkaWF0ZSkge1xuICAgIHZhciB0aW1lb3V0LCBhcmdzLCBjb250ZXh0LCB0aW1lc3RhbXAsIHJlc3VsdDtcblxuICAgIHZhciBsYXRlciA9IGZ1bmN0aW9uKCkge1xuICAgICAgdmFyIGxhc3QgPSBfLm5vdygpIC0gdGltZXN0YW1wO1xuXG4gICAgICBpZiAobGFzdCA8IHdhaXQgJiYgbGFzdCA+PSAwKSB7XG4gICAgICAgIHRpbWVvdXQgPSBzZXRUaW1lb3V0KGxhdGVyLCB3YWl0IC0gbGFzdCk7XG4gICAgICB9IGVsc2Uge1xuICAgICAgICB0aW1lb3V0ID0gbnVsbDtcbiAgICAgICAgaWYgKCFpbW1lZGlhdGUpIHtcbiAgICAgICAgICByZXN1bHQgPSBmdW5jLmFwcGx5KGNvbnRleHQsIGFyZ3MpO1xuICAgICAgICAgIGlmICghdGltZW91dCkgY29udGV4dCA9IGFyZ3MgPSBudWxsO1xuICAgICAgICB9XG4gICAgICB9XG4gICAgfTtcblxuICAgIHJldHVybiBmdW5jdGlvbigpIHtcbiAgICAgIGNvbnRleHQgPSB0aGlzO1xuICAgICAgYXJncyA9IGFyZ3VtZW50cztcbiAgICAgIHRpbWVzdGFtcCA9IF8ubm93KCk7XG4gICAgICB2YXIgY2FsbE5vdyA9IGltbWVkaWF0ZSAmJiAhdGltZW91dDtcbiAgICAgIGlmICghdGltZW91dCkgdGltZW91dCA9IHNldFRpbWVvdXQobGF0ZXIsIHdhaXQpO1xuICAgICAgaWYgKGNhbGxOb3cpIHtcbiAgICAgICAgcmVzdWx0ID0gZnVuYy5hcHBseShjb250ZXh0LCBhcmdzKTtcbiAgICAgICAgY29udGV4dCA9IGFyZ3MgPSBudWxsO1xuICAgICAgfVxuXG4gICAgICByZXR1cm4gcmVzdWx0O1xuICAgIH07XG4gIH07XG5cbiAgLy8gUmV0dXJucyB0aGUgZmlyc3QgZnVuY3Rpb24gcGFzc2VkIGFzIGFuIGFyZ3VtZW50IHRvIHRoZSBzZWNvbmQsXG4gIC8vIGFsbG93aW5nIHlvdSB0byBhZGp1c3QgYXJndW1lbnRzLCBydW4gY29kZSBiZWZvcmUgYW5kIGFmdGVyLCBhbmRcbiAgLy8gY29uZGl0aW9uYWxseSBleGVjdXRlIHRoZSBvcmlnaW5hbCBmdW5jdGlvbi5cbiAgXy53cmFwID0gZnVuY3Rpb24oZnVuYywgd3JhcHBlcikge1xuICAgIHJldHVybiBfLnBhcnRpYWwod3JhcHBlciwgZnVuYyk7XG4gIH07XG5cbiAgLy8gUmV0dXJucyBhIG5lZ2F0ZWQgdmVyc2lvbiBvZiB0aGUgcGFzc2VkLWluIHByZWRpY2F0ZS5cbiAgXy5uZWdhdGUgPSBmdW5jdGlvbihwcmVkaWNhdGUpIHtcbiAgICByZXR1cm4gZnVuY3Rpb24oKSB7XG4gICAgICByZXR1cm4gIXByZWRpY2F0ZS5hcHBseSh0aGlzLCBhcmd1bWVudHMpO1xuICAgIH07XG4gIH07XG5cbiAgLy8gUmV0dXJucyBhIGZ1bmN0aW9uIHRoYXQgaXMgdGhlIGNvbXBvc2l0aW9uIG9mIGEgbGlzdCBvZiBmdW5jdGlvbnMsIGVhY2hcbiAgLy8gY29uc3VtaW5nIHRoZSByZXR1cm4gdmFsdWUgb2YgdGhlIGZ1bmN0aW9uIHRoYXQgZm9sbG93cy5cbiAgXy5jb21wb3NlID0gZnVuY3Rpb24oKSB7XG4gICAgdmFyIGFyZ3MgPSBhcmd1bWVudHM7XG4gICAgdmFyIHN0YXJ0ID0gYXJncy5sZW5ndGggLSAxO1xuICAgIHJldHVybiBmdW5jdGlvbigpIHtcbiAgICAgIHZhciBpID0gc3RhcnQ7XG4gICAgICB2YXIgcmVzdWx0ID0gYXJnc1tzdGFydF0uYXBwbHkodGhpcywgYXJndW1lbnRzKTtcbiAgICAgIHdoaWxlIChpLS0pIHJlc3VsdCA9IGFyZ3NbaV0uY2FsbCh0aGlzLCByZXN1bHQpO1xuICAgICAgcmV0dXJuIHJlc3VsdDtcbiAgICB9O1xuICB9O1xuXG4gIC8vIFJldHVybnMgYSBmdW5jdGlvbiB0aGF0IHdpbGwgb25seSBiZSBleGVjdXRlZCBvbiBhbmQgYWZ0ZXIgdGhlIE50aCBjYWxsLlxuICBfLmFmdGVyID0gZnVuY3Rpb24odGltZXMsIGZ1bmMpIHtcbiAgICByZXR1cm4gZnVuY3Rpb24oKSB7XG4gICAgICBpZiAoLS10aW1lcyA8IDEpIHtcbiAgICAgICAgcmV0dXJuIGZ1bmMuYXBwbHkodGhpcywgYXJndW1lbnRzKTtcbiAgICAgIH1cbiAgICB9O1xuICB9O1xuXG4gIC8vIFJldHVybnMgYSBmdW5jdGlvbiB0aGF0IHdpbGwgb25seSBiZSBleGVjdXRlZCB1cCB0byAoYnV0IG5vdCBpbmNsdWRpbmcpIHRoZSBOdGggY2FsbC5cbiAgXy5iZWZvcmUgPSBmdW5jdGlvbih0aW1lcywgZnVuYykge1xuICAgIHZhciBtZW1vO1xuICAgIHJldHVybiBmdW5jdGlvbigpIHtcbiAgICAgIGlmICgtLXRpbWVzID4gMCkge1xuICAgICAgICBtZW1vID0gZnVuYy5hcHBseSh0aGlzLCBhcmd1bWVudHMpO1xuICAgICAgfVxuICAgICAgaWYgKHRpbWVzIDw9IDEpIGZ1bmMgPSBudWxsO1xuICAgICAgcmV0dXJuIG1lbW87XG4gICAgfTtcbiAgfTtcblxuICAvLyBSZXR1cm5zIGEgZnVuY3Rpb24gdGhhdCB3aWxsIGJlIGV4ZWN1dGVkIGF0IG1vc3Qgb25lIHRpbWUsIG5vIG1hdHRlciBob3dcbiAgLy8gb2Z0ZW4geW91IGNhbGwgaXQuIFVzZWZ1bCBmb3IgbGF6eSBpbml0aWFsaXphdGlvbi5cbiAgXy5vbmNlID0gXy5wYXJ0aWFsKF8uYmVmb3JlLCAyKTtcblxuICAvLyBPYmplY3QgRnVuY3Rpb25zXG4gIC8vIC0tLS0tLS0tLS0tLS0tLS1cblxuICAvLyBLZXlzIGluIElFIDwgOSB0aGF0IHdvbid0IGJlIGl0ZXJhdGVkIGJ5IGBmb3Iga2V5IGluIC4uLmAgYW5kIHRodXMgbWlzc2VkLlxuICB2YXIgaGFzRW51bUJ1ZyA9ICF7dG9TdHJpbmc6IG51bGx9LnByb3BlcnR5SXNFbnVtZXJhYmxlKCd0b1N0cmluZycpO1xuICB2YXIgbm9uRW51bWVyYWJsZVByb3BzID0gWyd2YWx1ZU9mJywgJ2lzUHJvdG90eXBlT2YnLCAndG9TdHJpbmcnLFxuICAgICAgICAgICAgICAgICAgICAgICdwcm9wZXJ0eUlzRW51bWVyYWJsZScsICdoYXNPd25Qcm9wZXJ0eScsICd0b0xvY2FsZVN0cmluZyddO1xuXG4gIGZ1bmN0aW9uIGNvbGxlY3ROb25FbnVtUHJvcHMob2JqLCBrZXlzKSB7XG4gICAgdmFyIG5vbkVudW1JZHggPSBub25FbnVtZXJhYmxlUHJvcHMubGVuZ3RoO1xuICAgIHZhciBjb25zdHJ1Y3RvciA9IG9iai5jb25zdHJ1Y3RvcjtcbiAgICB2YXIgcHJvdG8gPSAoXy5pc0Z1bmN0aW9uKGNvbnN0cnVjdG9yKSAmJiBjb25zdHJ1Y3Rvci5wcm90b3R5cGUpIHx8IE9ialByb3RvO1xuXG4gICAgLy8gQ29uc3RydWN0b3IgaXMgYSBzcGVjaWFsIGNhc2UuXG4gICAgdmFyIHByb3AgPSAnY29uc3RydWN0b3InO1xuICAgIGlmIChfLmhhcyhvYmosIHByb3ApICYmICFfLmNvbnRhaW5zKGtleXMsIHByb3ApKSBrZXlzLnB1c2gocHJvcCk7XG5cbiAgICB3aGlsZSAobm9uRW51bUlkeC0tKSB7XG4gICAgICBwcm9wID0gbm9uRW51bWVyYWJsZVByb3BzW25vbkVudW1JZHhdO1xuICAgICAgaWYgKHByb3AgaW4gb2JqICYmIG9ialtwcm9wXSAhPT0gcHJvdG9bcHJvcF0gJiYgIV8uY29udGFpbnMoa2V5cywgcHJvcCkpIHtcbiAgICAgICAga2V5cy5wdXNoKHByb3ApO1xuICAgICAgfVxuICAgIH1cbiAgfVxuXG4gIC8vIFJldHJpZXZlIHRoZSBuYW1lcyBvZiBhbiBvYmplY3QncyBvd24gcHJvcGVydGllcy5cbiAgLy8gRGVsZWdhdGVzIHRvICoqRUNNQVNjcmlwdCA1KioncyBuYXRpdmUgYE9iamVjdC5rZXlzYFxuICBfLmtleXMgPSBmdW5jdGlvbihvYmopIHtcbiAgICBpZiAoIV8uaXNPYmplY3Qob2JqKSkgcmV0dXJuIFtdO1xuICAgIGlmIChuYXRpdmVLZXlzKSByZXR1cm4gbmF0aXZlS2V5cyhvYmopO1xuICAgIHZhciBrZXlzID0gW107XG4gICAgZm9yICh2YXIga2V5IGluIG9iaikgaWYgKF8uaGFzKG9iaiwga2V5KSkga2V5cy5wdXNoKGtleSk7XG4gICAgLy8gQWhlbSwgSUUgPCA5LlxuICAgIGlmIChoYXNFbnVtQnVnKSBjb2xsZWN0Tm9uRW51bVByb3BzKG9iaiwga2V5cyk7XG4gICAgcmV0dXJuIGtleXM7XG4gIH07XG5cbiAgLy8gUmV0cmlldmUgYWxsIHRoZSBwcm9wZXJ0eSBuYW1lcyBvZiBhbiBvYmplY3QuXG4gIF8uYWxsS2V5cyA9IGZ1bmN0aW9uKG9iaikge1xuICAgIGlmICghXy5pc09iamVjdChvYmopKSByZXR1cm4gW107XG4gICAgdmFyIGtleXMgPSBbXTtcbiAgICBmb3IgKHZhciBrZXkgaW4gb2JqKSBrZXlzLnB1c2goa2V5KTtcbiAgICAvLyBBaGVtLCBJRSA8IDkuXG4gICAgaWYgKGhhc0VudW1CdWcpIGNvbGxlY3ROb25FbnVtUHJvcHMob2JqLCBrZXlzKTtcbiAgICByZXR1cm4ga2V5cztcbiAgfTtcblxuICAvLyBSZXRyaWV2ZSB0aGUgdmFsdWVzIG9mIGFuIG9iamVjdCdzIHByb3BlcnRpZXMuXG4gIF8udmFsdWVzID0gZnVuY3Rpb24ob2JqKSB7XG4gICAgdmFyIGtleXMgPSBfLmtleXMob2JqKTtcbiAgICB2YXIgbGVuZ3RoID0ga2V5cy5sZW5ndGg7XG4gICAgdmFyIHZhbHVlcyA9IEFycmF5KGxlbmd0aCk7XG4gICAgZm9yICh2YXIgaSA9IDA7IGkgPCBsZW5ndGg7IGkrKykge1xuICAgICAgdmFsdWVzW2ldID0gb2JqW2tleXNbaV1dO1xuICAgIH1cbiAgICByZXR1cm4gdmFsdWVzO1xuICB9O1xuXG4gIC8vIFJldHVybnMgdGhlIHJlc3VsdHMgb2YgYXBwbHlpbmcgdGhlIGl0ZXJhdGVlIHRvIGVhY2ggZWxlbWVudCBvZiB0aGUgb2JqZWN0XG4gIC8vIEluIGNvbnRyYXN0IHRvIF8ubWFwIGl0IHJldHVybnMgYW4gb2JqZWN0XG4gIF8ubWFwT2JqZWN0ID0gZnVuY3Rpb24ob2JqLCBpdGVyYXRlZSwgY29udGV4dCkge1xuICAgIGl0ZXJhdGVlID0gY2IoaXRlcmF0ZWUsIGNvbnRleHQpO1xuICAgIHZhciBrZXlzID0gIF8ua2V5cyhvYmopLFxuICAgICAgICAgIGxlbmd0aCA9IGtleXMubGVuZ3RoLFxuICAgICAgICAgIHJlc3VsdHMgPSB7fSxcbiAgICAgICAgICBjdXJyZW50S2V5O1xuICAgICAgZm9yICh2YXIgaW5kZXggPSAwOyBpbmRleCA8IGxlbmd0aDsgaW5kZXgrKykge1xuICAgICAgICBjdXJyZW50S2V5ID0ga2V5c1tpbmRleF07XG4gICAgICAgIHJlc3VsdHNbY3VycmVudEtleV0gPSBpdGVyYXRlZShvYmpbY3VycmVudEtleV0sIGN1cnJlbnRLZXksIG9iaik7XG4gICAgICB9XG4gICAgICByZXR1cm4gcmVzdWx0cztcbiAgfTtcblxuICAvLyBDb252ZXJ0IGFuIG9iamVjdCBpbnRvIGEgbGlzdCBvZiBgW2tleSwgdmFsdWVdYCBwYWlycy5cbiAgXy5wYWlycyA9IGZ1bmN0aW9uKG9iaikge1xuICAgIHZhciBrZXlzID0gXy5rZXlzKG9iaik7XG4gICAgdmFyIGxlbmd0aCA9IGtleXMubGVuZ3RoO1xuICAgIHZhciBwYWlycyA9IEFycmF5KGxlbmd0aCk7XG4gICAgZm9yICh2YXIgaSA9IDA7IGkgPCBsZW5ndGg7IGkrKykge1xuICAgICAgcGFpcnNbaV0gPSBba2V5c1tpXSwgb2JqW2tleXNbaV1dXTtcbiAgICB9XG4gICAgcmV0dXJuIHBhaXJzO1xuICB9O1xuXG4gIC8vIEludmVydCB0aGUga2V5cyBhbmQgdmFsdWVzIG9mIGFuIG9iamVjdC4gVGhlIHZhbHVlcyBtdXN0IGJlIHNlcmlhbGl6YWJsZS5cbiAgXy5pbnZlcnQgPSBmdW5jdGlvbihvYmopIHtcbiAgICB2YXIgcmVzdWx0ID0ge307XG4gICAgdmFyIGtleXMgPSBfLmtleXMob2JqKTtcbiAgICBmb3IgKHZhciBpID0gMCwgbGVuZ3RoID0ga2V5cy5sZW5ndGg7IGkgPCBsZW5ndGg7IGkrKykge1xuICAgICAgcmVzdWx0W29ialtrZXlzW2ldXV0gPSBrZXlzW2ldO1xuICAgIH1cbiAgICByZXR1cm4gcmVzdWx0O1xuICB9O1xuXG4gIC8vIFJldHVybiBhIHNvcnRlZCBsaXN0IG9mIHRoZSBmdW5jdGlvbiBuYW1lcyBhdmFpbGFibGUgb24gdGhlIG9iamVjdC5cbiAgLy8gQWxpYXNlZCBhcyBgbWV0aG9kc2BcbiAgXy5mdW5jdGlvbnMgPSBfLm1ldGhvZHMgPSBmdW5jdGlvbihvYmopIHtcbiAgICB2YXIgbmFtZXMgPSBbXTtcbiAgICBmb3IgKHZhciBrZXkgaW4gb2JqKSB7XG4gICAgICBpZiAoXy5pc0Z1bmN0aW9uKG9ialtrZXldKSkgbmFtZXMucHVzaChrZXkpO1xuICAgIH1cbiAgICByZXR1cm4gbmFtZXMuc29ydCgpO1xuICB9O1xuXG4gIC8vIEV4dGVuZCBhIGdpdmVuIG9iamVjdCB3aXRoIGFsbCB0aGUgcHJvcGVydGllcyBpbiBwYXNzZWQtaW4gb2JqZWN0KHMpLlxuICBfLmV4dGVuZCA9IGNyZWF0ZUFzc2lnbmVyKF8uYWxsS2V5cyk7XG5cbiAgLy8gQXNzaWducyBhIGdpdmVuIG9iamVjdCB3aXRoIGFsbCB0aGUgb3duIHByb3BlcnRpZXMgaW4gdGhlIHBhc3NlZC1pbiBvYmplY3QocylcbiAgLy8gKGh0dHBzOi8vZGV2ZWxvcGVyLm1vemlsbGEub3JnL2RvY3MvV2ViL0phdmFTY3JpcHQvUmVmZXJlbmNlL0dsb2JhbF9PYmplY3RzL09iamVjdC9hc3NpZ24pXG4gIF8uZXh0ZW5kT3duID0gXy5hc3NpZ24gPSBjcmVhdGVBc3NpZ25lcihfLmtleXMpO1xuXG4gIC8vIFJldHVybnMgdGhlIGZpcnN0IGtleSBvbiBhbiBvYmplY3QgdGhhdCBwYXNzZXMgYSBwcmVkaWNhdGUgdGVzdFxuICBfLmZpbmRLZXkgPSBmdW5jdGlvbihvYmosIHByZWRpY2F0ZSwgY29udGV4dCkge1xuICAgIHByZWRpY2F0ZSA9IGNiKHByZWRpY2F0ZSwgY29udGV4dCk7XG4gICAgdmFyIGtleXMgPSBfLmtleXMob2JqKSwga2V5O1xuICAgIGZvciAodmFyIGkgPSAwLCBsZW5ndGggPSBrZXlzLmxlbmd0aDsgaSA8IGxlbmd0aDsgaSsrKSB7XG4gICAgICBrZXkgPSBrZXlzW2ldO1xuICAgICAgaWYgKHByZWRpY2F0ZShvYmpba2V5XSwga2V5LCBvYmopKSByZXR1cm4ga2V5O1xuICAgIH1cbiAgfTtcblxuICAvLyBSZXR1cm4gYSBjb3B5IG9mIHRoZSBvYmplY3Qgb25seSBjb250YWluaW5nIHRoZSB3aGl0ZWxpc3RlZCBwcm9wZXJ0aWVzLlxuICBfLnBpY2sgPSBmdW5jdGlvbihvYmplY3QsIG9pdGVyYXRlZSwgY29udGV4dCkge1xuICAgIHZhciByZXN1bHQgPSB7fSwgb2JqID0gb2JqZWN0LCBpdGVyYXRlZSwga2V5cztcbiAgICBpZiAob2JqID09IG51bGwpIHJldHVybiByZXN1bHQ7XG4gICAgaWYgKF8uaXNGdW5jdGlvbihvaXRlcmF0ZWUpKSB7XG4gICAgICBrZXlzID0gXy5hbGxLZXlzKG9iaik7XG4gICAgICBpdGVyYXRlZSA9IG9wdGltaXplQ2Iob2l0ZXJhdGVlLCBjb250ZXh0KTtcbiAgICB9IGVsc2Uge1xuICAgICAga2V5cyA9IGZsYXR0ZW4oYXJndW1lbnRzLCBmYWxzZSwgZmFsc2UsIDEpO1xuICAgICAgaXRlcmF0ZWUgPSBmdW5jdGlvbih2YWx1ZSwga2V5LCBvYmopIHsgcmV0dXJuIGtleSBpbiBvYmo7IH07XG4gICAgICBvYmogPSBPYmplY3Qob2JqKTtcbiAgICB9XG4gICAgZm9yICh2YXIgaSA9IDAsIGxlbmd0aCA9IGtleXMubGVuZ3RoOyBpIDwgbGVuZ3RoOyBpKyspIHtcbiAgICAgIHZhciBrZXkgPSBrZXlzW2ldO1xuICAgICAgdmFyIHZhbHVlID0gb2JqW2tleV07XG4gICAgICBpZiAoaXRlcmF0ZWUodmFsdWUsIGtleSwgb2JqKSkgcmVzdWx0W2tleV0gPSB2YWx1ZTtcbiAgICB9XG4gICAgcmV0dXJuIHJlc3VsdDtcbiAgfTtcblxuICAgLy8gUmV0dXJuIGEgY29weSBvZiB0aGUgb2JqZWN0IHdpdGhvdXQgdGhlIGJsYWNrbGlzdGVkIHByb3BlcnRpZXMuXG4gIF8ub21pdCA9IGZ1bmN0aW9uKG9iaiwgaXRlcmF0ZWUsIGNvbnRleHQpIHtcbiAgICBpZiAoXy5pc0Z1bmN0aW9uKGl0ZXJhdGVlKSkge1xuICAgICAgaXRlcmF0ZWUgPSBfLm5lZ2F0ZShpdGVyYXRlZSk7XG4gICAgfSBlbHNlIHtcbiAgICAgIHZhciBrZXlzID0gXy5tYXAoZmxhdHRlbihhcmd1bWVudHMsIGZhbHNlLCBmYWxzZSwgMSksIFN0cmluZyk7XG4gICAgICBpdGVyYXRlZSA9IGZ1bmN0aW9uKHZhbHVlLCBrZXkpIHtcbiAgICAgICAgcmV0dXJuICFfLmNvbnRhaW5zKGtleXMsIGtleSk7XG4gICAgICB9O1xuICAgIH1cbiAgICByZXR1cm4gXy5waWNrKG9iaiwgaXRlcmF0ZWUsIGNvbnRleHQpO1xuICB9O1xuXG4gIC8vIEZpbGwgaW4gYSBnaXZlbiBvYmplY3Qgd2l0aCBkZWZhdWx0IHByb3BlcnRpZXMuXG4gIF8uZGVmYXVsdHMgPSBjcmVhdGVBc3NpZ25lcihfLmFsbEtleXMsIHRydWUpO1xuXG4gIC8vIENyZWF0ZXMgYW4gb2JqZWN0IHRoYXQgaW5oZXJpdHMgZnJvbSB0aGUgZ2l2ZW4gcHJvdG90eXBlIG9iamVjdC5cbiAgLy8gSWYgYWRkaXRpb25hbCBwcm9wZXJ0aWVzIGFyZSBwcm92aWRlZCB0aGVuIHRoZXkgd2lsbCBiZSBhZGRlZCB0byB0aGVcbiAgLy8gY3JlYXRlZCBvYmplY3QuXG4gIF8uY3JlYXRlID0gZnVuY3Rpb24ocHJvdG90eXBlLCBwcm9wcykge1xuICAgIHZhciByZXN1bHQgPSBiYXNlQ3JlYXRlKHByb3RvdHlwZSk7XG4gICAgaWYgKHByb3BzKSBfLmV4dGVuZE93bihyZXN1bHQsIHByb3BzKTtcbiAgICByZXR1cm4gcmVzdWx0O1xuICB9O1xuXG4gIC8vIENyZWF0ZSBhIChzaGFsbG93LWNsb25lZCkgZHVwbGljYXRlIG9mIGFuIG9iamVjdC5cbiAgXy5jbG9uZSA9IGZ1bmN0aW9uKG9iaikge1xuICAgIGlmICghXy5pc09iamVjdChvYmopKSByZXR1cm4gb2JqO1xuICAgIHJldHVybiBfLmlzQXJyYXkob2JqKSA/IG9iai5zbGljZSgpIDogXy5leHRlbmQoe30sIG9iaik7XG4gIH07XG5cbiAgLy8gSW52b2tlcyBpbnRlcmNlcHRvciB3aXRoIHRoZSBvYmosIGFuZCB0aGVuIHJldHVybnMgb2JqLlxuICAvLyBUaGUgcHJpbWFyeSBwdXJwb3NlIG9mIHRoaXMgbWV0aG9kIGlzIHRvIFwidGFwIGludG9cIiBhIG1ldGhvZCBjaGFpbiwgaW5cbiAgLy8gb3JkZXIgdG8gcGVyZm9ybSBvcGVyYXRpb25zIG9uIGludGVybWVkaWF0ZSByZXN1bHRzIHdpdGhpbiB0aGUgY2hhaW4uXG4gIF8udGFwID0gZnVuY3Rpb24ob2JqLCBpbnRlcmNlcHRvcikge1xuICAgIGludGVyY2VwdG9yKG9iaik7XG4gICAgcmV0dXJuIG9iajtcbiAgfTtcblxuICAvLyBSZXR1cm5zIHdoZXRoZXIgYW4gb2JqZWN0IGhhcyBhIGdpdmVuIHNldCBvZiBga2V5OnZhbHVlYCBwYWlycy5cbiAgXy5pc01hdGNoID0gZnVuY3Rpb24ob2JqZWN0LCBhdHRycykge1xuICAgIHZhciBrZXlzID0gXy5rZXlzKGF0dHJzKSwgbGVuZ3RoID0ga2V5cy5sZW5ndGg7XG4gICAgaWYgKG9iamVjdCA9PSBudWxsKSByZXR1cm4gIWxlbmd0aDtcbiAgICB2YXIgb2JqID0gT2JqZWN0KG9iamVjdCk7XG4gICAgZm9yICh2YXIgaSA9IDA7IGkgPCBsZW5ndGg7IGkrKykge1xuICAgICAgdmFyIGtleSA9IGtleXNbaV07XG4gICAgICBpZiAoYXR0cnNba2V5XSAhPT0gb2JqW2tleV0gfHwgIShrZXkgaW4gb2JqKSkgcmV0dXJuIGZhbHNlO1xuICAgIH1cbiAgICByZXR1cm4gdHJ1ZTtcbiAgfTtcblxuXG4gIC8vIEludGVybmFsIHJlY3Vyc2l2ZSBjb21wYXJpc29uIGZ1bmN0aW9uIGZvciBgaXNFcXVhbGAuXG4gIHZhciBlcSA9IGZ1bmN0aW9uKGEsIGIsIGFTdGFjaywgYlN0YWNrKSB7XG4gICAgLy8gSWRlbnRpY2FsIG9iamVjdHMgYXJlIGVxdWFsLiBgMCA9PT0gLTBgLCBidXQgdGhleSBhcmVuJ3QgaWRlbnRpY2FsLlxuICAgIC8vIFNlZSB0aGUgW0hhcm1vbnkgYGVnYWxgIHByb3Bvc2FsXShodHRwOi8vd2lraS5lY21hc2NyaXB0Lm9yZy9kb2t1LnBocD9pZD1oYXJtb255OmVnYWwpLlxuICAgIGlmIChhID09PSBiKSByZXR1cm4gYSAhPT0gMCB8fCAxIC8gYSA9PT0gMSAvIGI7XG4gICAgLy8gQSBzdHJpY3QgY29tcGFyaXNvbiBpcyBuZWNlc3NhcnkgYmVjYXVzZSBgbnVsbCA9PSB1bmRlZmluZWRgLlxuICAgIGlmIChhID09IG51bGwgfHwgYiA9PSBudWxsKSByZXR1cm4gYSA9PT0gYjtcbiAgICAvLyBVbndyYXAgYW55IHdyYXBwZWQgb2JqZWN0cy5cbiAgICBpZiAoYSBpbnN0YW5jZW9mIF8pIGEgPSBhLl93cmFwcGVkO1xuICAgIGlmIChiIGluc3RhbmNlb2YgXykgYiA9IGIuX3dyYXBwZWQ7XG4gICAgLy8gQ29tcGFyZSBgW1tDbGFzc11dYCBuYW1lcy5cbiAgICB2YXIgY2xhc3NOYW1lID0gdG9TdHJpbmcuY2FsbChhKTtcbiAgICBpZiAoY2xhc3NOYW1lICE9PSB0b1N0cmluZy5jYWxsKGIpKSByZXR1cm4gZmFsc2U7XG4gICAgc3dpdGNoIChjbGFzc05hbWUpIHtcbiAgICAgIC8vIFN0cmluZ3MsIG51bWJlcnMsIHJlZ3VsYXIgZXhwcmVzc2lvbnMsIGRhdGVzLCBhbmQgYm9vbGVhbnMgYXJlIGNvbXBhcmVkIGJ5IHZhbHVlLlxuICAgICAgY2FzZSAnW29iamVjdCBSZWdFeHBdJzpcbiAgICAgIC8vIFJlZ0V4cHMgYXJlIGNvZXJjZWQgdG8gc3RyaW5ncyBmb3IgY29tcGFyaXNvbiAoTm90ZTogJycgKyAvYS9pID09PSAnL2EvaScpXG4gICAgICBjYXNlICdbb2JqZWN0IFN0cmluZ10nOlxuICAgICAgICAvLyBQcmltaXRpdmVzIGFuZCB0aGVpciBjb3JyZXNwb25kaW5nIG9iamVjdCB3cmFwcGVycyBhcmUgZXF1aXZhbGVudDsgdGh1cywgYFwiNVwiYCBpc1xuICAgICAgICAvLyBlcXVpdmFsZW50IHRvIGBuZXcgU3RyaW5nKFwiNVwiKWAuXG4gICAgICAgIHJldHVybiAnJyArIGEgPT09ICcnICsgYjtcbiAgICAgIGNhc2UgJ1tvYmplY3QgTnVtYmVyXSc6XG4gICAgICAgIC8vIGBOYU5gcyBhcmUgZXF1aXZhbGVudCwgYnV0IG5vbi1yZWZsZXhpdmUuXG4gICAgICAgIC8vIE9iamVjdChOYU4pIGlzIGVxdWl2YWxlbnQgdG8gTmFOXG4gICAgICAgIGlmICgrYSAhPT0gK2EpIHJldHVybiArYiAhPT0gK2I7XG4gICAgICAgIC8vIEFuIGBlZ2FsYCBjb21wYXJpc29uIGlzIHBlcmZvcm1lZCBmb3Igb3RoZXIgbnVtZXJpYyB2YWx1ZXMuXG4gICAgICAgIHJldHVybiArYSA9PT0gMCA/IDEgLyArYSA9PT0gMSAvIGIgOiArYSA9PT0gK2I7XG4gICAgICBjYXNlICdbb2JqZWN0IERhdGVdJzpcbiAgICAgIGNhc2UgJ1tvYmplY3QgQm9vbGVhbl0nOlxuICAgICAgICAvLyBDb2VyY2UgZGF0ZXMgYW5kIGJvb2xlYW5zIHRvIG51bWVyaWMgcHJpbWl0aXZlIHZhbHVlcy4gRGF0ZXMgYXJlIGNvbXBhcmVkIGJ5IHRoZWlyXG4gICAgICAgIC8vIG1pbGxpc2Vjb25kIHJlcHJlc2VudGF0aW9ucy4gTm90ZSB0aGF0IGludmFsaWQgZGF0ZXMgd2l0aCBtaWxsaXNlY29uZCByZXByZXNlbnRhdGlvbnNcbiAgICAgICAgLy8gb2YgYE5hTmAgYXJlIG5vdCBlcXVpdmFsZW50LlxuICAgICAgICByZXR1cm4gK2EgPT09ICtiO1xuICAgIH1cblxuICAgIHZhciBhcmVBcnJheXMgPSBjbGFzc05hbWUgPT09ICdbb2JqZWN0IEFycmF5XSc7XG4gICAgaWYgKCFhcmVBcnJheXMpIHtcbiAgICAgIGlmICh0eXBlb2YgYSAhPSAnb2JqZWN0JyB8fCB0eXBlb2YgYiAhPSAnb2JqZWN0JykgcmV0dXJuIGZhbHNlO1xuXG4gICAgICAvLyBPYmplY3RzIHdpdGggZGlmZmVyZW50IGNvbnN0cnVjdG9ycyBhcmUgbm90IGVxdWl2YWxlbnQsIGJ1dCBgT2JqZWN0YHMgb3IgYEFycmF5YHNcbiAgICAgIC8vIGZyb20gZGlmZmVyZW50IGZyYW1lcyBhcmUuXG4gICAgICB2YXIgYUN0b3IgPSBhLmNvbnN0cnVjdG9yLCBiQ3RvciA9IGIuY29uc3RydWN0b3I7XG4gICAgICBpZiAoYUN0b3IgIT09IGJDdG9yICYmICEoXy5pc0Z1bmN0aW9uKGFDdG9yKSAmJiBhQ3RvciBpbnN0YW5jZW9mIGFDdG9yICYmXG4gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgXy5pc0Z1bmN0aW9uKGJDdG9yKSAmJiBiQ3RvciBpbnN0YW5jZW9mIGJDdG9yKVxuICAgICAgICAgICAgICAgICAgICAgICAgICAmJiAoJ2NvbnN0cnVjdG9yJyBpbiBhICYmICdjb25zdHJ1Y3RvcicgaW4gYikpIHtcbiAgICAgICAgcmV0dXJuIGZhbHNlO1xuICAgICAgfVxuICAgIH1cbiAgICAvLyBBc3N1bWUgZXF1YWxpdHkgZm9yIGN5Y2xpYyBzdHJ1Y3R1cmVzLiBUaGUgYWxnb3JpdGhtIGZvciBkZXRlY3RpbmcgY3ljbGljXG4gICAgLy8gc3RydWN0dXJlcyBpcyBhZGFwdGVkIGZyb20gRVMgNS4xIHNlY3Rpb24gMTUuMTIuMywgYWJzdHJhY3Qgb3BlcmF0aW9uIGBKT2AuXG5cbiAgICAvLyBJbml0aWFsaXppbmcgc3RhY2sgb2YgdHJhdmVyc2VkIG9iamVjdHMuXG4gICAgLy8gSXQncyBkb25lIGhlcmUgc2luY2Ugd2Ugb25seSBuZWVkIHRoZW0gZm9yIG9iamVjdHMgYW5kIGFycmF5cyBjb21wYXJpc29uLlxuICAgIGFTdGFjayA9IGFTdGFjayB8fCBbXTtcbiAgICBiU3RhY2sgPSBiU3RhY2sgfHwgW107XG4gICAgdmFyIGxlbmd0aCA9IGFTdGFjay5sZW5ndGg7XG4gICAgd2hpbGUgKGxlbmd0aC0tKSB7XG4gICAgICAvLyBMaW5lYXIgc2VhcmNoLiBQZXJmb3JtYW5jZSBpcyBpbnZlcnNlbHkgcHJvcG9ydGlvbmFsIHRvIHRoZSBudW1iZXIgb2ZcbiAgICAgIC8vIHVuaXF1ZSBuZXN0ZWQgc3RydWN0dXJlcy5cbiAgICAgIGlmIChhU3RhY2tbbGVuZ3RoXSA9PT0gYSkgcmV0dXJuIGJTdGFja1tsZW5ndGhdID09PSBiO1xuICAgIH1cblxuICAgIC8vIEFkZCB0aGUgZmlyc3Qgb2JqZWN0IHRvIHRoZSBzdGFjayBvZiB0cmF2ZXJzZWQgb2JqZWN0cy5cbiAgICBhU3RhY2sucHVzaChhKTtcbiAgICBiU3RhY2sucHVzaChiKTtcblxuICAgIC8vIFJlY3Vyc2l2ZWx5IGNvbXBhcmUgb2JqZWN0cyBhbmQgYXJyYXlzLlxuICAgIGlmIChhcmVBcnJheXMpIHtcbiAgICAgIC8vIENvbXBhcmUgYXJyYXkgbGVuZ3RocyB0byBkZXRlcm1pbmUgaWYgYSBkZWVwIGNvbXBhcmlzb24gaXMgbmVjZXNzYXJ5LlxuICAgICAgbGVuZ3RoID0gYS5sZW5ndGg7XG4gICAgICBpZiAobGVuZ3RoICE9PSBiLmxlbmd0aCkgcmV0dXJuIGZhbHNlO1xuICAgICAgLy8gRGVlcCBjb21wYXJlIHRoZSBjb250ZW50cywgaWdub3Jpbmcgbm9uLW51bWVyaWMgcHJvcGVydGllcy5cbiAgICAgIHdoaWxlIChsZW5ndGgtLSkge1xuICAgICAgICBpZiAoIWVxKGFbbGVuZ3RoXSwgYltsZW5ndGhdLCBhU3RhY2ssIGJTdGFjaykpIHJldHVybiBmYWxzZTtcbiAgICAgIH1cbiAgICB9IGVsc2Uge1xuICAgICAgLy8gRGVlcCBjb21wYXJlIG9iamVjdHMuXG4gICAgICB2YXIga2V5cyA9IF8ua2V5cyhhKSwga2V5O1xuICAgICAgbGVuZ3RoID0ga2V5cy5sZW5ndGg7XG4gICAgICAvLyBFbnN1cmUgdGhhdCBib3RoIG9iamVjdHMgY29udGFpbiB0aGUgc2FtZSBudW1iZXIgb2YgcHJvcGVydGllcyBiZWZvcmUgY29tcGFyaW5nIGRlZXAgZXF1YWxpdHkuXG4gICAgICBpZiAoXy5rZXlzKGIpLmxlbmd0aCAhPT0gbGVuZ3RoKSByZXR1cm4gZmFsc2U7XG4gICAgICB3aGlsZSAobGVuZ3RoLS0pIHtcbiAgICAgICAgLy8gRGVlcCBjb21wYXJlIGVhY2ggbWVtYmVyXG4gICAgICAgIGtleSA9IGtleXNbbGVuZ3RoXTtcbiAgICAgICAgaWYgKCEoXy5oYXMoYiwga2V5KSAmJiBlcShhW2tleV0sIGJba2V5XSwgYVN0YWNrLCBiU3RhY2spKSkgcmV0dXJuIGZhbHNlO1xuICAgICAgfVxuICAgIH1cbiAgICAvLyBSZW1vdmUgdGhlIGZpcnN0IG9iamVjdCBmcm9tIHRoZSBzdGFjayBvZiB0cmF2ZXJzZWQgb2JqZWN0cy5cbiAgICBhU3RhY2sucG9wKCk7XG4gICAgYlN0YWNrLnBvcCgpO1xuICAgIHJldHVybiB0cnVlO1xuICB9O1xuXG4gIC8vIFBlcmZvcm0gYSBkZWVwIGNvbXBhcmlzb24gdG8gY2hlY2sgaWYgdHdvIG9iamVjdHMgYXJlIGVxdWFsLlxuICBfLmlzRXF1YWwgPSBmdW5jdGlvbihhLCBiKSB7XG4gICAgcmV0dXJuIGVxKGEsIGIpO1xuICB9O1xuXG4gIC8vIElzIGEgZ2l2ZW4gYXJyYXksIHN0cmluZywgb3Igb2JqZWN0IGVtcHR5P1xuICAvLyBBbiBcImVtcHR5XCIgb2JqZWN0IGhhcyBubyBlbnVtZXJhYmxlIG93bi1wcm9wZXJ0aWVzLlxuICBfLmlzRW1wdHkgPSBmdW5jdGlvbihvYmopIHtcbiAgICBpZiAob2JqID09IG51bGwpIHJldHVybiB0cnVlO1xuICAgIGlmIChpc0FycmF5TGlrZShvYmopICYmIChfLmlzQXJyYXkob2JqKSB8fCBfLmlzU3RyaW5nKG9iaikgfHwgXy5pc0FyZ3VtZW50cyhvYmopKSkgcmV0dXJuIG9iai5sZW5ndGggPT09IDA7XG4gICAgcmV0dXJuIF8ua2V5cyhvYmopLmxlbmd0aCA9PT0gMDtcbiAgfTtcblxuICAvLyBJcyBhIGdpdmVuIHZhbHVlIGEgRE9NIGVsZW1lbnQ/XG4gIF8uaXNFbGVtZW50ID0gZnVuY3Rpb24ob2JqKSB7XG4gICAgcmV0dXJuICEhKG9iaiAmJiBvYmoubm9kZVR5cGUgPT09IDEpO1xuICB9O1xuXG4gIC8vIElzIGEgZ2l2ZW4gdmFsdWUgYW4gYXJyYXk/XG4gIC8vIERlbGVnYXRlcyB0byBFQ01BNSdzIG5hdGl2ZSBBcnJheS5pc0FycmF5XG4gIF8uaXNBcnJheSA9IG5hdGl2ZUlzQXJyYXkgfHwgZnVuY3Rpb24ob2JqKSB7XG4gICAgcmV0dXJuIHRvU3RyaW5nLmNhbGwob2JqKSA9PT0gJ1tvYmplY3QgQXJyYXldJztcbiAgfTtcblxuICAvLyBJcyBhIGdpdmVuIHZhcmlhYmxlIGFuIG9iamVjdD9cbiAgXy5pc09iamVjdCA9IGZ1bmN0aW9uKG9iaikge1xuICAgIHZhciB0eXBlID0gdHlwZW9mIG9iajtcbiAgICByZXR1cm4gdHlwZSA9PT0gJ2Z1bmN0aW9uJyB8fCB0eXBlID09PSAnb2JqZWN0JyAmJiAhIW9iajtcbiAgfTtcblxuICAvLyBBZGQgc29tZSBpc1R5cGUgbWV0aG9kczogaXNBcmd1bWVudHMsIGlzRnVuY3Rpb24sIGlzU3RyaW5nLCBpc051bWJlciwgaXNEYXRlLCBpc1JlZ0V4cCwgaXNFcnJvci5cbiAgXy5lYWNoKFsnQXJndW1lbnRzJywgJ0Z1bmN0aW9uJywgJ1N0cmluZycsICdOdW1iZXInLCAnRGF0ZScsICdSZWdFeHAnLCAnRXJyb3InXSwgZnVuY3Rpb24obmFtZSkge1xuICAgIF9bJ2lzJyArIG5hbWVdID0gZnVuY3Rpb24ob2JqKSB7XG4gICAgICByZXR1cm4gdG9TdHJpbmcuY2FsbChvYmopID09PSAnW29iamVjdCAnICsgbmFtZSArICddJztcbiAgICB9O1xuICB9KTtcblxuICAvLyBEZWZpbmUgYSBmYWxsYmFjayB2ZXJzaW9uIG9mIHRoZSBtZXRob2QgaW4gYnJvd3NlcnMgKGFoZW0sIElFIDwgOSksIHdoZXJlXG4gIC8vIHRoZXJlIGlzbid0IGFueSBpbnNwZWN0YWJsZSBcIkFyZ3VtZW50c1wiIHR5cGUuXG4gIGlmICghXy5pc0FyZ3VtZW50cyhhcmd1bWVudHMpKSB7XG4gICAgXy5pc0FyZ3VtZW50cyA9IGZ1bmN0aW9uKG9iaikge1xuICAgICAgcmV0dXJuIF8uaGFzKG9iaiwgJ2NhbGxlZScpO1xuICAgIH07XG4gIH1cblxuICAvLyBPcHRpbWl6ZSBgaXNGdW5jdGlvbmAgaWYgYXBwcm9wcmlhdGUuIFdvcmsgYXJvdW5kIHNvbWUgdHlwZW9mIGJ1Z3MgaW4gb2xkIHY4LFxuICAvLyBJRSAxMSAoIzE2MjEpLCBhbmQgaW4gU2FmYXJpIDggKCMxOTI5KS5cbiAgaWYgKHR5cGVvZiAvLi8gIT0gJ2Z1bmN0aW9uJyAmJiB0eXBlb2YgSW50OEFycmF5ICE9ICdvYmplY3QnKSB7XG4gICAgXy5pc0Z1bmN0aW9uID0gZnVuY3Rpb24ob2JqKSB7XG4gICAgICByZXR1cm4gdHlwZW9mIG9iaiA9PSAnZnVuY3Rpb24nIHx8IGZhbHNlO1xuICAgIH07XG4gIH1cblxuICAvLyBJcyBhIGdpdmVuIG9iamVjdCBhIGZpbml0ZSBudW1iZXI/XG4gIF8uaXNGaW5pdGUgPSBmdW5jdGlvbihvYmopIHtcbiAgICByZXR1cm4gaXNGaW5pdGUob2JqKSAmJiAhaXNOYU4ocGFyc2VGbG9hdChvYmopKTtcbiAgfTtcblxuICAvLyBJcyB0aGUgZ2l2ZW4gdmFsdWUgYE5hTmA/IChOYU4gaXMgdGhlIG9ubHkgbnVtYmVyIHdoaWNoIGRvZXMgbm90IGVxdWFsIGl0c2VsZikuXG4gIF8uaXNOYU4gPSBmdW5jdGlvbihvYmopIHtcbiAgICByZXR1cm4gXy5pc051bWJlcihvYmopICYmIG9iaiAhPT0gK29iajtcbiAgfTtcblxuICAvLyBJcyBhIGdpdmVuIHZhbHVlIGEgYm9vbGVhbj9cbiAgXy5pc0Jvb2xlYW4gPSBmdW5jdGlvbihvYmopIHtcbiAgICByZXR1cm4gb2JqID09PSB0cnVlIHx8IG9iaiA9PT0gZmFsc2UgfHwgdG9TdHJpbmcuY2FsbChvYmopID09PSAnW29iamVjdCBCb29sZWFuXSc7XG4gIH07XG5cbiAgLy8gSXMgYSBnaXZlbiB2YWx1ZSBlcXVhbCB0byBudWxsP1xuICBfLmlzTnVsbCA9IGZ1bmN0aW9uKG9iaikge1xuICAgIHJldHVybiBvYmogPT09IG51bGw7XG4gIH07XG5cbiAgLy8gSXMgYSBnaXZlbiB2YXJpYWJsZSB1bmRlZmluZWQ/XG4gIF8uaXNVbmRlZmluZWQgPSBmdW5jdGlvbihvYmopIHtcbiAgICByZXR1cm4gb2JqID09PSB2b2lkIDA7XG4gIH07XG5cbiAgLy8gU2hvcnRjdXQgZnVuY3Rpb24gZm9yIGNoZWNraW5nIGlmIGFuIG9iamVjdCBoYXMgYSBnaXZlbiBwcm9wZXJ0eSBkaXJlY3RseVxuICAvLyBvbiBpdHNlbGYgKGluIG90aGVyIHdvcmRzLCBub3Qgb24gYSBwcm90b3R5cGUpLlxuICBfLmhhcyA9IGZ1bmN0aW9uKG9iaiwga2V5KSB7XG4gICAgcmV0dXJuIG9iaiAhPSBudWxsICYmIGhhc093blByb3BlcnR5LmNhbGwob2JqLCBrZXkpO1xuICB9O1xuXG4gIC8vIFV0aWxpdHkgRnVuY3Rpb25zXG4gIC8vIC0tLS0tLS0tLS0tLS0tLS0tXG5cbiAgLy8gUnVuIFVuZGVyc2NvcmUuanMgaW4gKm5vQ29uZmxpY3QqIG1vZGUsIHJldHVybmluZyB0aGUgYF9gIHZhcmlhYmxlIHRvIGl0c1xuICAvLyBwcmV2aW91cyBvd25lci4gUmV0dXJucyBhIHJlZmVyZW5jZSB0byB0aGUgVW5kZXJzY29yZSBvYmplY3QuXG4gIF8ubm9Db25mbGljdCA9IGZ1bmN0aW9uKCkge1xuICAgIHJvb3QuXyA9IHByZXZpb3VzVW5kZXJzY29yZTtcbiAgICByZXR1cm4gdGhpcztcbiAgfTtcblxuICAvLyBLZWVwIHRoZSBpZGVudGl0eSBmdW5jdGlvbiBhcm91bmQgZm9yIGRlZmF1bHQgaXRlcmF0ZWVzLlxuICBfLmlkZW50aXR5ID0gZnVuY3Rpb24odmFsdWUpIHtcbiAgICByZXR1cm4gdmFsdWU7XG4gIH07XG5cbiAgLy8gUHJlZGljYXRlLWdlbmVyYXRpbmcgZnVuY3Rpb25zLiBPZnRlbiB1c2VmdWwgb3V0c2lkZSBvZiBVbmRlcnNjb3JlLlxuICBfLmNvbnN0YW50ID0gZnVuY3Rpb24odmFsdWUpIHtcbiAgICByZXR1cm4gZnVuY3Rpb24oKSB7XG4gICAgICByZXR1cm4gdmFsdWU7XG4gICAgfTtcbiAgfTtcblxuICBfLm5vb3AgPSBmdW5jdGlvbigpe307XG5cbiAgXy5wcm9wZXJ0eSA9IHByb3BlcnR5O1xuXG4gIC8vIEdlbmVyYXRlcyBhIGZ1bmN0aW9uIGZvciBhIGdpdmVuIG9iamVjdCB0aGF0IHJldHVybnMgYSBnaXZlbiBwcm9wZXJ0eS5cbiAgXy5wcm9wZXJ0eU9mID0gZnVuY3Rpb24ob2JqKSB7XG4gICAgcmV0dXJuIG9iaiA9PSBudWxsID8gZnVuY3Rpb24oKXt9IDogZnVuY3Rpb24oa2V5KSB7XG4gICAgICByZXR1cm4gb2JqW2tleV07XG4gICAgfTtcbiAgfTtcblxuICAvLyBSZXR1cm5zIGEgcHJlZGljYXRlIGZvciBjaGVja2luZyB3aGV0aGVyIGFuIG9iamVjdCBoYXMgYSBnaXZlbiBzZXQgb2ZcbiAgLy8gYGtleTp2YWx1ZWAgcGFpcnMuXG4gIF8ubWF0Y2hlciA9IF8ubWF0Y2hlcyA9IGZ1bmN0aW9uKGF0dHJzKSB7XG4gICAgYXR0cnMgPSBfLmV4dGVuZE93bih7fSwgYXR0cnMpO1xuICAgIHJldHVybiBmdW5jdGlvbihvYmopIHtcbiAgICAgIHJldHVybiBfLmlzTWF0Y2gob2JqLCBhdHRycyk7XG4gICAgfTtcbiAgfTtcblxuICAvLyBSdW4gYSBmdW5jdGlvbiAqKm4qKiB0aW1lcy5cbiAgXy50aW1lcyA9IGZ1bmN0aW9uKG4sIGl0ZXJhdGVlLCBjb250ZXh0KSB7XG4gICAgdmFyIGFjY3VtID0gQXJyYXkoTWF0aC5tYXgoMCwgbikpO1xuICAgIGl0ZXJhdGVlID0gb3B0aW1pemVDYihpdGVyYXRlZSwgY29udGV4dCwgMSk7XG4gICAgZm9yICh2YXIgaSA9IDA7IGkgPCBuOyBpKyspIGFjY3VtW2ldID0gaXRlcmF0ZWUoaSk7XG4gICAgcmV0dXJuIGFjY3VtO1xuICB9O1xuXG4gIC8vIFJldHVybiBhIHJhbmRvbSBpbnRlZ2VyIGJldHdlZW4gbWluIGFuZCBtYXggKGluY2x1c2l2ZSkuXG4gIF8ucmFuZG9tID0gZnVuY3Rpb24obWluLCBtYXgpIHtcbiAgICBpZiAobWF4ID09IG51bGwpIHtcbiAgICAgIG1heCA9IG1pbjtcbiAgICAgIG1pbiA9IDA7XG4gICAgfVxuICAgIHJldHVybiBtaW4gKyBNYXRoLmZsb29yKE1hdGgucmFuZG9tKCkgKiAobWF4IC0gbWluICsgMSkpO1xuICB9O1xuXG4gIC8vIEEgKHBvc3NpYmx5IGZhc3Rlcikgd2F5IHRvIGdldCB0aGUgY3VycmVudCB0aW1lc3RhbXAgYXMgYW4gaW50ZWdlci5cbiAgXy5ub3cgPSBEYXRlLm5vdyB8fCBmdW5jdGlvbigpIHtcbiAgICByZXR1cm4gbmV3IERhdGUoKS5nZXRUaW1lKCk7XG4gIH07XG5cbiAgIC8vIExpc3Qgb2YgSFRNTCBlbnRpdGllcyBmb3IgZXNjYXBpbmcuXG4gIHZhciBlc2NhcGVNYXAgPSB7XG4gICAgJyYnOiAnJmFtcDsnLFxuICAgICc8JzogJyZsdDsnLFxuICAgICc+JzogJyZndDsnLFxuICAgICdcIic6ICcmcXVvdDsnLFxuICAgIFwiJ1wiOiAnJiN4Mjc7JyxcbiAgICAnYCc6ICcmI3g2MDsnXG4gIH07XG4gIHZhciB1bmVzY2FwZU1hcCA9IF8uaW52ZXJ0KGVzY2FwZU1hcCk7XG5cbiAgLy8gRnVuY3Rpb25zIGZvciBlc2NhcGluZyBhbmQgdW5lc2NhcGluZyBzdHJpbmdzIHRvL2Zyb20gSFRNTCBpbnRlcnBvbGF0aW9uLlxuICB2YXIgY3JlYXRlRXNjYXBlciA9IGZ1bmN0aW9uKG1hcCkge1xuICAgIHZhciBlc2NhcGVyID0gZnVuY3Rpb24obWF0Y2gpIHtcbiAgICAgIHJldHVybiBtYXBbbWF0Y2hdO1xuICAgIH07XG4gICAgLy8gUmVnZXhlcyBmb3IgaWRlbnRpZnlpbmcgYSBrZXkgdGhhdCBuZWVkcyB0byBiZSBlc2NhcGVkXG4gICAgdmFyIHNvdXJjZSA9ICcoPzonICsgXy5rZXlzKG1hcCkuam9pbignfCcpICsgJyknO1xuICAgIHZhciB0ZXN0UmVnZXhwID0gUmVnRXhwKHNvdXJjZSk7XG4gICAgdmFyIHJlcGxhY2VSZWdleHAgPSBSZWdFeHAoc291cmNlLCAnZycpO1xuICAgIHJldHVybiBmdW5jdGlvbihzdHJpbmcpIHtcbiAgICAgIHN0cmluZyA9IHN0cmluZyA9PSBudWxsID8gJycgOiAnJyArIHN0cmluZztcbiAgICAgIHJldHVybiB0ZXN0UmVnZXhwLnRlc3Qoc3RyaW5nKSA/IHN0cmluZy5yZXBsYWNlKHJlcGxhY2VSZWdleHAsIGVzY2FwZXIpIDogc3RyaW5nO1xuICAgIH07XG4gIH07XG4gIF8uZXNjYXBlID0gY3JlYXRlRXNjYXBlcihlc2NhcGVNYXApO1xuICBfLnVuZXNjYXBlID0gY3JlYXRlRXNjYXBlcih1bmVzY2FwZU1hcCk7XG5cbiAgLy8gSWYgdGhlIHZhbHVlIG9mIHRoZSBuYW1lZCBgcHJvcGVydHlgIGlzIGEgZnVuY3Rpb24gdGhlbiBpbnZva2UgaXQgd2l0aCB0aGVcbiAgLy8gYG9iamVjdGAgYXMgY29udGV4dDsgb3RoZXJ3aXNlLCByZXR1cm4gaXQuXG4gIF8ucmVzdWx0ID0gZnVuY3Rpb24ob2JqZWN0LCBwcm9wZXJ0eSwgZmFsbGJhY2spIHtcbiAgICB2YXIgdmFsdWUgPSBvYmplY3QgPT0gbnVsbCA/IHZvaWQgMCA6IG9iamVjdFtwcm9wZXJ0eV07XG4gICAgaWYgKHZhbHVlID09PSB2b2lkIDApIHtcbiAgICAgIHZhbHVlID0gZmFsbGJhY2s7XG4gICAgfVxuICAgIHJldHVybiBfLmlzRnVuY3Rpb24odmFsdWUpID8gdmFsdWUuY2FsbChvYmplY3QpIDogdmFsdWU7XG4gIH07XG5cbiAgLy8gR2VuZXJhdGUgYSB1bmlxdWUgaW50ZWdlciBpZCAodW5pcXVlIHdpdGhpbiB0aGUgZW50aXJlIGNsaWVudCBzZXNzaW9uKS5cbiAgLy8gVXNlZnVsIGZvciB0ZW1wb3JhcnkgRE9NIGlkcy5cbiAgdmFyIGlkQ291bnRlciA9IDA7XG4gIF8udW5pcXVlSWQgPSBmdW5jdGlvbihwcmVmaXgpIHtcbiAgICB2YXIgaWQgPSArK2lkQ291bnRlciArICcnO1xuICAgIHJldHVybiBwcmVmaXggPyBwcmVmaXggKyBpZCA6IGlkO1xuICB9O1xuXG4gIC8vIEJ5IGRlZmF1bHQsIFVuZGVyc2NvcmUgdXNlcyBFUkItc3R5bGUgdGVtcGxhdGUgZGVsaW1pdGVycywgY2hhbmdlIHRoZVxuICAvLyBmb2xsb3dpbmcgdGVtcGxhdGUgc2V0dGluZ3MgdG8gdXNlIGFsdGVybmF0aXZlIGRlbGltaXRlcnMuXG4gIF8udGVtcGxhdGVTZXR0aW5ncyA9IHtcbiAgICBldmFsdWF0ZSAgICA6IC88JShbXFxzXFxTXSs/KSU+L2csXG4gICAgaW50ZXJwb2xhdGUgOiAvPCU9KFtcXHNcXFNdKz8pJT4vZyxcbiAgICBlc2NhcGUgICAgICA6IC88JS0oW1xcc1xcU10rPyklPi9nXG4gIH07XG5cbiAgLy8gV2hlbiBjdXN0b21pemluZyBgdGVtcGxhdGVTZXR0aW5nc2AsIGlmIHlvdSBkb24ndCB3YW50IHRvIGRlZmluZSBhblxuICAvLyBpbnRlcnBvbGF0aW9uLCBldmFsdWF0aW9uIG9yIGVzY2FwaW5nIHJlZ2V4LCB3ZSBuZWVkIG9uZSB0aGF0IGlzXG4gIC8vIGd1YXJhbnRlZWQgbm90IHRvIG1hdGNoLlxuICB2YXIgbm9NYXRjaCA9IC8oLileLztcblxuICAvLyBDZXJ0YWluIGNoYXJhY3RlcnMgbmVlZCB0byBiZSBlc2NhcGVkIHNvIHRoYXQgdGhleSBjYW4gYmUgcHV0IGludG8gYVxuICAvLyBzdHJpbmcgbGl0ZXJhbC5cbiAgdmFyIGVzY2FwZXMgPSB7XG4gICAgXCInXCI6ICAgICAgXCInXCIsXG4gICAgJ1xcXFwnOiAgICAgJ1xcXFwnLFxuICAgICdcXHInOiAgICAgJ3InLFxuICAgICdcXG4nOiAgICAgJ24nLFxuICAgICdcXHUyMDI4JzogJ3UyMDI4JyxcbiAgICAnXFx1MjAyOSc6ICd1MjAyOSdcbiAgfTtcblxuICB2YXIgZXNjYXBlciA9IC9cXFxcfCd8XFxyfFxcbnxcXHUyMDI4fFxcdTIwMjkvZztcblxuICB2YXIgZXNjYXBlQ2hhciA9IGZ1bmN0aW9uKG1hdGNoKSB7XG4gICAgcmV0dXJuICdcXFxcJyArIGVzY2FwZXNbbWF0Y2hdO1xuICB9O1xuXG4gIC8vIEphdmFTY3JpcHQgbWljcm8tdGVtcGxhdGluZywgc2ltaWxhciB0byBKb2huIFJlc2lnJ3MgaW1wbGVtZW50YXRpb24uXG4gIC8vIFVuZGVyc2NvcmUgdGVtcGxhdGluZyBoYW5kbGVzIGFyYml0cmFyeSBkZWxpbWl0ZXJzLCBwcmVzZXJ2ZXMgd2hpdGVzcGFjZSxcbiAgLy8gYW5kIGNvcnJlY3RseSBlc2NhcGVzIHF1b3RlcyB3aXRoaW4gaW50ZXJwb2xhdGVkIGNvZGUuXG4gIC8vIE5COiBgb2xkU2V0dGluZ3NgIG9ubHkgZXhpc3RzIGZvciBiYWNrd2FyZHMgY29tcGF0aWJpbGl0eS5cbiAgXy50ZW1wbGF0ZSA9IGZ1bmN0aW9uKHRleHQsIHNldHRpbmdzLCBvbGRTZXR0aW5ncykge1xuICAgIGlmICghc2V0dGluZ3MgJiYgb2xkU2V0dGluZ3MpIHNldHRpbmdzID0gb2xkU2V0dGluZ3M7XG4gICAgc2V0dGluZ3MgPSBfLmRlZmF1bHRzKHt9LCBzZXR0aW5ncywgXy50ZW1wbGF0ZVNldHRpbmdzKTtcblxuICAgIC8vIENvbWJpbmUgZGVsaW1pdGVycyBpbnRvIG9uZSByZWd1bGFyIGV4cHJlc3Npb24gdmlhIGFsdGVybmF0aW9uLlxuICAgIHZhciBtYXRjaGVyID0gUmVnRXhwKFtcbiAgICAgIChzZXR0aW5ncy5lc2NhcGUgfHwgbm9NYXRjaCkuc291cmNlLFxuICAgICAgKHNldHRpbmdzLmludGVycG9sYXRlIHx8IG5vTWF0Y2gpLnNvdXJjZSxcbiAgICAgIChzZXR0aW5ncy5ldmFsdWF0ZSB8fCBub01hdGNoKS5zb3VyY2VcbiAgICBdLmpvaW4oJ3wnKSArICd8JCcsICdnJyk7XG5cbiAgICAvLyBDb21waWxlIHRoZSB0ZW1wbGF0ZSBzb3VyY2UsIGVzY2FwaW5nIHN0cmluZyBsaXRlcmFscyBhcHByb3ByaWF0ZWx5LlxuICAgIHZhciBpbmRleCA9IDA7XG4gICAgdmFyIHNvdXJjZSA9IFwiX19wKz0nXCI7XG4gICAgdGV4dC5yZXBsYWNlKG1hdGNoZXIsIGZ1bmN0aW9uKG1hdGNoLCBlc2NhcGUsIGludGVycG9sYXRlLCBldmFsdWF0ZSwgb2Zmc2V0KSB7XG4gICAgICBzb3VyY2UgKz0gdGV4dC5zbGljZShpbmRleCwgb2Zmc2V0KS5yZXBsYWNlKGVzY2FwZXIsIGVzY2FwZUNoYXIpO1xuICAgICAgaW5kZXggPSBvZmZzZXQgKyBtYXRjaC5sZW5ndGg7XG5cbiAgICAgIGlmIChlc2NhcGUpIHtcbiAgICAgICAgc291cmNlICs9IFwiJytcXG4oKF9fdD0oXCIgKyBlc2NhcGUgKyBcIikpPT1udWxsPycnOl8uZXNjYXBlKF9fdCkpK1xcbidcIjtcbiAgICAgIH0gZWxzZSBpZiAoaW50ZXJwb2xhdGUpIHtcbiAgICAgICAgc291cmNlICs9IFwiJytcXG4oKF9fdD0oXCIgKyBpbnRlcnBvbGF0ZSArIFwiKSk9PW51bGw/Jyc6X190KStcXG4nXCI7XG4gICAgICB9IGVsc2UgaWYgKGV2YWx1YXRlKSB7XG4gICAgICAgIHNvdXJjZSArPSBcIic7XFxuXCIgKyBldmFsdWF0ZSArIFwiXFxuX19wKz0nXCI7XG4gICAgICB9XG5cbiAgICAgIC8vIEFkb2JlIFZNcyBuZWVkIHRoZSBtYXRjaCByZXR1cm5lZCB0byBwcm9kdWNlIHRoZSBjb3JyZWN0IG9mZmVzdC5cbiAgICAgIHJldHVybiBtYXRjaDtcbiAgICB9KTtcbiAgICBzb3VyY2UgKz0gXCInO1xcblwiO1xuXG4gICAgLy8gSWYgYSB2YXJpYWJsZSBpcyBub3Qgc3BlY2lmaWVkLCBwbGFjZSBkYXRhIHZhbHVlcyBpbiBsb2NhbCBzY29wZS5cbiAgICBpZiAoIXNldHRpbmdzLnZhcmlhYmxlKSBzb3VyY2UgPSAnd2l0aChvYmp8fHt9KXtcXG4nICsgc291cmNlICsgJ31cXG4nO1xuXG4gICAgc291cmNlID0gXCJ2YXIgX190LF9fcD0nJyxfX2o9QXJyYXkucHJvdG90eXBlLmpvaW4sXCIgK1xuICAgICAgXCJwcmludD1mdW5jdGlvbigpe19fcCs9X19qLmNhbGwoYXJndW1lbnRzLCcnKTt9O1xcblwiICtcbiAgICAgIHNvdXJjZSArICdyZXR1cm4gX19wO1xcbic7XG5cbiAgICB0cnkge1xuICAgICAgdmFyIHJlbmRlciA9IG5ldyBGdW5jdGlvbihzZXR0aW5ncy52YXJpYWJsZSB8fCAnb2JqJywgJ18nLCBzb3VyY2UpO1xuICAgIH0gY2F0Y2ggKGUpIHtcbiAgICAgIGUuc291cmNlID0gc291cmNlO1xuICAgICAgdGhyb3cgZTtcbiAgICB9XG5cbiAgICB2YXIgdGVtcGxhdGUgPSBmdW5jdGlvbihkYXRhKSB7XG4gICAgICByZXR1cm4gcmVuZGVyLmNhbGwodGhpcywgZGF0YSwgXyk7XG4gICAgfTtcblxuICAgIC8vIFByb3ZpZGUgdGhlIGNvbXBpbGVkIHNvdXJjZSBhcyBhIGNvbnZlbmllbmNlIGZvciBwcmVjb21waWxhdGlvbi5cbiAgICB2YXIgYXJndW1lbnQgPSBzZXR0aW5ncy52YXJpYWJsZSB8fCAnb2JqJztcbiAgICB0ZW1wbGF0ZS5zb3VyY2UgPSAnZnVuY3Rpb24oJyArIGFyZ3VtZW50ICsgJyl7XFxuJyArIHNvdXJjZSArICd9JztcblxuICAgIHJldHVybiB0ZW1wbGF0ZTtcbiAgfTtcblxuICAvLyBBZGQgYSBcImNoYWluXCIgZnVuY3Rpb24uIFN0YXJ0IGNoYWluaW5nIGEgd3JhcHBlZCBVbmRlcnNjb3JlIG9iamVjdC5cbiAgXy5jaGFpbiA9IGZ1bmN0aW9uKG9iaikge1xuICAgIHZhciBpbnN0YW5jZSA9IF8ob2JqKTtcbiAgICBpbnN0YW5jZS5fY2hhaW4gPSB0cnVlO1xuICAgIHJldHVybiBpbnN0YW5jZTtcbiAgfTtcblxuICAvLyBPT1BcbiAgLy8gLS0tLS0tLS0tLS0tLS0tXG4gIC8vIElmIFVuZGVyc2NvcmUgaXMgY2FsbGVkIGFzIGEgZnVuY3Rpb24sIGl0IHJldHVybnMgYSB3cmFwcGVkIG9iamVjdCB0aGF0XG4gIC8vIGNhbiBiZSB1c2VkIE9PLXN0eWxlLiBUaGlzIHdyYXBwZXIgaG9sZHMgYWx0ZXJlZCB2ZXJzaW9ucyBvZiBhbGwgdGhlXG4gIC8vIHVuZGVyc2NvcmUgZnVuY3Rpb25zLiBXcmFwcGVkIG9iamVjdHMgbWF5IGJlIGNoYWluZWQuXG5cbiAgLy8gSGVscGVyIGZ1bmN0aW9uIHRvIGNvbnRpbnVlIGNoYWluaW5nIGludGVybWVkaWF0ZSByZXN1bHRzLlxuICB2YXIgcmVzdWx0ID0gZnVuY3Rpb24oaW5zdGFuY2UsIG9iaikge1xuICAgIHJldHVybiBpbnN0YW5jZS5fY2hhaW4gPyBfKG9iaikuY2hhaW4oKSA6IG9iajtcbiAgfTtcblxuICAvLyBBZGQgeW91ciBvd24gY3VzdG9tIGZ1bmN0aW9ucyB0byB0aGUgVW5kZXJzY29yZSBvYmplY3QuXG4gIF8ubWl4aW4gPSBmdW5jdGlvbihvYmopIHtcbiAgICBfLmVhY2goXy5mdW5jdGlvbnMob2JqKSwgZnVuY3Rpb24obmFtZSkge1xuICAgICAgdmFyIGZ1bmMgPSBfW25hbWVdID0gb2JqW25hbWVdO1xuICAgICAgXy5wcm90b3R5cGVbbmFtZV0gPSBmdW5jdGlvbigpIHtcbiAgICAgICAgdmFyIGFyZ3MgPSBbdGhpcy5fd3JhcHBlZF07XG4gICAgICAgIHB1c2guYXBwbHkoYXJncywgYXJndW1lbnRzKTtcbiAgICAgICAgcmV0dXJuIHJlc3VsdCh0aGlzLCBmdW5jLmFwcGx5KF8sIGFyZ3MpKTtcbiAgICAgIH07XG4gICAgfSk7XG4gIH07XG5cbiAgLy8gQWRkIGFsbCBvZiB0aGUgVW5kZXJzY29yZSBmdW5jdGlvbnMgdG8gdGhlIHdyYXBwZXIgb2JqZWN0LlxuICBfLm1peGluKF8pO1xuXG4gIC8vIEFkZCBhbGwgbXV0YXRvciBBcnJheSBmdW5jdGlvbnMgdG8gdGhlIHdyYXBwZXIuXG4gIF8uZWFjaChbJ3BvcCcsICdwdXNoJywgJ3JldmVyc2UnLCAnc2hpZnQnLCAnc29ydCcsICdzcGxpY2UnLCAndW5zaGlmdCddLCBmdW5jdGlvbihuYW1lKSB7XG4gICAgdmFyIG1ldGhvZCA9IEFycmF5UHJvdG9bbmFtZV07XG4gICAgXy5wcm90b3R5cGVbbmFtZV0gPSBmdW5jdGlvbigpIHtcbiAgICAgIHZhciBvYmogPSB0aGlzLl93cmFwcGVkO1xuICAgICAgbWV0aG9kLmFwcGx5KG9iaiwgYXJndW1lbnRzKTtcbiAgICAgIGlmICgobmFtZSA9PT0gJ3NoaWZ0JyB8fCBuYW1lID09PSAnc3BsaWNlJykgJiYgb2JqLmxlbmd0aCA9PT0gMCkgZGVsZXRlIG9ialswXTtcbiAgICAgIHJldHVybiByZXN1bHQodGhpcywgb2JqKTtcbiAgICB9O1xuICB9KTtcblxuICAvLyBBZGQgYWxsIGFjY2Vzc29yIEFycmF5IGZ1bmN0aW9ucyB0byB0aGUgd3JhcHBlci5cbiAgXy5lYWNoKFsnY29uY2F0JywgJ2pvaW4nLCAnc2xpY2UnXSwgZnVuY3Rpb24obmFtZSkge1xuICAgIHZhciBtZXRob2QgPSBBcnJheVByb3RvW25hbWVdO1xuICAgIF8ucHJvdG90eXBlW25hbWVdID0gZnVuY3Rpb24oKSB7XG4gICAgICByZXR1cm4gcmVzdWx0KHRoaXMsIG1ldGhvZC5hcHBseSh0aGlzLl93cmFwcGVkLCBhcmd1bWVudHMpKTtcbiAgICB9O1xuICB9KTtcblxuICAvLyBFeHRyYWN0cyB0aGUgcmVzdWx0IGZyb20gYSB3cmFwcGVkIGFuZCBjaGFpbmVkIG9iamVjdC5cbiAgXy5wcm90b3R5cGUudmFsdWUgPSBmdW5jdGlvbigpIHtcbiAgICByZXR1cm4gdGhpcy5fd3JhcHBlZDtcbiAgfTtcblxuICAvLyBQcm92aWRlIHVud3JhcHBpbmcgcHJveHkgZm9yIHNvbWUgbWV0aG9kcyB1c2VkIGluIGVuZ2luZSBvcGVyYXRpb25zXG4gIC8vIHN1Y2ggYXMgYXJpdGhtZXRpYyBhbmQgSlNPTiBzdHJpbmdpZmljYXRpb24uXG4gIF8ucHJvdG90eXBlLnZhbHVlT2YgPSBfLnByb3RvdHlwZS50b0pTT04gPSBfLnByb3RvdHlwZS52YWx1ZTtcblxuICBfLnByb3RvdHlwZS50b1N0cmluZyA9IGZ1bmN0aW9uKCkge1xuICAgIHJldHVybiAnJyArIHRoaXMuX3dyYXBwZWQ7XG4gIH07XG5cbiAgLy8gQU1EIHJlZ2lzdHJhdGlvbiBoYXBwZW5zIGF0IHRoZSBlbmQgZm9yIGNvbXBhdGliaWxpdHkgd2l0aCBBTUQgbG9hZGVyc1xuICAvLyB0aGF0IG1heSBub3QgZW5mb3JjZSBuZXh0LXR1cm4gc2VtYW50aWNzIG9uIG1vZHVsZXMuIEV2ZW4gdGhvdWdoIGdlbmVyYWxcbiAgLy8gcHJhY3RpY2UgZm9yIEFNRCByZWdpc3RyYXRpb24gaXMgdG8gYmUgYW5vbnltb3VzLCB1bmRlcnNjb3JlIHJlZ2lzdGVyc1xuICAvLyBhcyBhIG5hbWVkIG1vZHVsZSBiZWNhdXNlLCBsaWtlIGpRdWVyeSwgaXQgaXMgYSBiYXNlIGxpYnJhcnkgdGhhdCBpc1xuICAvLyBwb3B1bGFyIGVub3VnaCB0byBiZSBidW5kbGVkIGluIGEgdGhpcmQgcGFydHkgbGliLCBidXQgbm90IGJlIHBhcnQgb2ZcbiAgLy8gYW4gQU1EIGxvYWQgcmVxdWVzdC4gVGhvc2UgY2FzZXMgY291bGQgZ2VuZXJhdGUgYW4gZXJyb3Igd2hlbiBhblxuICAvLyBhbm9ueW1vdXMgZGVmaW5lKCkgaXMgY2FsbGVkIG91dHNpZGUgb2YgYSBsb2FkZXIgcmVxdWVzdC5cbiAgaWYgKHR5cGVvZiBkZWZpbmUgPT09ICdmdW5jdGlvbicgJiYgZGVmaW5lLmFtZCkge1xuICAgIGRlZmluZSgndW5kZXJzY29yZScsIFtdLCBmdW5jdGlvbigpIHtcbiAgICAgIHJldHVybiBfO1xuICAgIH0pO1xuICB9XG59LmNhbGwodGhpcykpO1xuXG5cblxuLyoqKioqKioqKioqKioqKioqXG4gKiogV0VCUEFDSyBGT09URVJcbiAqKiAvVXNlcnMveHVnZS9wcm9qZWN0L3F1YW5zaGktdWkvfi91bmRlcnNjb3JlL3VuZGVyc2NvcmUuanNcbiAqKiBtb2R1bGUgaWQgPSAxODRcbiAqKiBtb2R1bGUgY2h1bmtzID0gMCAyIDMgNCA1IDYgN1xuICoqLyIsInJlcXVpcmUoJy4vdWktYm9vdHN0cmFwLXRwbHMnKTtcbm1vZHVsZS5leHBvcnRzID0gJ3VpLmJvb3RzdHJhcCc7XG5cblxuXG4vKioqKioqKioqKioqKioqKipcbiAqKiBXRUJQQUNLIEZPT1RFUlxuICoqIC9Vc2Vycy94dWdlL3Byb2plY3QvcXVhbnNoaS11aS9+L2FuZ3VsYXItdWktYm9vdHN0cmFwL2luZGV4LmpzXG4gKiogbW9kdWxlIGlkID0gMTg1XG4gKiogbW9kdWxlIGNodW5rcyA9IDAgMSAyIDNcbiAqKi8iLCIvKlxuICogYW5ndWxhci11aS1ib290c3RyYXBcbiAqIGh0dHA6Ly9hbmd1bGFyLXVpLmdpdGh1Yi5pby9ib290c3RyYXAvXG5cbiAqIFZlcnNpb246IDAuMTQuMyAtIDIwMTUtMTAtMjNcbiAqIExpY2Vuc2U6IE1JVFxuICovXG5hbmd1bGFyLm1vZHVsZShcInVpLmJvb3RzdHJhcFwiLCBbXCJ1aS5ib290c3RyYXAudHBsc1wiLCBcInVpLmJvb3RzdHJhcC5jb2xsYXBzZVwiLFwidWkuYm9vdHN0cmFwLmFjY29yZGlvblwiLFwidWkuYm9vdHN0cmFwLmFsZXJ0XCIsXCJ1aS5ib290c3RyYXAuYnV0dG9uc1wiLFwidWkuYm9vdHN0cmFwLmNhcm91c2VsXCIsXCJ1aS5ib290c3RyYXAuZGF0ZXBhcnNlclwiLFwidWkuYm9vdHN0cmFwLnBvc2l0aW9uXCIsXCJ1aS5ib290c3RyYXAuZGF0ZXBpY2tlclwiLFwidWkuYm9vdHN0cmFwLmRyb3Bkb3duXCIsXCJ1aS5ib290c3RyYXAuc3RhY2tlZE1hcFwiLFwidWkuYm9vdHN0cmFwLm1vZGFsXCIsXCJ1aS5ib290c3RyYXAucGFnaW5hdGlvblwiLFwidWkuYm9vdHN0cmFwLnRvb2x0aXBcIixcInVpLmJvb3RzdHJhcC5wb3BvdmVyXCIsXCJ1aS5ib290c3RyYXAucHJvZ3Jlc3NiYXJcIixcInVpLmJvb3RzdHJhcC5yYXRpbmdcIixcInVpLmJvb3RzdHJhcC50YWJzXCIsXCJ1aS5ib290c3RyYXAudGltZXBpY2tlclwiLFwidWkuYm9vdHN0cmFwLnR5cGVhaGVhZFwiXSk7XG5hbmd1bGFyLm1vZHVsZShcInVpLmJvb3RzdHJhcC50cGxzXCIsIFtcInRlbXBsYXRlL2FjY29yZGlvbi9hY2NvcmRpb24tZ3JvdXAuaHRtbFwiLFwidGVtcGxhdGUvYWNjb3JkaW9uL2FjY29yZGlvbi5odG1sXCIsXCJ0ZW1wbGF0ZS9hbGVydC9hbGVydC5odG1sXCIsXCJ0ZW1wbGF0ZS9jYXJvdXNlbC9jYXJvdXNlbC5odG1sXCIsXCJ0ZW1wbGF0ZS9jYXJvdXNlbC9zbGlkZS5odG1sXCIsXCJ0ZW1wbGF0ZS9kYXRlcGlja2VyL2RhdGVwaWNrZXIuaHRtbFwiLFwidGVtcGxhdGUvZGF0ZXBpY2tlci9kYXkuaHRtbFwiLFwidGVtcGxhdGUvZGF0ZXBpY2tlci9tb250aC5odG1sXCIsXCJ0ZW1wbGF0ZS9kYXRlcGlja2VyL3BvcHVwLmh0bWxcIixcInRlbXBsYXRlL2RhdGVwaWNrZXIveWVhci5odG1sXCIsXCJ0ZW1wbGF0ZS9tb2RhbC9iYWNrZHJvcC5odG1sXCIsXCJ0ZW1wbGF0ZS9tb2RhbC93aW5kb3cuaHRtbFwiLFwidGVtcGxhdGUvcGFnaW5hdGlvbi9wYWdlci5odG1sXCIsXCJ0ZW1wbGF0ZS9wYWdpbmF0aW9uL3BhZ2luYXRpb24uaHRtbFwiLFwidGVtcGxhdGUvdG9vbHRpcC90b29sdGlwLWh0bWwtcG9wdXAuaHRtbFwiLFwidGVtcGxhdGUvdG9vbHRpcC90b29sdGlwLXBvcHVwLmh0bWxcIixcInRlbXBsYXRlL3Rvb2x0aXAvdG9vbHRpcC10ZW1wbGF0ZS1wb3B1cC5odG1sXCIsXCJ0ZW1wbGF0ZS9wb3BvdmVyL3BvcG92ZXItaHRtbC5odG1sXCIsXCJ0ZW1wbGF0ZS9wb3BvdmVyL3BvcG92ZXItdGVtcGxhdGUuaHRtbFwiLFwidGVtcGxhdGUvcG9wb3Zlci9wb3BvdmVyLmh0bWxcIixcInRlbXBsYXRlL3Byb2dyZXNzYmFyL2Jhci5odG1sXCIsXCJ0ZW1wbGF0ZS9wcm9ncmVzc2Jhci9wcm9ncmVzcy5odG1sXCIsXCJ0ZW1wbGF0ZS9wcm9ncmVzc2Jhci9wcm9ncmVzc2Jhci5odG1sXCIsXCJ0ZW1wbGF0ZS9yYXRpbmcvcmF0aW5nLmh0bWxcIixcInRlbXBsYXRlL3RhYnMvdGFiLmh0bWxcIixcInRlbXBsYXRlL3RhYnMvdGFic2V0Lmh0bWxcIixcInRlbXBsYXRlL3RpbWVwaWNrZXIvdGltZXBpY2tlci5odG1sXCIsXCJ0ZW1wbGF0ZS90eXBlYWhlYWQvdHlwZWFoZWFkLW1hdGNoLmh0bWxcIixcInRlbXBsYXRlL3R5cGVhaGVhZC90eXBlYWhlYWQtcG9wdXAuaHRtbFwiXSk7XG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLmNvbGxhcHNlJywgW10pXG5cbiAgLmRpcmVjdGl2ZSgndWliQ29sbGFwc2UnLCBbJyRhbmltYXRlJywgJyRpbmplY3RvcicsIGZ1bmN0aW9uKCRhbmltYXRlLCAkaW5qZWN0b3IpIHtcbiAgICB2YXIgJGFuaW1hdGVDc3MgPSAkaW5qZWN0b3IuaGFzKCckYW5pbWF0ZUNzcycpID8gJGluamVjdG9yLmdldCgnJGFuaW1hdGVDc3MnKSA6IG51bGw7XG4gICAgcmV0dXJuIHtcbiAgICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycykge1xuICAgICAgICBmdW5jdGlvbiBleHBhbmQoKSB7XG4gICAgICAgICAgZWxlbWVudC5yZW1vdmVDbGFzcygnY29sbGFwc2UnKVxuICAgICAgICAgICAgLmFkZENsYXNzKCdjb2xsYXBzaW5nJylcbiAgICAgICAgICAgIC5hdHRyKCdhcmlhLWV4cGFuZGVkJywgdHJ1ZSlcbiAgICAgICAgICAgIC5hdHRyKCdhcmlhLWhpZGRlbicsIGZhbHNlKTtcblxuICAgICAgICAgIGlmICgkYW5pbWF0ZUNzcykge1xuICAgICAgICAgICAgJGFuaW1hdGVDc3MoZWxlbWVudCwge1xuICAgICAgICAgICAgICBhZGRDbGFzczogJ2luJyxcbiAgICAgICAgICAgICAgZWFzaW5nOiAnZWFzZScsXG4gICAgICAgICAgICAgIHRvOiB7IGhlaWdodDogZWxlbWVudFswXS5zY3JvbGxIZWlnaHQgKyAncHgnIH1cbiAgICAgICAgICAgIH0pLnN0YXJ0KCkuZmluYWxseShleHBhbmREb25lKTtcbiAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgJGFuaW1hdGUuYWRkQ2xhc3MoZWxlbWVudCwgJ2luJywge1xuICAgICAgICAgICAgICB0bzogeyBoZWlnaHQ6IGVsZW1lbnRbMF0uc2Nyb2xsSGVpZ2h0ICsgJ3B4JyB9XG4gICAgICAgICAgICB9KS50aGVuKGV4cGFuZERvbmUpO1xuICAgICAgICAgIH1cbiAgICAgICAgfVxuXG4gICAgICAgIGZ1bmN0aW9uIGV4cGFuZERvbmUoKSB7XG4gICAgICAgICAgZWxlbWVudC5yZW1vdmVDbGFzcygnY29sbGFwc2luZycpXG4gICAgICAgICAgICAuYWRkQ2xhc3MoJ2NvbGxhcHNlJylcbiAgICAgICAgICAgIC5jc3Moe2hlaWdodDogJ2F1dG8nfSk7XG4gICAgICAgIH1cblxuICAgICAgICBmdW5jdGlvbiBjb2xsYXBzZSgpIHtcbiAgICAgICAgICBpZiAoIWVsZW1lbnQuaGFzQ2xhc3MoJ2NvbGxhcHNlJykgJiYgIWVsZW1lbnQuaGFzQ2xhc3MoJ2luJykpIHtcbiAgICAgICAgICAgIHJldHVybiBjb2xsYXBzZURvbmUoKTtcbiAgICAgICAgICB9XG5cbiAgICAgICAgICBlbGVtZW50XG4gICAgICAgICAgICAvLyBJTVBPUlRBTlQ6IFRoZSBoZWlnaHQgbXVzdCBiZSBzZXQgYmVmb3JlIGFkZGluZyBcImNvbGxhcHNpbmdcIiBjbGFzcy5cbiAgICAgICAgICAgIC8vIE90aGVyd2lzZSwgdGhlIGJyb3dzZXIgYXR0ZW1wdHMgdG8gYW5pbWF0ZSBmcm9tIGhlaWdodCAwIChpblxuICAgICAgICAgICAgLy8gY29sbGFwc2luZyBjbGFzcykgdG8gdGhlIGdpdmVuIGhlaWdodCBoZXJlLlxuICAgICAgICAgICAgLmNzcyh7aGVpZ2h0OiBlbGVtZW50WzBdLnNjcm9sbEhlaWdodCArICdweCd9KVxuICAgICAgICAgICAgLy8gaW5pdGlhbGx5IGFsbCBwYW5lbCBjb2xsYXBzZSBoYXZlIHRoZSBjb2xsYXBzZSBjbGFzcywgdGhpcyByZW1vdmFsXG4gICAgICAgICAgICAvLyBwcmV2ZW50cyB0aGUgYW5pbWF0aW9uIGZyb20ganVtcGluZyB0byBjb2xsYXBzZWQgc3RhdGVcbiAgICAgICAgICAgIC5yZW1vdmVDbGFzcygnY29sbGFwc2UnKVxuICAgICAgICAgICAgLmFkZENsYXNzKCdjb2xsYXBzaW5nJylcbiAgICAgICAgICAgIC5hdHRyKCdhcmlhLWV4cGFuZGVkJywgZmFsc2UpXG4gICAgICAgICAgICAuYXR0cignYXJpYS1oaWRkZW4nLCB0cnVlKTtcblxuICAgICAgICAgIGlmICgkYW5pbWF0ZUNzcykge1xuICAgICAgICAgICAgJGFuaW1hdGVDc3MoZWxlbWVudCwge1xuICAgICAgICAgICAgICByZW1vdmVDbGFzczogJ2luJyxcbiAgICAgICAgICAgICAgdG86IHtoZWlnaHQ6ICcwJ31cbiAgICAgICAgICAgIH0pLnN0YXJ0KCkuZmluYWxseShjb2xsYXBzZURvbmUpO1xuICAgICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgICAkYW5pbWF0ZS5yZW1vdmVDbGFzcyhlbGVtZW50LCAnaW4nLCB7XG4gICAgICAgICAgICAgIHRvOiB7aGVpZ2h0OiAnMCd9XG4gICAgICAgICAgICB9KS50aGVuKGNvbGxhcHNlRG9uZSk7XG4gICAgICAgICAgfVxuICAgICAgICB9XG5cbiAgICAgICAgZnVuY3Rpb24gY29sbGFwc2VEb25lKCkge1xuICAgICAgICAgIGVsZW1lbnQuY3NzKHtoZWlnaHQ6ICcwJ30pOyAvLyBSZXF1aXJlZCBzbyB0aGF0IGNvbGxhcHNlIHdvcmtzIHdoZW4gYW5pbWF0aW9uIGlzIGRpc2FibGVkXG4gICAgICAgICAgZWxlbWVudC5yZW1vdmVDbGFzcygnY29sbGFwc2luZycpXG4gICAgICAgICAgICAuYWRkQ2xhc3MoJ2NvbGxhcHNlJyk7XG4gICAgICAgIH1cblxuICAgICAgICBzY29wZS4kd2F0Y2goYXR0cnMudWliQ29sbGFwc2UsIGZ1bmN0aW9uKHNob3VsZENvbGxhcHNlKSB7XG4gICAgICAgICAgaWYgKHNob3VsZENvbGxhcHNlKSB7XG4gICAgICAgICAgICBjb2xsYXBzZSgpO1xuICAgICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgICBleHBhbmQoKTtcbiAgICAgICAgICB9XG4gICAgICAgIH0pO1xuICAgICAgfVxuICAgIH07XG4gIH1dKTtcblxuLyogRGVwcmVjYXRlZCBjb2xsYXBzZSBiZWxvdyAqL1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLmNvbGxhcHNlJylcblxuICAudmFsdWUoJyRjb2xsYXBzZVN1cHByZXNzV2FybmluZycsIGZhbHNlKVxuXG4gIC5kaXJlY3RpdmUoJ2NvbGxhcHNlJywgWyckYW5pbWF0ZScsICckaW5qZWN0b3InLCAnJGxvZycsICckY29sbGFwc2VTdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkYW5pbWF0ZSwgJGluamVjdG9yLCAkbG9nLCAkY29sbGFwc2VTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICB2YXIgJGFuaW1hdGVDc3MgPSAkaW5qZWN0b3IuaGFzKCckYW5pbWF0ZUNzcycpID8gJGluamVjdG9yLmdldCgnJGFuaW1hdGVDc3MnKSA6IG51bGw7XG4gICAgcmV0dXJuIHtcbiAgICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycykge1xuICAgICAgICBpZiAoISRjb2xsYXBzZVN1cHByZXNzV2FybmluZykge1xuICAgICAgICAgICRsb2cud2FybignY29sbGFwc2UgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSB1aWItY29sbGFwc2UgaW5zdGVhZC4nKTtcbiAgICAgICAgfVxuXG4gICAgICAgIGZ1bmN0aW9uIGV4cGFuZCgpIHtcbiAgICAgICAgICBlbGVtZW50LnJlbW92ZUNsYXNzKCdjb2xsYXBzZScpXG4gICAgICAgICAgICAuYWRkQ2xhc3MoJ2NvbGxhcHNpbmcnKVxuICAgICAgICAgICAgLmF0dHIoJ2FyaWEtZXhwYW5kZWQnLCB0cnVlKVxuICAgICAgICAgICAgLmF0dHIoJ2FyaWEtaGlkZGVuJywgZmFsc2UpO1xuXG4gICAgICAgICAgaWYgKCRhbmltYXRlQ3NzKSB7XG4gICAgICAgICAgICAkYW5pbWF0ZUNzcyhlbGVtZW50LCB7XG4gICAgICAgICAgICAgIGVhc2luZzogJ2Vhc2UnLFxuICAgICAgICAgICAgICB0bzogeyBoZWlnaHQ6IGVsZW1lbnRbMF0uc2Nyb2xsSGVpZ2h0ICsgJ3B4JyB9XG4gICAgICAgICAgICB9KS5zdGFydCgpLmRvbmUoZXhwYW5kRG9uZSk7XG4gICAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICAgICRhbmltYXRlLmFuaW1hdGUoZWxlbWVudCwge30sIHtcbiAgICAgICAgICAgICAgaGVpZ2h0OiBlbGVtZW50WzBdLnNjcm9sbEhlaWdodCArICdweCdcbiAgICAgICAgICAgIH0pLnRoZW4oZXhwYW5kRG9uZSk7XG4gICAgICAgICAgfVxuICAgICAgICB9XG5cbiAgICAgICAgZnVuY3Rpb24gZXhwYW5kRG9uZSgpIHtcbiAgICAgICAgICBlbGVtZW50LnJlbW92ZUNsYXNzKCdjb2xsYXBzaW5nJylcbiAgICAgICAgICAgIC5hZGRDbGFzcygnY29sbGFwc2UgaW4nKVxuICAgICAgICAgICAgLmNzcyh7aGVpZ2h0OiAnYXV0byd9KTtcbiAgICAgICAgfVxuXG4gICAgICAgIGZ1bmN0aW9uIGNvbGxhcHNlKCkge1xuICAgICAgICAgIGlmICghZWxlbWVudC5oYXNDbGFzcygnY29sbGFwc2UnKSAmJiAhZWxlbWVudC5oYXNDbGFzcygnaW4nKSkge1xuICAgICAgICAgICAgcmV0dXJuIGNvbGxhcHNlRG9uZSgpO1xuICAgICAgICAgIH1cblxuICAgICAgICAgIGVsZW1lbnRcbiAgICAgICAgICAgIC8vIElNUE9SVEFOVDogVGhlIGhlaWdodCBtdXN0IGJlIHNldCBiZWZvcmUgYWRkaW5nIFwiY29sbGFwc2luZ1wiIGNsYXNzLlxuICAgICAgICAgICAgLy8gT3RoZXJ3aXNlLCB0aGUgYnJvd3NlciBhdHRlbXB0cyB0byBhbmltYXRlIGZyb20gaGVpZ2h0IDAgKGluXG4gICAgICAgICAgICAvLyBjb2xsYXBzaW5nIGNsYXNzKSB0byB0aGUgZ2l2ZW4gaGVpZ2h0IGhlcmUuXG4gICAgICAgICAgICAuY3NzKHtoZWlnaHQ6IGVsZW1lbnRbMF0uc2Nyb2xsSGVpZ2h0ICsgJ3B4J30pXG4gICAgICAgICAgICAvLyBpbml0aWFsbHkgYWxsIHBhbmVsIGNvbGxhcHNlIGhhdmUgdGhlIGNvbGxhcHNlIGNsYXNzLCB0aGlzIHJlbW92YWxcbiAgICAgICAgICAgIC8vIHByZXZlbnRzIHRoZSBhbmltYXRpb24gZnJvbSBqdW1waW5nIHRvIGNvbGxhcHNlZCBzdGF0ZVxuICAgICAgICAgICAgLnJlbW92ZUNsYXNzKCdjb2xsYXBzZSBpbicpXG4gICAgICAgICAgICAuYWRkQ2xhc3MoJ2NvbGxhcHNpbmcnKVxuICAgICAgICAgICAgLmF0dHIoJ2FyaWEtZXhwYW5kZWQnLCBmYWxzZSlcbiAgICAgICAgICAgIC5hdHRyKCdhcmlhLWhpZGRlbicsIHRydWUpO1xuXG4gICAgICAgICAgaWYgKCRhbmltYXRlQ3NzKSB7XG4gICAgICAgICAgICAkYW5pbWF0ZUNzcyhlbGVtZW50LCB7XG4gICAgICAgICAgICAgIHRvOiB7aGVpZ2h0OiAnMCd9XG4gICAgICAgICAgICB9KS5zdGFydCgpLmRvbmUoY29sbGFwc2VEb25lKTtcbiAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgJGFuaW1hdGUuYW5pbWF0ZShlbGVtZW50LCB7fSwge1xuICAgICAgICAgICAgICBoZWlnaHQ6ICcwJ1xuICAgICAgICAgICAgfSkudGhlbihjb2xsYXBzZURvbmUpO1xuICAgICAgICAgIH1cbiAgICAgICAgfVxuXG4gICAgICAgIGZ1bmN0aW9uIGNvbGxhcHNlRG9uZSgpIHtcbiAgICAgICAgICBlbGVtZW50LmNzcyh7aGVpZ2h0OiAnMCd9KTsgLy8gUmVxdWlyZWQgc28gdGhhdCBjb2xsYXBzZSB3b3JrcyB3aGVuIGFuaW1hdGlvbiBpcyBkaXNhYmxlZFxuICAgICAgICAgIGVsZW1lbnQucmVtb3ZlQ2xhc3MoJ2NvbGxhcHNpbmcnKVxuICAgICAgICAgICAgLmFkZENsYXNzKCdjb2xsYXBzZScpO1xuICAgICAgICB9XG5cbiAgICAgICAgc2NvcGUuJHdhdGNoKGF0dHJzLmNvbGxhcHNlLCBmdW5jdGlvbihzaG91bGRDb2xsYXBzZSkge1xuICAgICAgICAgIGlmIChzaG91bGRDb2xsYXBzZSkge1xuICAgICAgICAgICAgY29sbGFwc2UoKTtcbiAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgZXhwYW5kKCk7XG4gICAgICAgICAgfVxuICAgICAgICB9KTtcbiAgICAgIH1cbiAgICB9O1xuICB9XSk7XG5cbmFuZ3VsYXIubW9kdWxlKCd1aS5ib290c3RyYXAuYWNjb3JkaW9uJywgWyd1aS5ib290c3RyYXAuY29sbGFwc2UnXSlcblxuLmNvbnN0YW50KCd1aWJBY2NvcmRpb25Db25maWcnLCB7XG4gIGNsb3NlT3RoZXJzOiB0cnVlXG59KVxuXG4uY29udHJvbGxlcignVWliQWNjb3JkaW9uQ29udHJvbGxlcicsIFsnJHNjb3BlJywgJyRhdHRycycsICd1aWJBY2NvcmRpb25Db25maWcnLCBmdW5jdGlvbigkc2NvcGUsICRhdHRycywgYWNjb3JkaW9uQ29uZmlnKSB7XG4gIC8vIFRoaXMgYXJyYXkga2VlcHMgdHJhY2sgb2YgdGhlIGFjY29yZGlvbiBncm91cHNcbiAgdGhpcy5ncm91cHMgPSBbXTtcblxuICAvLyBFbnN1cmUgdGhhdCBhbGwgdGhlIGdyb3VwcyBpbiB0aGlzIGFjY29yZGlvbiBhcmUgY2xvc2VkLCB1bmxlc3MgY2xvc2Utb3RoZXJzIGV4cGxpY2l0bHkgc2F5cyBub3QgdG9cbiAgdGhpcy5jbG9zZU90aGVycyA9IGZ1bmN0aW9uKG9wZW5Hcm91cCkge1xuICAgIHZhciBjbG9zZU90aGVycyA9IGFuZ3VsYXIuaXNEZWZpbmVkKCRhdHRycy5jbG9zZU90aGVycykgP1xuICAgICAgJHNjb3BlLiRldmFsKCRhdHRycy5jbG9zZU90aGVycykgOiBhY2NvcmRpb25Db25maWcuY2xvc2VPdGhlcnM7XG4gICAgaWYgKGNsb3NlT3RoZXJzKSB7XG4gICAgICBhbmd1bGFyLmZvckVhY2godGhpcy5ncm91cHMsIGZ1bmN0aW9uKGdyb3VwKSB7XG4gICAgICAgIGlmIChncm91cCAhPT0gb3Blbkdyb3VwKSB7XG4gICAgICAgICAgZ3JvdXAuaXNPcGVuID0gZmFsc2U7XG4gICAgICAgIH1cbiAgICAgIH0pO1xuICAgIH1cbiAgfTtcblxuICAvLyBUaGlzIGlzIGNhbGxlZCBmcm9tIHRoZSBhY2NvcmRpb24tZ3JvdXAgZGlyZWN0aXZlIHRvIGFkZCBpdHNlbGYgdG8gdGhlIGFjY29yZGlvblxuICB0aGlzLmFkZEdyb3VwID0gZnVuY3Rpb24oZ3JvdXBTY29wZSkge1xuICAgIHZhciB0aGF0ID0gdGhpcztcbiAgICB0aGlzLmdyb3Vwcy5wdXNoKGdyb3VwU2NvcGUpO1xuXG4gICAgZ3JvdXBTY29wZS4kb24oJyRkZXN0cm95JywgZnVuY3Rpb24oZXZlbnQpIHtcbiAgICAgIHRoYXQucmVtb3ZlR3JvdXAoZ3JvdXBTY29wZSk7XG4gICAgfSk7XG4gIH07XG5cbiAgLy8gVGhpcyBpcyBjYWxsZWQgZnJvbSB0aGUgYWNjb3JkaW9uLWdyb3VwIGRpcmVjdGl2ZSB3aGVuIHRvIHJlbW92ZSBpdHNlbGZcbiAgdGhpcy5yZW1vdmVHcm91cCA9IGZ1bmN0aW9uKGdyb3VwKSB7XG4gICAgdmFyIGluZGV4ID0gdGhpcy5ncm91cHMuaW5kZXhPZihncm91cCk7XG4gICAgaWYgKGluZGV4ICE9PSAtMSkge1xuICAgICAgdGhpcy5ncm91cHMuc3BsaWNlKGluZGV4LCAxKTtcbiAgICB9XG4gIH07XG5cbn1dKVxuXG4vLyBUaGUgYWNjb3JkaW9uIGRpcmVjdGl2ZSBzaW1wbHkgc2V0cyB1cCB0aGUgZGlyZWN0aXZlIGNvbnRyb2xsZXJcbi8vIGFuZCBhZGRzIGFuIGFjY29yZGlvbiBDU1MgY2xhc3MgdG8gaXRzZWxmIGVsZW1lbnQuXG4uZGlyZWN0aXZlKCd1aWJBY2NvcmRpb24nLCBmdW5jdGlvbigpIHtcbiAgcmV0dXJuIHtcbiAgICBjb250cm9sbGVyOiAnVWliQWNjb3JkaW9uQ29udHJvbGxlcicsXG4gICAgY29udHJvbGxlckFzOiAnYWNjb3JkaW9uJyxcbiAgICB0cmFuc2NsdWRlOiB0cnVlLFxuICAgIHRlbXBsYXRlVXJsOiBmdW5jdGlvbihlbGVtZW50LCBhdHRycykge1xuICAgICAgcmV0dXJuIGF0dHJzLnRlbXBsYXRlVXJsIHx8ICd0ZW1wbGF0ZS9hY2NvcmRpb24vYWNjb3JkaW9uLmh0bWwnO1xuICAgIH1cbiAgfTtcbn0pXG5cbi8vIFRoZSBhY2NvcmRpb24tZ3JvdXAgZGlyZWN0aXZlIGluZGljYXRlcyBhIGJsb2NrIG9mIGh0bWwgdGhhdCB3aWxsIGV4cGFuZCBhbmQgY29sbGFwc2UgaW4gYW4gYWNjb3JkaW9uXG4uZGlyZWN0aXZlKCd1aWJBY2NvcmRpb25Hcm91cCcsIGZ1bmN0aW9uKCkge1xuICByZXR1cm4ge1xuICAgIHJlcXVpcmU6ICdedWliQWNjb3JkaW9uJywgICAgICAgICAvLyBXZSBuZWVkIHRoaXMgZGlyZWN0aXZlIHRvIGJlIGluc2lkZSBhbiBhY2NvcmRpb25cbiAgICB0cmFuc2NsdWRlOiB0cnVlLCAgICAgICAgICAgICAgLy8gSXQgdHJhbnNjbHVkZXMgdGhlIGNvbnRlbnRzIG9mIHRoZSBkaXJlY3RpdmUgaW50byB0aGUgdGVtcGxhdGVcbiAgICByZXBsYWNlOiB0cnVlLCAgICAgICAgICAgICAgICAvLyBUaGUgZWxlbWVudCBjb250YWluaW5nIHRoZSBkaXJlY3RpdmUgd2lsbCBiZSByZXBsYWNlZCB3aXRoIHRoZSB0ZW1wbGF0ZVxuICAgIHRlbXBsYXRlVXJsOiBmdW5jdGlvbihlbGVtZW50LCBhdHRycykge1xuICAgICAgcmV0dXJuIGF0dHJzLnRlbXBsYXRlVXJsIHx8ICd0ZW1wbGF0ZS9hY2NvcmRpb24vYWNjb3JkaW9uLWdyb3VwLmh0bWwnO1xuICAgIH0sXG4gICAgc2NvcGU6IHtcbiAgICAgIGhlYWRpbmc6ICdAJywgICAgICAgICAgICAgICAvLyBJbnRlcnBvbGF0ZSB0aGUgaGVhZGluZyBhdHRyaWJ1dGUgb250byB0aGlzIHNjb3BlXG4gICAgICBpc09wZW46ICc9PycsXG4gICAgICBpc0Rpc2FibGVkOiAnPT8nXG4gICAgfSxcbiAgICBjb250cm9sbGVyOiBmdW5jdGlvbigpIHtcbiAgICAgIHRoaXMuc2V0SGVhZGluZyA9IGZ1bmN0aW9uKGVsZW1lbnQpIHtcbiAgICAgICAgdGhpcy5oZWFkaW5nID0gZWxlbWVudDtcbiAgICAgIH07XG4gICAgfSxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMsIGFjY29yZGlvbkN0cmwpIHtcbiAgICAgIGFjY29yZGlvbkN0cmwuYWRkR3JvdXAoc2NvcGUpO1xuXG4gICAgICBzY29wZS5vcGVuQ2xhc3MgPSBhdHRycy5vcGVuQ2xhc3MgfHwgJ3BhbmVsLW9wZW4nO1xuICAgICAgc2NvcGUucGFuZWxDbGFzcyA9IGF0dHJzLnBhbmVsQ2xhc3M7XG4gICAgICBzY29wZS4kd2F0Y2goJ2lzT3BlbicsIGZ1bmN0aW9uKHZhbHVlKSB7XG4gICAgICAgIGVsZW1lbnQudG9nZ2xlQ2xhc3Moc2NvcGUub3BlbkNsYXNzLCAhIXZhbHVlKTtcbiAgICAgICAgaWYgKHZhbHVlKSB7XG4gICAgICAgICAgYWNjb3JkaW9uQ3RybC5jbG9zZU90aGVycyhzY29wZSk7XG4gICAgICAgIH1cbiAgICAgIH0pO1xuXG4gICAgICBzY29wZS50b2dnbGVPcGVuID0gZnVuY3Rpb24oJGV2ZW50KSB7XG4gICAgICAgIGlmICghc2NvcGUuaXNEaXNhYmxlZCkge1xuICAgICAgICAgIGlmICghJGV2ZW50IHx8ICRldmVudC53aGljaCA9PT0gMzIpIHtcbiAgICAgICAgICAgIHNjb3BlLmlzT3BlbiA9ICFzY29wZS5pc09wZW47XG4gICAgICAgICAgfVxuICAgICAgICB9XG4gICAgICB9O1xuICAgIH1cbiAgfTtcbn0pXG5cbi8vIFVzZSBhY2NvcmRpb24taGVhZGluZyBiZWxvdyBhbiBhY2NvcmRpb24tZ3JvdXAgdG8gcHJvdmlkZSBhIGhlYWRpbmcgY29udGFpbmluZyBIVE1MXG4uZGlyZWN0aXZlKCd1aWJBY2NvcmRpb25IZWFkaW5nJywgZnVuY3Rpb24oKSB7XG4gIHJldHVybiB7XG4gICAgdHJhbnNjbHVkZTogdHJ1ZSwgICAvLyBHcmFiIHRoZSBjb250ZW50cyB0byBiZSB1c2VkIGFzIHRoZSBoZWFkaW5nXG4gICAgdGVtcGxhdGU6ICcnLCAgICAgICAvLyBJbiBlZmZlY3QgcmVtb3ZlIHRoaXMgZWxlbWVudCFcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHJlcXVpcmU6ICdedWliQWNjb3JkaW9uR3JvdXAnLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgYWNjb3JkaW9uR3JvdXBDdHJsLCB0cmFuc2NsdWRlKSB7XG4gICAgICAvLyBQYXNzIHRoZSBoZWFkaW5nIHRvIHRoZSBhY2NvcmRpb24tZ3JvdXAgY29udHJvbGxlclxuICAgICAgLy8gc28gdGhhdCBpdCBjYW4gYmUgdHJhbnNjbHVkZWQgaW50byB0aGUgcmlnaHQgcGxhY2UgaW4gdGhlIHRlbXBsYXRlXG4gICAgICAvLyBbVGhlIHNlY29uZCBwYXJhbWV0ZXIgdG8gdHJhbnNjbHVkZSBjYXVzZXMgdGhlIGVsZW1lbnRzIHRvIGJlIGNsb25lZCBzbyB0aGF0IHRoZXkgd29yayBpbiBuZy1yZXBlYXRdXG4gICAgICBhY2NvcmRpb25Hcm91cEN0cmwuc2V0SGVhZGluZyh0cmFuc2NsdWRlKHNjb3BlLCBhbmd1bGFyLm5vb3ApKTtcbiAgICB9XG4gIH07XG59KVxuXG4vLyBVc2UgaW4gdGhlIGFjY29yZGlvbi1ncm91cCB0ZW1wbGF0ZSB0byBpbmRpY2F0ZSB3aGVyZSB5b3Ugd2FudCB0aGUgaGVhZGluZyB0byBiZSB0cmFuc2NsdWRlZFxuLy8gWW91IG11c3QgcHJvdmlkZSB0aGUgcHJvcGVydHkgb24gdGhlIGFjY29yZGlvbi1ncm91cCBjb250cm9sbGVyIHRoYXQgd2lsbCBob2xkIHRoZSB0cmFuc2NsdWRlZCBlbGVtZW50XG4uZGlyZWN0aXZlKCd1aWJBY2NvcmRpb25UcmFuc2NsdWRlJywgZnVuY3Rpb24oKSB7XG4gIHJldHVybiB7XG4gICAgcmVxdWlyZTogWyc/XnVpYkFjY29yZGlvbkdyb3VwJywgJz9eYWNjb3JkaW9uR3JvdXAnXSxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMsIGNvbnRyb2xsZXIpIHtcbiAgICAgIGNvbnRyb2xsZXIgPSBjb250cm9sbGVyWzBdID8gY29udHJvbGxlclswXSA6IGNvbnRyb2xsZXJbMV07IC8vIERlbGV0ZSBhZnRlciB3ZSByZW1vdmUgZGVwcmVjYXRpb25cbiAgICAgIHNjb3BlLiR3YXRjaChmdW5jdGlvbigpIHsgcmV0dXJuIGNvbnRyb2xsZXJbYXR0cnMudWliQWNjb3JkaW9uVHJhbnNjbHVkZV07IH0sIGZ1bmN0aW9uKGhlYWRpbmcpIHtcbiAgICAgICAgaWYgKGhlYWRpbmcpIHtcbiAgICAgICAgICBlbGVtZW50LmZpbmQoJ3NwYW4nKS5odG1sKCcnKTtcbiAgICAgICAgICBlbGVtZW50LmZpbmQoJ3NwYW4nKS5hcHBlbmQoaGVhZGluZyk7XG4gICAgICAgIH1cbiAgICAgIH0pO1xuICAgIH1cbiAgfTtcbn0pO1xuXG4vKiBEZXByZWNhdGVkIGFjY29yZGlvbiBiZWxvdyAqL1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLmFjY29yZGlvbicpXG5cbiAgLnZhbHVlKCckYWNjb3JkaW9uU3VwcHJlc3NXYXJuaW5nJywgZmFsc2UpXG5cbiAgLmNvbnRyb2xsZXIoJ0FjY29yZGlvbkNvbnRyb2xsZXInLCBbJyRzY29wZScsICckYXR0cnMnLCAnJGNvbnRyb2xsZXInLCAnJGxvZycsICckYWNjb3JkaW9uU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJHNjb3BlLCAkYXR0cnMsICRjb250cm9sbGVyLCAkbG9nLCAkYWNjb3JkaW9uU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgaWYgKCEkYWNjb3JkaW9uU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAkbG9nLndhcm4oJ0FjY29yZGlvbkNvbnRyb2xsZXIgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSBVaWJBY2NvcmRpb25Db250cm9sbGVyIGluc3RlYWQuJyk7XG4gICAgfVxuXG4gICAgYW5ndWxhci5leHRlbmQodGhpcywgJGNvbnRyb2xsZXIoJ1VpYkFjY29yZGlvbkNvbnRyb2xsZXInLCB7XG4gICAgICAkc2NvcGU6ICRzY29wZSxcbiAgICAgICRhdHRyczogJGF0dHJzXG4gICAgfSkpO1xuICB9XSlcblxuICAuZGlyZWN0aXZlKCdhY2NvcmRpb24nLCBbJyRsb2cnLCAnJGFjY29yZGlvblN1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRsb2csICRhY2NvcmRpb25TdXBwcmVzc1dhcm5pbmcpIHtcbiAgICByZXR1cm4ge1xuICAgICAgcmVzdHJpY3Q6ICdFQScsXG4gICAgICBjb250cm9sbGVyOiAnQWNjb3JkaW9uQ29udHJvbGxlcicsXG4gICAgICBjb250cm9sbGVyQXM6ICdhY2NvcmRpb24nLFxuICAgICAgdHJhbnNjbHVkZTogdHJ1ZSxcbiAgICAgIHJlcGxhY2U6IGZhbHNlLFxuICAgICAgdGVtcGxhdGVVcmw6IGZ1bmN0aW9uKGVsZW1lbnQsIGF0dHJzKSB7XG4gICAgICAgIHJldHVybiBhdHRycy50ZW1wbGF0ZVVybCB8fCAndGVtcGxhdGUvYWNjb3JkaW9uL2FjY29yZGlvbi5odG1sJztcbiAgICAgIH0sXG4gICAgICBsaW5rOiBmdW5jdGlvbigpIHtcbiAgICAgICAgaWYgKCEkYWNjb3JkaW9uU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICAgJGxvZy53YXJuKCdhY2NvcmRpb24gaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSB1aWItYWNjb3JkaW9uIGluc3RlYWQuJyk7XG4gICAgICAgIH1cbiAgICAgIH1cbiAgICB9O1xuICB9XSlcblxuICAuZGlyZWN0aXZlKCdhY2NvcmRpb25Hcm91cCcsIFsnJGxvZycsICckYWNjb3JkaW9uU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJGxvZywgJGFjY29yZGlvblN1cHByZXNzV2FybmluZykge1xuICAgIHJldHVybiB7XG4gICAgICByZXF1aXJlOiAnXmFjY29yZGlvbicsICAgICAgICAgLy8gV2UgbmVlZCB0aGlzIGRpcmVjdGl2ZSB0byBiZSBpbnNpZGUgYW4gYWNjb3JkaW9uXG4gICAgICByZXN0cmljdDogJ0VBJyxcbiAgICAgIHRyYW5zY2x1ZGU6IHRydWUsICAgICAgICAgICAgICAvLyBJdCB0cmFuc2NsdWRlcyB0aGUgY29udGVudHMgb2YgdGhlIGRpcmVjdGl2ZSBpbnRvIHRoZSB0ZW1wbGF0ZVxuICAgICAgcmVwbGFjZTogdHJ1ZSwgICAgICAgICAgICAgICAgLy8gVGhlIGVsZW1lbnQgY29udGFpbmluZyB0aGUgZGlyZWN0aXZlIHdpbGwgYmUgcmVwbGFjZWQgd2l0aCB0aGUgdGVtcGxhdGVcbiAgICAgIHRlbXBsYXRlVXJsOiBmdW5jdGlvbihlbGVtZW50LCBhdHRycykge1xuICAgICAgICByZXR1cm4gYXR0cnMudGVtcGxhdGVVcmwgfHwgJ3RlbXBsYXRlL2FjY29yZGlvbi9hY2NvcmRpb24tZ3JvdXAuaHRtbCc7XG4gICAgICB9LFxuICAgICAgc2NvcGU6IHtcbiAgICAgICAgaGVhZGluZzogJ0AnLCAgICAgICAgICAgICAgIC8vIEludGVycG9sYXRlIHRoZSBoZWFkaW5nIGF0dHJpYnV0ZSBvbnRvIHRoaXMgc2NvcGVcbiAgICAgICAgaXNPcGVuOiAnPT8nLFxuICAgICAgICBpc0Rpc2FibGVkOiAnPT8nXG4gICAgICB9LFxuICAgICAgY29udHJvbGxlcjogZnVuY3Rpb24oKSB7XG4gICAgICAgIHRoaXMuc2V0SGVhZGluZyA9IGZ1bmN0aW9uKGVsZW1lbnQpIHtcbiAgICAgICAgICB0aGlzLmhlYWRpbmcgPSBlbGVtZW50O1xuICAgICAgICB9O1xuICAgICAgfSxcbiAgICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgYWNjb3JkaW9uQ3RybCkge1xuICAgICAgICBpZiAoISRhY2NvcmRpb25TdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgICAkbG9nLndhcm4oJ2FjY29yZGlvbi1ncm91cCBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi1hY2NvcmRpb24tZ3JvdXAgaW5zdGVhZC4nKTtcbiAgICAgICAgfVxuXG4gICAgICAgIGFjY29yZGlvbkN0cmwuYWRkR3JvdXAoc2NvcGUpO1xuXG4gICAgICAgIHNjb3BlLm9wZW5DbGFzcyA9IGF0dHJzLm9wZW5DbGFzcyB8fCAncGFuZWwtb3Blbic7XG4gICAgICAgIHNjb3BlLnBhbmVsQ2xhc3MgPSBhdHRycy5wYW5lbENsYXNzO1xuICAgICAgICBzY29wZS4kd2F0Y2goJ2lzT3BlbicsIGZ1bmN0aW9uKHZhbHVlKSB7XG4gICAgICAgICAgZWxlbWVudC50b2dnbGVDbGFzcyhzY29wZS5vcGVuQ2xhc3MsICEhdmFsdWUpO1xuICAgICAgICAgIGlmICh2YWx1ZSkge1xuICAgICAgICAgICAgYWNjb3JkaW9uQ3RybC5jbG9zZU90aGVycyhzY29wZSk7XG4gICAgICAgICAgfVxuICAgICAgICB9KTtcblxuICAgICAgICBzY29wZS50b2dnbGVPcGVuID0gZnVuY3Rpb24oJGV2ZW50KSB7XG4gICAgICAgICAgaWYgKCFzY29wZS5pc0Rpc2FibGVkKSB7XG4gICAgICAgICAgICBpZiAoISRldmVudCB8fCAkZXZlbnQud2hpY2ggPT09IDMyKSB7XG4gICAgICAgICAgICAgIHNjb3BlLmlzT3BlbiA9ICFzY29wZS5pc09wZW47XG4gICAgICAgICAgICB9XG4gICAgICAgICAgfVxuICAgICAgICB9O1xuICAgICAgfVxuICAgIH07XG4gIH1dKVxuXG4gIC5kaXJlY3RpdmUoJ2FjY29yZGlvbkhlYWRpbmcnLCBbJyRsb2cnLCAnJGFjY29yZGlvblN1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRsb2csICRhY2NvcmRpb25TdXBwcmVzc1dhcm5pbmcpIHtcbiAgICByZXR1cm4ge1xuICAgICAgcmVzdHJpY3Q6ICdFQScsXG4gICAgICB0cmFuc2NsdWRlOiB0cnVlLCAgIC8vIEdyYWIgdGhlIGNvbnRlbnRzIHRvIGJlIHVzZWQgYXMgdGhlIGhlYWRpbmdcbiAgICAgIHRlbXBsYXRlOiAnJywgICAgICAgLy8gSW4gZWZmZWN0IHJlbW92ZSB0aGlzIGVsZW1lbnQhXG4gICAgICByZXBsYWNlOiB0cnVlLFxuICAgICAgcmVxdWlyZTogJ15hY2NvcmRpb25Hcm91cCcsXG4gICAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0ciwgYWNjb3JkaW9uR3JvdXBDdHJsLCB0cmFuc2NsdWRlKSB7XG4gICAgICAgIGlmICghJGFjY29yZGlvblN1cHByZXNzV2FybmluZykge1xuICAgICAgICAgICRsb2cud2FybignYWNjb3JkaW9uLWhlYWRpbmcgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSB1aWItYWNjb3JkaW9uLWhlYWRpbmcgaW5zdGVhZC4nKTtcbiAgICAgICAgfVxuICAgICAgICAvLyBQYXNzIHRoZSBoZWFkaW5nIHRvIHRoZSBhY2NvcmRpb24tZ3JvdXAgY29udHJvbGxlclxuICAgICAgICAvLyBzbyB0aGF0IGl0IGNhbiBiZSB0cmFuc2NsdWRlZCBpbnRvIHRoZSByaWdodCBwbGFjZSBpbiB0aGUgdGVtcGxhdGVcbiAgICAgICAgLy8gW1RoZSBzZWNvbmQgcGFyYW1ldGVyIHRvIHRyYW5zY2x1ZGUgY2F1c2VzIHRoZSBlbGVtZW50cyB0byBiZSBjbG9uZWQgc28gdGhhdCB0aGV5IHdvcmsgaW4gbmctcmVwZWF0XVxuICAgICAgICBhY2NvcmRpb25Hcm91cEN0cmwuc2V0SGVhZGluZyh0cmFuc2NsdWRlKHNjb3BlLCBhbmd1bGFyLm5vb3ApKTtcbiAgICAgIH1cbiAgICB9O1xuICB9XSlcblxuICAuZGlyZWN0aXZlKCdhY2NvcmRpb25UcmFuc2NsdWRlJywgWyckbG9nJywgJyRhY2NvcmRpb25TdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkbG9nLCAkYWNjb3JkaW9uU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgcmV0dXJuIHtcbiAgICAgIHJlcXVpcmU6ICdeYWNjb3JkaW9uR3JvdXAnLFxuICAgICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHIsIGNvbnRyb2xsZXIpIHtcbiAgICAgICAgaWYgKCEkYWNjb3JkaW9uU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICAgJGxvZy53YXJuKCdhY2NvcmRpb24tdHJhbnNjbHVkZSBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi1hY2NvcmRpb24tdHJhbnNjbHVkZSBpbnN0ZWFkLicpO1xuICAgICAgICB9XG5cbiAgICAgICAgc2NvcGUuJHdhdGNoKGZ1bmN0aW9uKCkgeyByZXR1cm4gY29udHJvbGxlclthdHRyLmFjY29yZGlvblRyYW5zY2x1ZGVdOyB9LCBmdW5jdGlvbihoZWFkaW5nKSB7XG4gICAgICAgICAgaWYgKGhlYWRpbmcpIHtcbiAgICAgICAgICAgIGVsZW1lbnQuZmluZCgnc3BhbicpLmh0bWwoJycpO1xuICAgICAgICAgICAgZWxlbWVudC5maW5kKCdzcGFuJykuYXBwZW5kKGhlYWRpbmcpO1xuICAgICAgICAgIH1cbiAgICAgICAgfSk7XG4gICAgICB9XG4gICAgfTtcbiAgfV0pO1xuXG5cbmFuZ3VsYXIubW9kdWxlKCd1aS5ib290c3RyYXAuYWxlcnQnLCBbXSlcblxuLmNvbnRyb2xsZXIoJ1VpYkFsZXJ0Q29udHJvbGxlcicsIFsnJHNjb3BlJywgJyRhdHRycycsICckaW50ZXJwb2xhdGUnLCAnJHRpbWVvdXQnLCBmdW5jdGlvbigkc2NvcGUsICRhdHRycywgJGludGVycG9sYXRlLCAkdGltZW91dCkge1xuICAkc2NvcGUuY2xvc2VhYmxlID0gISEkYXR0cnMuY2xvc2U7XG5cbiAgdmFyIGRpc21pc3NPblRpbWVvdXQgPSBhbmd1bGFyLmlzRGVmaW5lZCgkYXR0cnMuZGlzbWlzc09uVGltZW91dCkgP1xuICAgICRpbnRlcnBvbGF0ZSgkYXR0cnMuZGlzbWlzc09uVGltZW91dCkoJHNjb3BlLiRwYXJlbnQpIDogbnVsbDtcblxuICBpZiAoZGlzbWlzc09uVGltZW91dCkge1xuICAgICR0aW1lb3V0KGZ1bmN0aW9uKCkge1xuICAgICAgJHNjb3BlLmNsb3NlKCk7XG4gICAgfSwgcGFyc2VJbnQoZGlzbWlzc09uVGltZW91dCwgMTApKTtcbiAgfVxufV0pXG5cbi5kaXJlY3RpdmUoJ3VpYkFsZXJ0JywgZnVuY3Rpb24oKSB7XG4gIHJldHVybiB7XG4gICAgY29udHJvbGxlcjogJ1VpYkFsZXJ0Q29udHJvbGxlcicsXG4gICAgY29udHJvbGxlckFzOiAnYWxlcnQnLFxuICAgIHRlbXBsYXRlVXJsOiBmdW5jdGlvbihlbGVtZW50LCBhdHRycykge1xuICAgICAgcmV0dXJuIGF0dHJzLnRlbXBsYXRlVXJsIHx8ICd0ZW1wbGF0ZS9hbGVydC9hbGVydC5odG1sJztcbiAgICB9LFxuICAgIHRyYW5zY2x1ZGU6IHRydWUsXG4gICAgcmVwbGFjZTogdHJ1ZSxcbiAgICBzY29wZToge1xuICAgICAgdHlwZTogJ0AnLFxuICAgICAgY2xvc2U6ICcmJ1xuICAgIH1cbiAgfTtcbn0pO1xuXG4vKiBEZXByZWNhdGVkIGFsZXJ0IGJlbG93ICovXG5cbmFuZ3VsYXIubW9kdWxlKCd1aS5ib290c3RyYXAuYWxlcnQnKVxuXG4gIC52YWx1ZSgnJGFsZXJ0U3VwcHJlc3NXYXJuaW5nJywgZmFsc2UpXG5cbiAgLmNvbnRyb2xsZXIoJ0FsZXJ0Q29udHJvbGxlcicsIFsnJHNjb3BlJywgJyRhdHRycycsICckY29udHJvbGxlcicsICckbG9nJywgJyRhbGVydFN1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRzY29wZSwgJGF0dHJzLCAkY29udHJvbGxlciwgJGxvZywgJGFsZXJ0U3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgaWYgKCEkYWxlcnRTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICRsb2cud2FybignQWxlcnRDb250cm9sbGVyIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgVWliQWxlcnRDb250cm9sbGVyIGluc3RlYWQuJyk7XG4gICAgfVxuXG4gICAgYW5ndWxhci5leHRlbmQodGhpcywgJGNvbnRyb2xsZXIoJ1VpYkFsZXJ0Q29udHJvbGxlcicsIHtcbiAgICAgICRzY29wZTogJHNjb3BlLFxuICAgICAgJGF0dHJzOiAkYXR0cnNcbiAgICB9KSk7XG4gIH1dKVxuXG4gIC5kaXJlY3RpdmUoJ2FsZXJ0JywgWyckbG9nJywgJyRhbGVydFN1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRsb2csICRhbGVydFN1cHByZXNzV2FybmluZykge1xuICAgIHJldHVybiB7XG4gICAgICBjb250cm9sbGVyOiAnQWxlcnRDb250cm9sbGVyJyxcbiAgICAgIGNvbnRyb2xsZXJBczogJ2FsZXJ0JyxcbiAgICAgIHRlbXBsYXRlVXJsOiBmdW5jdGlvbihlbGVtZW50LCBhdHRycykge1xuICAgICAgICByZXR1cm4gYXR0cnMudGVtcGxhdGVVcmwgfHwgJ3RlbXBsYXRlL2FsZXJ0L2FsZXJ0Lmh0bWwnO1xuICAgICAgfSxcbiAgICAgIHRyYW5zY2x1ZGU6IHRydWUsXG4gICAgICByZXBsYWNlOiB0cnVlLFxuICAgICAgc2NvcGU6IHtcbiAgICAgICAgdHlwZTogJ0AnLFxuICAgICAgICBjbG9zZTogJyYnXG4gICAgICB9LFxuICAgICAgbGluazogZnVuY3Rpb24oKSB7XG4gICAgICAgIGlmICghJGFsZXJ0U3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICAgJGxvZy53YXJuKCdhbGVydCBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi1hbGVydCBpbnN0ZWFkLicpO1xuICAgICAgICB9XG4gICAgICB9XG4gICAgfTtcbiAgfV0pO1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLmJ1dHRvbnMnLCBbXSlcblxuLmNvbnN0YW50KCd1aWJCdXR0b25Db25maWcnLCB7XG4gIGFjdGl2ZUNsYXNzOiAnYWN0aXZlJyxcbiAgdG9nZ2xlRXZlbnQ6ICdjbGljaydcbn0pXG5cbi5jb250cm9sbGVyKCdVaWJCdXR0b25zQ29udHJvbGxlcicsIFsndWliQnV0dG9uQ29uZmlnJywgZnVuY3Rpb24oYnV0dG9uQ29uZmlnKSB7XG4gIHRoaXMuYWN0aXZlQ2xhc3MgPSBidXR0b25Db25maWcuYWN0aXZlQ2xhc3MgfHwgJ2FjdGl2ZSc7XG4gIHRoaXMudG9nZ2xlRXZlbnQgPSBidXR0b25Db25maWcudG9nZ2xlRXZlbnQgfHwgJ2NsaWNrJztcbn1dKVxuXG4uZGlyZWN0aXZlKCd1aWJCdG5SYWRpbycsIGZ1bmN0aW9uKCkge1xuICByZXR1cm4ge1xuICAgIHJlcXVpcmU6IFsndWliQnRuUmFkaW8nLCAnbmdNb2RlbCddLFxuICAgIGNvbnRyb2xsZXI6ICdVaWJCdXR0b25zQ29udHJvbGxlcicsXG4gICAgY29udHJvbGxlckFzOiAnYnV0dG9ucycsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHJzLCBjdHJscykge1xuICAgICAgdmFyIGJ1dHRvbnNDdHJsID0gY3RybHNbMF0sIG5nTW9kZWxDdHJsID0gY3RybHNbMV07XG5cbiAgICAgIGVsZW1lbnQuZmluZCgnaW5wdXQnKS5jc3Moe2Rpc3BsYXk6ICdub25lJ30pO1xuXG4gICAgICAvL21vZGVsIC0+IFVJXG4gICAgICBuZ01vZGVsQ3RybC4kcmVuZGVyID0gZnVuY3Rpb24oKSB7XG4gICAgICAgIGVsZW1lbnQudG9nZ2xlQ2xhc3MoYnV0dG9uc0N0cmwuYWN0aXZlQ2xhc3MsIGFuZ3VsYXIuZXF1YWxzKG5nTW9kZWxDdHJsLiRtb2RlbFZhbHVlLCBzY29wZS4kZXZhbChhdHRycy51aWJCdG5SYWRpbykpKTtcbiAgICAgIH07XG5cbiAgICAgIC8vdWktPm1vZGVsXG4gICAgICBlbGVtZW50Lm9uKGJ1dHRvbnNDdHJsLnRvZ2dsZUV2ZW50LCBmdW5jdGlvbigpIHtcbiAgICAgICAgaWYgKGF0dHJzLmRpc2FibGVkKSB7XG4gICAgICAgICAgcmV0dXJuO1xuICAgICAgICB9XG5cbiAgICAgICAgdmFyIGlzQWN0aXZlID0gZWxlbWVudC5oYXNDbGFzcyhidXR0b25zQ3RybC5hY3RpdmVDbGFzcyk7XG5cbiAgICAgICAgaWYgKCFpc0FjdGl2ZSB8fCBhbmd1bGFyLmlzRGVmaW5lZChhdHRycy51bmNoZWNrYWJsZSkpIHtcbiAgICAgICAgICBzY29wZS4kYXBwbHkoZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICBuZ01vZGVsQ3RybC4kc2V0Vmlld1ZhbHVlKGlzQWN0aXZlID8gbnVsbCA6IHNjb3BlLiRldmFsKGF0dHJzLnVpYkJ0blJhZGlvKSk7XG4gICAgICAgICAgICBuZ01vZGVsQ3RybC4kcmVuZGVyKCk7XG4gICAgICAgICAgfSk7XG4gICAgICAgIH1cbiAgICAgIH0pO1xuICAgIH1cbiAgfTtcbn0pXG5cbi5kaXJlY3RpdmUoJ3VpYkJ0bkNoZWNrYm94JywgZnVuY3Rpb24oKSB7XG4gIHJldHVybiB7XG4gICAgcmVxdWlyZTogWyd1aWJCdG5DaGVja2JveCcsICduZ01vZGVsJ10sXG4gICAgY29udHJvbGxlcjogJ1VpYkJ1dHRvbnNDb250cm9sbGVyJyxcbiAgICBjb250cm9sbGVyQXM6ICdidXR0b24nLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgY3RybHMpIHtcbiAgICAgIHZhciBidXR0b25zQ3RybCA9IGN0cmxzWzBdLCBuZ01vZGVsQ3RybCA9IGN0cmxzWzFdO1xuXG4gICAgICBlbGVtZW50LmZpbmQoJ2lucHV0JykuY3NzKHtkaXNwbGF5OiAnbm9uZSd9KTtcblxuICAgICAgZnVuY3Rpb24gZ2V0VHJ1ZVZhbHVlKCkge1xuICAgICAgICByZXR1cm4gZ2V0Q2hlY2tib3hWYWx1ZShhdHRycy5idG5DaGVja2JveFRydWUsIHRydWUpO1xuICAgICAgfVxuXG4gICAgICBmdW5jdGlvbiBnZXRGYWxzZVZhbHVlKCkge1xuICAgICAgICByZXR1cm4gZ2V0Q2hlY2tib3hWYWx1ZShhdHRycy5idG5DaGVja2JveEZhbHNlLCBmYWxzZSk7XG4gICAgICB9XG5cbiAgICAgIGZ1bmN0aW9uIGdldENoZWNrYm94VmFsdWUoYXR0cmlidXRlLCBkZWZhdWx0VmFsdWUpIHtcbiAgICAgICAgcmV0dXJuIGFuZ3VsYXIuaXNEZWZpbmVkKGF0dHJpYnV0ZSkgPyBzY29wZS4kZXZhbChhdHRyaWJ1dGUpIDogZGVmYXVsdFZhbHVlO1xuICAgICAgfVxuXG4gICAgICAvL21vZGVsIC0+IFVJXG4gICAgICBuZ01vZGVsQ3RybC4kcmVuZGVyID0gZnVuY3Rpb24oKSB7XG4gICAgICAgIGVsZW1lbnQudG9nZ2xlQ2xhc3MoYnV0dG9uc0N0cmwuYWN0aXZlQ2xhc3MsIGFuZ3VsYXIuZXF1YWxzKG5nTW9kZWxDdHJsLiRtb2RlbFZhbHVlLCBnZXRUcnVlVmFsdWUoKSkpO1xuICAgICAgfTtcblxuICAgICAgLy91aS0+bW9kZWxcbiAgICAgIGVsZW1lbnQub24oYnV0dG9uc0N0cmwudG9nZ2xlRXZlbnQsIGZ1bmN0aW9uKCkge1xuICAgICAgICBpZiAoYXR0cnMuZGlzYWJsZWQpIHtcbiAgICAgICAgICByZXR1cm47XG4gICAgICAgIH1cblxuICAgICAgICBzY29wZS4kYXBwbHkoZnVuY3Rpb24oKSB7XG4gICAgICAgICAgbmdNb2RlbEN0cmwuJHNldFZpZXdWYWx1ZShlbGVtZW50Lmhhc0NsYXNzKGJ1dHRvbnNDdHJsLmFjdGl2ZUNsYXNzKSA/IGdldEZhbHNlVmFsdWUoKSA6IGdldFRydWVWYWx1ZSgpKTtcbiAgICAgICAgICBuZ01vZGVsQ3RybC4kcmVuZGVyKCk7XG4gICAgICAgIH0pO1xuICAgICAgfSk7XG4gICAgfVxuICB9O1xufSk7XG5cbi8qIERlcHJlY2F0ZWQgYnV0dG9ucyBiZWxvdyAqL1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLmJ1dHRvbnMnKVxuXG4gIC52YWx1ZSgnJGJ1dHRvbnNTdXBwcmVzc1dhcm5pbmcnLCBmYWxzZSlcblxuICAuY29udHJvbGxlcignQnV0dG9uc0NvbnRyb2xsZXInLCBbJyRjb250cm9sbGVyJywgJyRsb2cnLCAnJGJ1dHRvbnNTdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkY29udHJvbGxlciwgJGxvZywgJGJ1dHRvbnNTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICBpZiAoISRidXR0b25zU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAkbG9nLndhcm4oJ0J1dHRvbnNDb250cm9sbGVyIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgVWliQnV0dG9uc0NvbnRyb2xsZXIgaW5zdGVhZC4nKTtcbiAgICB9XG5cbiAgICBhbmd1bGFyLmV4dGVuZCh0aGlzLCAkY29udHJvbGxlcignVWliQnV0dG9uc0NvbnRyb2xsZXInKSk7XG4gIH1dKVxuXG4gIC5kaXJlY3RpdmUoJ2J0blJhZGlvJywgWyckbG9nJywgJyRidXR0b25zU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJGxvZywgJGJ1dHRvbnNTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICByZXR1cm4ge1xuICAgICAgcmVxdWlyZTogWydidG5SYWRpbycsICduZ01vZGVsJ10sXG4gICAgICBjb250cm9sbGVyOiAnQnV0dG9uc0NvbnRyb2xsZXInLFxuICAgICAgY29udHJvbGxlckFzOiAnYnV0dG9ucycsXG4gICAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMsIGN0cmxzKSB7XG4gICAgICAgIGlmICghJGJ1dHRvbnNTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgICAkbG9nLndhcm4oJ2J0bi1yYWRpbyBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi1idG4tcmFkaW8gaW5zdGVhZC4nKTtcbiAgICAgICAgfVxuXG4gICAgICAgIHZhciBidXR0b25zQ3RybCA9IGN0cmxzWzBdLCBuZ01vZGVsQ3RybCA9IGN0cmxzWzFdO1xuXG4gICAgICAgIGVsZW1lbnQuZmluZCgnaW5wdXQnKS5jc3Moe2Rpc3BsYXk6ICdub25lJ30pO1xuXG4gICAgICAgIC8vbW9kZWwgLT4gVUlcbiAgICAgICAgbmdNb2RlbEN0cmwuJHJlbmRlciA9IGZ1bmN0aW9uKCkge1xuICAgICAgICAgIGVsZW1lbnQudG9nZ2xlQ2xhc3MoYnV0dG9uc0N0cmwuYWN0aXZlQ2xhc3MsIGFuZ3VsYXIuZXF1YWxzKG5nTW9kZWxDdHJsLiRtb2RlbFZhbHVlLCBzY29wZS4kZXZhbChhdHRycy5idG5SYWRpbykpKTtcbiAgICAgICAgfTtcblxuICAgICAgICAvL3VpLT5tb2RlbFxuICAgICAgICBlbGVtZW50LmJpbmQoYnV0dG9uc0N0cmwudG9nZ2xlRXZlbnQsIGZ1bmN0aW9uKCkge1xuICAgICAgICAgIGlmIChhdHRycy5kaXNhYmxlZCkge1xuICAgICAgICAgICAgcmV0dXJuO1xuICAgICAgICAgIH1cblxuICAgICAgICAgIHZhciBpc0FjdGl2ZSA9IGVsZW1lbnQuaGFzQ2xhc3MoYnV0dG9uc0N0cmwuYWN0aXZlQ2xhc3MpO1xuXG4gICAgICAgICAgaWYgKCFpc0FjdGl2ZSB8fCBhbmd1bGFyLmlzRGVmaW5lZChhdHRycy51bmNoZWNrYWJsZSkpIHtcbiAgICAgICAgICAgIHNjb3BlLiRhcHBseShmdW5jdGlvbigpIHtcbiAgICAgICAgICAgICAgbmdNb2RlbEN0cmwuJHNldFZpZXdWYWx1ZShpc0FjdGl2ZSA/IG51bGwgOiBzY29wZS4kZXZhbChhdHRycy5idG5SYWRpbykpO1xuICAgICAgICAgICAgICBuZ01vZGVsQ3RybC4kcmVuZGVyKCk7XG4gICAgICAgICAgICB9KTtcbiAgICAgICAgICB9XG4gICAgICAgIH0pO1xuICAgICAgfVxuICAgIH07XG4gIH1dKVxuXG4gIC5kaXJlY3RpdmUoJ2J0bkNoZWNrYm94JywgWyckZG9jdW1lbnQnLCAnJGxvZycsICckYnV0dG9uc1N1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRkb2N1bWVudCwgJGxvZywgJGJ1dHRvbnNTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICByZXR1cm4ge1xuICAgICAgcmVxdWlyZTogWydidG5DaGVja2JveCcsICduZ01vZGVsJ10sXG4gICAgICBjb250cm9sbGVyOiAnQnV0dG9uc0NvbnRyb2xsZXInLFxuICAgICAgY29udHJvbGxlckFzOiAnYnV0dG9uJyxcbiAgICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgY3RybHMpIHtcbiAgICAgICAgaWYgKCEkYnV0dG9uc1N1cHByZXNzV2FybmluZykge1xuICAgICAgICAgICRsb2cud2FybignYnRuLWNoZWNrYm94IGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliLWJ0bi1jaGVja2JveCBpbnN0ZWFkLicpO1xuICAgICAgICB9XG5cbiAgICAgICAgdmFyIGJ1dHRvbnNDdHJsID0gY3RybHNbMF0sIG5nTW9kZWxDdHJsID0gY3RybHNbMV07XG5cbiAgICAgICAgZWxlbWVudC5maW5kKCdpbnB1dCcpLmNzcyh7ZGlzcGxheTogJ25vbmUnfSk7XG5cbiAgICAgICAgZnVuY3Rpb24gZ2V0VHJ1ZVZhbHVlKCkge1xuICAgICAgICAgIHJldHVybiBnZXRDaGVja2JveFZhbHVlKGF0dHJzLmJ0bkNoZWNrYm94VHJ1ZSwgdHJ1ZSk7XG4gICAgICAgIH1cblxuICAgICAgICBmdW5jdGlvbiBnZXRGYWxzZVZhbHVlKCkge1xuICAgICAgICAgIHJldHVybiBnZXRDaGVja2JveFZhbHVlKGF0dHJzLmJ0bkNoZWNrYm94RmFsc2UsIGZhbHNlKTtcbiAgICAgICAgfVxuXG4gICAgICAgIGZ1bmN0aW9uIGdldENoZWNrYm94VmFsdWUoYXR0cmlidXRlVmFsdWUsIGRlZmF1bHRWYWx1ZSkge1xuICAgICAgICAgIHZhciB2YWwgPSBzY29wZS4kZXZhbChhdHRyaWJ1dGVWYWx1ZSk7XG4gICAgICAgICAgcmV0dXJuIGFuZ3VsYXIuaXNEZWZpbmVkKHZhbCkgPyB2YWwgOiBkZWZhdWx0VmFsdWU7XG4gICAgICAgIH1cblxuICAgICAgICAvL21vZGVsIC0+IFVJXG4gICAgICAgIG5nTW9kZWxDdHJsLiRyZW5kZXIgPSBmdW5jdGlvbigpIHtcbiAgICAgICAgICBlbGVtZW50LnRvZ2dsZUNsYXNzKGJ1dHRvbnNDdHJsLmFjdGl2ZUNsYXNzLCBhbmd1bGFyLmVxdWFscyhuZ01vZGVsQ3RybC4kbW9kZWxWYWx1ZSwgZ2V0VHJ1ZVZhbHVlKCkpKTtcbiAgICAgICAgfTtcblxuICAgICAgICAvL3VpLT5tb2RlbFxuICAgICAgICBlbGVtZW50LmJpbmQoYnV0dG9uc0N0cmwudG9nZ2xlRXZlbnQsIGZ1bmN0aW9uKCkge1xuICAgICAgICAgIGlmIChhdHRycy5kaXNhYmxlZCkge1xuICAgICAgICAgICAgcmV0dXJuO1xuICAgICAgICAgIH1cblxuICAgICAgICAgIHNjb3BlLiRhcHBseShmdW5jdGlvbigpIHtcbiAgICAgICAgICAgIG5nTW9kZWxDdHJsLiRzZXRWaWV3VmFsdWUoZWxlbWVudC5oYXNDbGFzcyhidXR0b25zQ3RybC5hY3RpdmVDbGFzcykgPyBnZXRGYWxzZVZhbHVlKCkgOiBnZXRUcnVlVmFsdWUoKSk7XG4gICAgICAgICAgICBuZ01vZGVsQ3RybC4kcmVuZGVyKCk7XG4gICAgICAgICAgfSk7XG4gICAgICAgIH0pO1xuXG4gICAgICAgIC8vYWNjZXNzaWJpbGl0eVxuICAgICAgICBlbGVtZW50Lm9uKCdrZXlwcmVzcycsIGZ1bmN0aW9uKGUpIHtcbiAgICAgICAgICBpZiAoYXR0cnMuZGlzYWJsZWQgfHwgZS53aGljaCAhPT0gMzIgfHwgJGRvY3VtZW50WzBdLmFjdGl2ZUVsZW1lbnQgIT09IGVsZW1lbnRbMF0pIHtcbiAgICAgICAgICAgIHJldHVybjtcbiAgICAgICAgICB9XG5cbiAgICAgICAgICBzY29wZS4kYXBwbHkoZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICBuZ01vZGVsQ3RybC4kc2V0Vmlld1ZhbHVlKGVsZW1lbnQuaGFzQ2xhc3MoYnV0dG9uc0N0cmwuYWN0aXZlQ2xhc3MpID8gZ2V0RmFsc2VWYWx1ZSgpIDogZ2V0VHJ1ZVZhbHVlKCkpO1xuICAgICAgICAgICAgbmdNb2RlbEN0cmwuJHJlbmRlcigpO1xuICAgICAgICAgIH0pO1xuICAgICAgICB9KTtcbiAgICAgIH1cbiAgICB9O1xuICB9XSk7XG5cblxuLyoqXG4gKiBAbmdkb2Mgb3ZlcnZpZXdcbiAqIEBuYW1lIHVpLmJvb3RzdHJhcC5jYXJvdXNlbFxuICpcbiAqIEBkZXNjcmlwdGlvblxuICogQW5ndWxhckpTIHZlcnNpb24gb2YgYW4gaW1hZ2UgY2Fyb3VzZWwuXG4gKlxuICovXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLmNhcm91c2VsJywgW10pXG5cbi5jb250cm9sbGVyKCdVaWJDYXJvdXNlbENvbnRyb2xsZXInLCBbJyRzY29wZScsICckZWxlbWVudCcsICckaW50ZXJ2YWwnLCAnJGFuaW1hdGUnLCBmdW5jdGlvbigkc2NvcGUsICRlbGVtZW50LCAkaW50ZXJ2YWwsICRhbmltYXRlKSB7XG4gIHZhciBzZWxmID0gdGhpcyxcbiAgICBzbGlkZXMgPSBzZWxmLnNsaWRlcyA9ICRzY29wZS5zbGlkZXMgPSBbXSxcbiAgICBORVdfQU5JTUFURSA9IGFuZ3VsYXIudmVyc2lvbi5taW5vciA+PSA0LFxuICAgIE5PX1RSQU5TSVRJT04gPSAndWliLW5vVHJhbnNpdGlvbicsXG4gICAgU0xJREVfRElSRUNUSU9OID0gJ3VpYi1zbGlkZURpcmVjdGlvbicsXG4gICAgY3VycmVudEluZGV4ID0gLTEsXG4gICAgY3VycmVudEludGVydmFsLCBpc1BsYXlpbmc7XG4gIHNlbGYuY3VycmVudFNsaWRlID0gbnVsbDtcblxuICB2YXIgZGVzdHJveWVkID0gZmFsc2U7XG4gIC8qIGRpcmVjdGlvbjogXCJwcmV2XCIgb3IgXCJuZXh0XCIgKi9cbiAgc2VsZi5zZWxlY3QgPSAkc2NvcGUuc2VsZWN0ID0gZnVuY3Rpb24obmV4dFNsaWRlLCBkaXJlY3Rpb24pIHtcbiAgICB2YXIgbmV4dEluZGV4ID0gJHNjb3BlLmluZGV4T2ZTbGlkZShuZXh0U2xpZGUpO1xuICAgIC8vRGVjaWRlIGRpcmVjdGlvbiBpZiBpdCdzIG5vdCBnaXZlblxuICAgIGlmIChkaXJlY3Rpb24gPT09IHVuZGVmaW5lZCkge1xuICAgICAgZGlyZWN0aW9uID0gbmV4dEluZGV4ID4gc2VsZi5nZXRDdXJyZW50SW5kZXgoKSA/ICduZXh0JyA6ICdwcmV2JztcbiAgICB9XG4gICAgLy9QcmV2ZW50IHRoaXMgdXNlci10cmlnZ2VyZWQgdHJhbnNpdGlvbiBmcm9tIG9jY3VycmluZyBpZiB0aGVyZSBpcyBhbHJlYWR5IG9uZSBpbiBwcm9ncmVzc1xuICAgIGlmIChuZXh0U2xpZGUgJiYgbmV4dFNsaWRlICE9PSBzZWxmLmN1cnJlbnRTbGlkZSAmJiAhJHNjb3BlLiRjdXJyZW50VHJhbnNpdGlvbikge1xuICAgICAgZ29OZXh0KG5leHRTbGlkZSwgbmV4dEluZGV4LCBkaXJlY3Rpb24pO1xuICAgIH1cbiAgfTtcblxuICBmdW5jdGlvbiBnb05leHQoc2xpZGUsIGluZGV4LCBkaXJlY3Rpb24pIHtcbiAgICAvLyBTY29wZSBoYXMgYmVlbiBkZXN0cm95ZWQsIHN0b3AgaGVyZS5cbiAgICBpZiAoZGVzdHJveWVkKSB7IHJldHVybjsgfVxuXG4gICAgYW5ndWxhci5leHRlbmQoc2xpZGUsIHtkaXJlY3Rpb246IGRpcmVjdGlvbiwgYWN0aXZlOiB0cnVlfSk7XG4gICAgYW5ndWxhci5leHRlbmQoc2VsZi5jdXJyZW50U2xpZGUgfHwge30sIHtkaXJlY3Rpb246IGRpcmVjdGlvbiwgYWN0aXZlOiBmYWxzZX0pO1xuICAgIGlmICgkYW5pbWF0ZS5lbmFibGVkKCkgJiYgISRzY29wZS5ub1RyYW5zaXRpb24gJiYgISRzY29wZS4kY3VycmVudFRyYW5zaXRpb24gJiZcbiAgICAgIHNsaWRlLiRlbGVtZW50ICYmIHNlbGYuc2xpZGVzLmxlbmd0aCA+IDEpIHtcbiAgICAgIHNsaWRlLiRlbGVtZW50LmRhdGEoU0xJREVfRElSRUNUSU9OLCBzbGlkZS5kaXJlY3Rpb24pO1xuICAgICAgaWYgKHNlbGYuY3VycmVudFNsaWRlICYmIHNlbGYuY3VycmVudFNsaWRlLiRlbGVtZW50KSB7XG4gICAgICAgIHNlbGYuY3VycmVudFNsaWRlLiRlbGVtZW50LmRhdGEoU0xJREVfRElSRUNUSU9OLCBzbGlkZS5kaXJlY3Rpb24pO1xuICAgICAgfVxuXG4gICAgICAkc2NvcGUuJGN1cnJlbnRUcmFuc2l0aW9uID0gdHJ1ZTtcbiAgICAgIGlmIChORVdfQU5JTUFURSkge1xuICAgICAgICAkYW5pbWF0ZS5vbignYWRkQ2xhc3MnLCBzbGlkZS4kZWxlbWVudCwgZnVuY3Rpb24oZWxlbWVudCwgcGhhc2UpIHtcbiAgICAgICAgICBpZiAocGhhc2UgPT09ICdjbG9zZScpIHtcbiAgICAgICAgICAgICRzY29wZS4kY3VycmVudFRyYW5zaXRpb24gPSBudWxsO1xuICAgICAgICAgICAgJGFuaW1hdGUub2ZmKCdhZGRDbGFzcycsIGVsZW1lbnQpO1xuICAgICAgICAgIH1cbiAgICAgICAgfSk7XG4gICAgICB9IGVsc2Uge1xuICAgICAgICBzbGlkZS4kZWxlbWVudC5vbmUoJyRhbmltYXRlOmNsb3NlJywgZnVuY3Rpb24gY2xvc2VGbigpIHtcbiAgICAgICAgICAkc2NvcGUuJGN1cnJlbnRUcmFuc2l0aW9uID0gbnVsbDtcbiAgICAgICAgfSk7XG4gICAgICB9XG4gICAgfVxuXG4gICAgc2VsZi5jdXJyZW50U2xpZGUgPSBzbGlkZTtcbiAgICBjdXJyZW50SW5kZXggPSBpbmRleDtcblxuICAgIC8vZXZlcnkgdGltZSB5b3UgY2hhbmdlIHNsaWRlcywgcmVzZXQgdGhlIHRpbWVyXG4gICAgcmVzdGFydFRpbWVyKCk7XG4gIH1cblxuICAkc2NvcGUuJG9uKCckZGVzdHJveScsIGZ1bmN0aW9uKCkge1xuICAgIGRlc3Ryb3llZCA9IHRydWU7XG4gIH0pO1xuXG4gIGZ1bmN0aW9uIGdldFNsaWRlQnlJbmRleChpbmRleCkge1xuICAgIGlmIChhbmd1bGFyLmlzVW5kZWZpbmVkKHNsaWRlc1tpbmRleF0uaW5kZXgpKSB7XG4gICAgICByZXR1cm4gc2xpZGVzW2luZGV4XTtcbiAgICB9XG4gICAgdmFyIGksIGxlbiA9IHNsaWRlcy5sZW5ndGg7XG4gICAgZm9yIChpID0gMDsgaSA8IHNsaWRlcy5sZW5ndGg7ICsraSkge1xuICAgICAgaWYgKHNsaWRlc1tpXS5pbmRleCA9PSBpbmRleCkge1xuICAgICAgICByZXR1cm4gc2xpZGVzW2ldO1xuICAgICAgfVxuICAgIH1cbiAgfVxuXG4gIHNlbGYuZ2V0Q3VycmVudEluZGV4ID0gZnVuY3Rpb24oKSB7XG4gICAgaWYgKHNlbGYuY3VycmVudFNsaWRlICYmIGFuZ3VsYXIuaXNEZWZpbmVkKHNlbGYuY3VycmVudFNsaWRlLmluZGV4KSkge1xuICAgICAgcmV0dXJuICtzZWxmLmN1cnJlbnRTbGlkZS5pbmRleDtcbiAgICB9XG4gICAgcmV0dXJuIGN1cnJlbnRJbmRleDtcbiAgfTtcblxuICAvKiBBbGxvdyBvdXRzaWRlIHBlb3BsZSB0byBjYWxsIGluZGV4T2Ygb24gc2xpZGVzIGFycmF5ICovXG4gICRzY29wZS5pbmRleE9mU2xpZGUgPSBmdW5jdGlvbihzbGlkZSkge1xuICAgIHJldHVybiBhbmd1bGFyLmlzRGVmaW5lZChzbGlkZS5pbmRleCkgPyArc2xpZGUuaW5kZXggOiBzbGlkZXMuaW5kZXhPZihzbGlkZSk7XG4gIH07XG5cbiAgJHNjb3BlLm5leHQgPSBmdW5jdGlvbigpIHtcbiAgICB2YXIgbmV3SW5kZXggPSAoc2VsZi5nZXRDdXJyZW50SW5kZXgoKSArIDEpICUgc2xpZGVzLmxlbmd0aDtcblxuICAgIGlmIChuZXdJbmRleCA9PT0gMCAmJiAkc2NvcGUubm9XcmFwKCkpIHtcbiAgICAgICRzY29wZS5wYXVzZSgpO1xuICAgICAgcmV0dXJuO1xuICAgIH1cblxuICAgIHJldHVybiBzZWxmLnNlbGVjdChnZXRTbGlkZUJ5SW5kZXgobmV3SW5kZXgpLCAnbmV4dCcpO1xuICB9O1xuXG4gICRzY29wZS5wcmV2ID0gZnVuY3Rpb24oKSB7XG4gICAgdmFyIG5ld0luZGV4ID0gc2VsZi5nZXRDdXJyZW50SW5kZXgoKSAtIDEgPCAwID8gc2xpZGVzLmxlbmd0aCAtIDEgOiBzZWxmLmdldEN1cnJlbnRJbmRleCgpIC0gMTtcblxuICAgIGlmICgkc2NvcGUubm9XcmFwKCkgJiYgbmV3SW5kZXggPT09IHNsaWRlcy5sZW5ndGggLSAxKSB7XG4gICAgICAkc2NvcGUucGF1c2UoKTtcbiAgICAgIHJldHVybjtcbiAgICB9XG5cbiAgICByZXR1cm4gc2VsZi5zZWxlY3QoZ2V0U2xpZGVCeUluZGV4KG5ld0luZGV4KSwgJ3ByZXYnKTtcbiAgfTtcblxuICAkc2NvcGUuaXNBY3RpdmUgPSBmdW5jdGlvbihzbGlkZSkge1xuICAgICByZXR1cm4gc2VsZi5jdXJyZW50U2xpZGUgPT09IHNsaWRlO1xuICB9O1xuXG4gICRzY29wZS4kd2F0Y2goJ2ludGVydmFsJywgcmVzdGFydFRpbWVyKTtcbiAgJHNjb3BlLiR3YXRjaENvbGxlY3Rpb24oJ3NsaWRlcycsIHJlc2V0VHJhbnNpdGlvbik7XG4gICRzY29wZS4kb24oJyRkZXN0cm95JywgcmVzZXRUaW1lcik7XG5cbiAgZnVuY3Rpb24gcmVzdGFydFRpbWVyKCkge1xuICAgIHJlc2V0VGltZXIoKTtcbiAgICB2YXIgaW50ZXJ2YWwgPSArJHNjb3BlLmludGVydmFsO1xuICAgIGlmICghaXNOYU4oaW50ZXJ2YWwpICYmIGludGVydmFsID4gMCkge1xuICAgICAgY3VycmVudEludGVydmFsID0gJGludGVydmFsKHRpbWVyRm4sIGludGVydmFsKTtcbiAgICB9XG4gIH1cblxuICBmdW5jdGlvbiByZXNldFRpbWVyKCkge1xuICAgIGlmIChjdXJyZW50SW50ZXJ2YWwpIHtcbiAgICAgICRpbnRlcnZhbC5jYW5jZWwoY3VycmVudEludGVydmFsKTtcbiAgICAgIGN1cnJlbnRJbnRlcnZhbCA9IG51bGw7XG4gICAgfVxuICB9XG5cbiAgZnVuY3Rpb24gdGltZXJGbigpIHtcbiAgICB2YXIgaW50ZXJ2YWwgPSArJHNjb3BlLmludGVydmFsO1xuICAgIGlmIChpc1BsYXlpbmcgJiYgIWlzTmFOKGludGVydmFsKSAmJiBpbnRlcnZhbCA+IDAgJiYgc2xpZGVzLmxlbmd0aCkge1xuICAgICAgJHNjb3BlLm5leHQoKTtcbiAgICB9IGVsc2Uge1xuICAgICAgJHNjb3BlLnBhdXNlKCk7XG4gICAgfVxuICB9XG5cbiAgZnVuY3Rpb24gcmVzZXRUcmFuc2l0aW9uKHNsaWRlcykge1xuICAgIGlmICghc2xpZGVzLmxlbmd0aCkge1xuICAgICAgJHNjb3BlLiRjdXJyZW50VHJhbnNpdGlvbiA9IG51bGw7XG4gICAgfVxuICB9XG5cbiAgJHNjb3BlLnBsYXkgPSBmdW5jdGlvbigpIHtcbiAgICBpZiAoIWlzUGxheWluZykge1xuICAgICAgaXNQbGF5aW5nID0gdHJ1ZTtcbiAgICAgIHJlc3RhcnRUaW1lcigpO1xuICAgIH1cbiAgfTtcbiAgJHNjb3BlLnBhdXNlID0gZnVuY3Rpb24oKSB7XG4gICAgaWYgKCEkc2NvcGUubm9QYXVzZSkge1xuICAgICAgaXNQbGF5aW5nID0gZmFsc2U7XG4gICAgICByZXNldFRpbWVyKCk7XG4gICAgfVxuICB9O1xuXG4gIHNlbGYuYWRkU2xpZGUgPSBmdW5jdGlvbihzbGlkZSwgZWxlbWVudCkge1xuICAgIHNsaWRlLiRlbGVtZW50ID0gZWxlbWVudDtcbiAgICBzbGlkZXMucHVzaChzbGlkZSk7XG4gICAgLy9pZiB0aGlzIGlzIHRoZSBmaXJzdCBzbGlkZSBvciB0aGUgc2xpZGUgaXMgc2V0IHRvIGFjdGl2ZSwgc2VsZWN0IGl0XG4gICAgaWYgKHNsaWRlcy5sZW5ndGggPT09IDEgfHwgc2xpZGUuYWN0aXZlKSB7XG4gICAgICBzZWxmLnNlbGVjdChzbGlkZXNbc2xpZGVzLmxlbmd0aCAtIDFdKTtcbiAgICAgIGlmIChzbGlkZXMubGVuZ3RoID09PSAxKSB7XG4gICAgICAgICRzY29wZS5wbGF5KCk7XG4gICAgICB9XG4gICAgfSBlbHNlIHtcbiAgICAgIHNsaWRlLmFjdGl2ZSA9IGZhbHNlO1xuICAgIH1cbiAgfTtcblxuICBzZWxmLnJlbW92ZVNsaWRlID0gZnVuY3Rpb24oc2xpZGUpIHtcbiAgICBpZiAoYW5ndWxhci5pc0RlZmluZWQoc2xpZGUuaW5kZXgpKSB7XG4gICAgICBzbGlkZXMuc29ydChmdW5jdGlvbihhLCBiKSB7XG4gICAgICAgIHJldHVybiArYS5pbmRleCA+ICtiLmluZGV4O1xuICAgICAgfSk7XG4gICAgfVxuICAgIC8vZ2V0IHRoZSBpbmRleCBvZiB0aGUgc2xpZGUgaW5zaWRlIHRoZSBjYXJvdXNlbFxuICAgIHZhciBpbmRleCA9IHNsaWRlcy5pbmRleE9mKHNsaWRlKTtcbiAgICBzbGlkZXMuc3BsaWNlKGluZGV4LCAxKTtcbiAgICBpZiAoc2xpZGVzLmxlbmd0aCA+IDAgJiYgc2xpZGUuYWN0aXZlKSB7XG4gICAgICBpZiAoaW5kZXggPj0gc2xpZGVzLmxlbmd0aCkge1xuICAgICAgICBzZWxmLnNlbGVjdChzbGlkZXNbaW5kZXggLSAxXSk7XG4gICAgICB9IGVsc2Uge1xuICAgICAgICBzZWxmLnNlbGVjdChzbGlkZXNbaW5kZXhdKTtcbiAgICAgIH1cbiAgICB9IGVsc2UgaWYgKGN1cnJlbnRJbmRleCA+IGluZGV4KSB7XG4gICAgICBjdXJyZW50SW5kZXgtLTtcbiAgICB9XG5cbiAgICAvL2NsZWFuIHRoZSBjdXJyZW50U2xpZGUgd2hlbiBubyBtb3JlIHNsaWRlXG4gICAgaWYgKHNsaWRlcy5sZW5ndGggPT09IDApIHtcbiAgICAgIHNlbGYuY3VycmVudFNsaWRlID0gbnVsbDtcbiAgICB9XG4gIH07XG5cbiAgJHNjb3BlLiR3YXRjaCgnbm9UcmFuc2l0aW9uJywgZnVuY3Rpb24obm9UcmFuc2l0aW9uKSB7XG4gICAgJGVsZW1lbnQuZGF0YShOT19UUkFOU0lUSU9OLCBub1RyYW5zaXRpb24pO1xuICB9KTtcblxufV0pXG5cbi8qKlxuICogQG5nZG9jIGRpcmVjdGl2ZVxuICogQG5hbWUgdWkuYm9vdHN0cmFwLmNhcm91c2VsLmRpcmVjdGl2ZTpjYXJvdXNlbFxuICogQHJlc3RyaWN0IEVBXG4gKlxuICogQGRlc2NyaXB0aW9uXG4gKiBDYXJvdXNlbCBpcyB0aGUgb3V0ZXIgY29udGFpbmVyIGZvciBhIHNldCBvZiBpbWFnZSAnc2xpZGVzJyB0byBzaG93Y2FzZS5cbiAqXG4gKiBAcGFyYW0ge251bWJlcj19IGludGVydmFsIFRoZSB0aW1lLCBpbiBtaWxsaXNlY29uZHMsIHRoYXQgaXQgd2lsbCB0YWtlIHRoZSBjYXJvdXNlbCB0byBnbyB0byB0aGUgbmV4dCBzbGlkZS5cbiAqIEBwYXJhbSB7Ym9vbGVhbj19IG5vVHJhbnNpdGlvbiBXaGV0aGVyIHRvIGRpc2FibGUgdHJhbnNpdGlvbnMgb24gdGhlIGNhcm91c2VsLlxuICogQHBhcmFtIHtib29sZWFuPX0gbm9QYXVzZSBXaGV0aGVyIHRvIGRpc2FibGUgcGF1c2luZyBvbiB0aGUgY2Fyb3VzZWwgKGJ5IGRlZmF1bHQsIHRoZSBjYXJvdXNlbCBpbnRlcnZhbCBwYXVzZXMgb24gaG92ZXIpLlxuICpcbiAqIEBleGFtcGxlXG48ZXhhbXBsZSBtb2R1bGU9XCJ1aS5ib290c3RyYXBcIj5cbiAgPGZpbGUgbmFtZT1cImluZGV4Lmh0bWxcIj5cbiAgICA8dWliLWNhcm91c2VsPlxuICAgICAgPHVpYi1zbGlkZT5cbiAgICAgICAgPGltZyBzcmM9XCJodHRwOi8vcGxhY2VraXR0ZW4uY29tLzE1MC8xNTBcIiBzdHlsZT1cIm1hcmdpbjphdXRvO1wiPlxuICAgICAgICA8ZGl2IGNsYXNzPVwiY2Fyb3VzZWwtY2FwdGlvblwiPlxuICAgICAgICAgIDxwPkJlYXV0aWZ1bCE8L3A+XG4gICAgICAgIDwvZGl2PlxuICAgICAgPC91aWItc2xpZGU+XG4gICAgICA8dWliLXNsaWRlPlxuICAgICAgICA8aW1nIHNyYz1cImh0dHA6Ly9wbGFjZWtpdHRlbi5jb20vMTAwLzE1MFwiIHN0eWxlPVwibWFyZ2luOmF1dG87XCI+XG4gICAgICAgIDxkaXYgY2xhc3M9XCJjYXJvdXNlbC1jYXB0aW9uXCI+XG4gICAgICAgICAgPHA+RCdhd3chPC9wPlxuICAgICAgICA8L2Rpdj5cbiAgICAgIDwvdWliLXNsaWRlPlxuICAgIDwvdWliLWNhcm91c2VsPlxuICA8L2ZpbGU+XG4gIDxmaWxlIG5hbWU9XCJkZW1vLmNzc1wiPlxuICAgIC5jYXJvdXNlbC1pbmRpY2F0b3JzIHtcbiAgICAgIHRvcDogYXV0bztcbiAgICAgIGJvdHRvbTogMTVweDtcbiAgICB9XG4gIDwvZmlsZT5cbjwvZXhhbXBsZT5cbiAqL1xuLmRpcmVjdGl2ZSgndWliQ2Fyb3VzZWwnLCBbZnVuY3Rpb24oKSB7XG4gIHJldHVybiB7XG4gICAgdHJhbnNjbHVkZTogdHJ1ZSxcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIGNvbnRyb2xsZXI6ICdVaWJDYXJvdXNlbENvbnRyb2xsZXInLFxuICAgIGNvbnRyb2xsZXJBczogJ2Nhcm91c2VsJyxcbiAgICByZXF1aXJlOiAnY2Fyb3VzZWwnLFxuICAgIHRlbXBsYXRlVXJsOiBmdW5jdGlvbihlbGVtZW50LCBhdHRycykge1xuICAgICAgcmV0dXJuIGF0dHJzLnRlbXBsYXRlVXJsIHx8ICd0ZW1wbGF0ZS9jYXJvdXNlbC9jYXJvdXNlbC5odG1sJztcbiAgICB9LFxuICAgIHNjb3BlOiB7XG4gICAgICBpbnRlcnZhbDogJz0nLFxuICAgICAgbm9UcmFuc2l0aW9uOiAnPScsXG4gICAgICBub1BhdXNlOiAnPScsXG4gICAgICBub1dyYXA6ICcmJ1xuICAgIH1cbiAgfTtcbn1dKVxuXG4vKipcbiAqIEBuZ2RvYyBkaXJlY3RpdmVcbiAqIEBuYW1lIHVpLmJvb3RzdHJhcC5jYXJvdXNlbC5kaXJlY3RpdmU6c2xpZGVcbiAqIEByZXN0cmljdCBFQVxuICpcbiAqIEBkZXNjcmlwdGlvblxuICogQ3JlYXRlcyBhIHNsaWRlIGluc2lkZSBhIHtAbGluayB1aS5ib290c3RyYXAuY2Fyb3VzZWwuZGlyZWN0aXZlOmNhcm91c2VsIGNhcm91c2VsfS4gIE11c3QgYmUgcGxhY2VkIGFzIGEgY2hpbGQgb2YgYSBjYXJvdXNlbCBlbGVtZW50LlxuICpcbiAqIEBwYXJhbSB7Ym9vbGVhbj19IGFjdGl2ZSBNb2RlbCBiaW5kaW5nLCB3aGV0aGVyIG9yIG5vdCB0aGlzIHNsaWRlIGlzIGN1cnJlbnRseSBhY3RpdmUuXG4gKiBAcGFyYW0ge251bWJlcj19IGluZGV4IFRoZSBpbmRleCBvZiB0aGUgc2xpZGUuIFRoZSBzbGlkZXMgd2lsbCBiZSBzb3J0ZWQgYnkgdGhpcyBwYXJhbWV0ZXIuXG4gKlxuICogQGV4YW1wbGVcbjxleGFtcGxlIG1vZHVsZT1cInVpLmJvb3RzdHJhcFwiPlxuICA8ZmlsZSBuYW1lPVwiaW5kZXguaHRtbFwiPlxuPGRpdiBuZy1jb250cm9sbGVyPVwiQ2Fyb3VzZWxEZW1vQ3RybFwiPlxuICA8dWliLWNhcm91c2VsPlxuICAgIDx1aWItc2xpZGUgbmctcmVwZWF0PVwic2xpZGUgaW4gc2xpZGVzXCIgYWN0aXZlPVwic2xpZGUuYWN0aXZlXCIgaW5kZXg9XCIkaW5kZXhcIj5cbiAgICAgIDxpbWcgbmctc3JjPVwie3tzbGlkZS5pbWFnZX19XCIgc3R5bGU9XCJtYXJnaW46YXV0bztcIj5cbiAgICAgIDxkaXYgY2xhc3M9XCJjYXJvdXNlbC1jYXB0aW9uXCI+XG4gICAgICAgIDxoND5TbGlkZSB7eyRpbmRleH19PC9oND5cbiAgICAgICAgPHA+e3tzbGlkZS50ZXh0fX08L3A+XG4gICAgICA8L2Rpdj5cbiAgICA8L3VpYi1zbGlkZT5cbiAgPC91aWItY2Fyb3VzZWw+XG4gIEludGVydmFsLCBpbiBtaWxsaXNlY29uZHM6IDxpbnB1dCB0eXBlPVwibnVtYmVyXCIgbmctbW9kZWw9XCJteUludGVydmFsXCI+XG4gIDxiciAvPkVudGVyIGEgbmVnYXRpdmUgbnVtYmVyIHRvIHN0b3AgdGhlIGludGVydmFsLlxuPC9kaXY+XG4gIDwvZmlsZT5cbiAgPGZpbGUgbmFtZT1cInNjcmlwdC5qc1wiPlxuZnVuY3Rpb24gQ2Fyb3VzZWxEZW1vQ3RybCgkc2NvcGUpIHtcbiAgJHNjb3BlLm15SW50ZXJ2YWwgPSA1MDAwO1xufVxuICA8L2ZpbGU+XG4gIDxmaWxlIG5hbWU9XCJkZW1vLmNzc1wiPlxuICAgIC5jYXJvdXNlbC1pbmRpY2F0b3JzIHtcbiAgICAgIHRvcDogYXV0bztcbiAgICAgIGJvdHRvbTogMTVweDtcbiAgICB9XG4gIDwvZmlsZT5cbjwvZXhhbXBsZT5cbiovXG5cbi5kaXJlY3RpdmUoJ3VpYlNsaWRlJywgZnVuY3Rpb24oKSB7XG4gIHJldHVybiB7XG4gICAgcmVxdWlyZTogJ151aWJDYXJvdXNlbCcsXG4gICAgcmVzdHJpY3Q6ICdFQScsXG4gICAgdHJhbnNjbHVkZTogdHJ1ZSxcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHRlbXBsYXRlVXJsOiBmdW5jdGlvbihlbGVtZW50LCBhdHRycykge1xuICAgICAgcmV0dXJuIGF0dHJzLnRlbXBsYXRlVXJsIHx8ICd0ZW1wbGF0ZS9jYXJvdXNlbC9zbGlkZS5odG1sJztcbiAgICB9LFxuICAgIHNjb3BlOiB7XG4gICAgICBhY3RpdmU6ICc9PycsXG4gICAgICBhY3R1YWw6ICc9PycsXG4gICAgICBpbmRleDogJz0/J1xuICAgIH0sXG4gICAgbGluazogZnVuY3Rpb24gKHNjb3BlLCBlbGVtZW50LCBhdHRycywgY2Fyb3VzZWxDdHJsKSB7XG4gICAgICBjYXJvdXNlbEN0cmwuYWRkU2xpZGUoc2NvcGUsIGVsZW1lbnQpO1xuICAgICAgLy93aGVuIHRoZSBzY29wZSBpcyBkZXN0cm95ZWQgdGhlbiByZW1vdmUgdGhlIHNsaWRlIGZyb20gdGhlIGN1cnJlbnQgc2xpZGVzIGFycmF5XG4gICAgICBzY29wZS4kb24oJyRkZXN0cm95JywgZnVuY3Rpb24oKSB7XG4gICAgICAgIGNhcm91c2VsQ3RybC5yZW1vdmVTbGlkZShzY29wZSk7XG4gICAgICB9KTtcblxuICAgICAgc2NvcGUuJHdhdGNoKCdhY3RpdmUnLCBmdW5jdGlvbihhY3RpdmUpIHtcbiAgICAgICAgaWYgKGFjdGl2ZSkge1xuICAgICAgICAgIGNhcm91c2VsQ3RybC5zZWxlY3Qoc2NvcGUpO1xuICAgICAgICB9XG4gICAgICB9KTtcbiAgICB9XG4gIH07XG59KVxuXG4uYW5pbWF0aW9uKCcuaXRlbScsIFtcbiAgICAgICAgICckaW5qZWN0b3InLCAnJGFuaW1hdGUnLFxuZnVuY3Rpb24gKCRpbmplY3RvciwgJGFuaW1hdGUpIHtcbiAgdmFyIE5PX1RSQU5TSVRJT04gPSAndWliLW5vVHJhbnNpdGlvbicsXG4gICAgU0xJREVfRElSRUNUSU9OID0gJ3VpYi1zbGlkZURpcmVjdGlvbicsXG4gICAgJGFuaW1hdGVDc3MgPSBudWxsO1xuXG4gIGlmICgkaW5qZWN0b3IuaGFzKCckYW5pbWF0ZUNzcycpKSB7XG4gICAgJGFuaW1hdGVDc3MgPSAkaW5qZWN0b3IuZ2V0KCckYW5pbWF0ZUNzcycpO1xuICB9XG5cbiAgZnVuY3Rpb24gcmVtb3ZlQ2xhc3MoZWxlbWVudCwgY2xhc3NOYW1lLCBjYWxsYmFjaykge1xuICAgIGVsZW1lbnQucmVtb3ZlQ2xhc3MoY2xhc3NOYW1lKTtcbiAgICBpZiAoY2FsbGJhY2spIHtcbiAgICAgIGNhbGxiYWNrKCk7XG4gICAgfVxuICB9XG5cbiAgcmV0dXJuIHtcbiAgICBiZWZvcmVBZGRDbGFzczogZnVuY3Rpb24oZWxlbWVudCwgY2xhc3NOYW1lLCBkb25lKSB7XG4gICAgICAvLyBEdWUgdG8gdHJhbnNjbHVzaW9uLCBub1RyYW5zaXRpb24gcHJvcGVydHkgaXMgb24gcGFyZW50J3Mgc2NvcGVcbiAgICAgIGlmIChjbGFzc05hbWUgPT0gJ2FjdGl2ZScgJiYgZWxlbWVudC5wYXJlbnQoKSAmJiBlbGVtZW50LnBhcmVudCgpLnBhcmVudCgpICYmXG4gICAgICAgICAgIWVsZW1lbnQucGFyZW50KCkucGFyZW50KCkuZGF0YShOT19UUkFOU0lUSU9OKSkge1xuICAgICAgICB2YXIgc3RvcHBlZCA9IGZhbHNlO1xuICAgICAgICB2YXIgZGlyZWN0aW9uID0gZWxlbWVudC5kYXRhKFNMSURFX0RJUkVDVElPTik7XG4gICAgICAgIHZhciBkaXJlY3Rpb25DbGFzcyA9IGRpcmVjdGlvbiA9PSAnbmV4dCcgPyAnbGVmdCcgOiAncmlnaHQnO1xuICAgICAgICB2YXIgcmVtb3ZlQ2xhc3NGbiA9IHJlbW92ZUNsYXNzLmJpbmQodGhpcywgZWxlbWVudCxcbiAgICAgICAgICBkaXJlY3Rpb25DbGFzcyArICcgJyArIGRpcmVjdGlvbiwgZG9uZSk7XG4gICAgICAgIGVsZW1lbnQuYWRkQ2xhc3MoZGlyZWN0aW9uKTtcblxuICAgICAgICBpZiAoJGFuaW1hdGVDc3MpIHtcbiAgICAgICAgICAkYW5pbWF0ZUNzcyhlbGVtZW50LCB7YWRkQ2xhc3M6IGRpcmVjdGlvbkNsYXNzfSlcbiAgICAgICAgICAgIC5zdGFydCgpXG4gICAgICAgICAgICAuZG9uZShyZW1vdmVDbGFzc0ZuKTtcbiAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICAkYW5pbWF0ZS5hZGRDbGFzcyhlbGVtZW50LCBkaXJlY3Rpb25DbGFzcykudGhlbihmdW5jdGlvbiAoKSB7XG4gICAgICAgICAgICBpZiAoIXN0b3BwZWQpIHtcbiAgICAgICAgICAgICAgcmVtb3ZlQ2xhc3NGbigpO1xuICAgICAgICAgICAgfVxuICAgICAgICAgICAgZG9uZSgpO1xuICAgICAgICAgIH0pO1xuICAgICAgICB9XG5cbiAgICAgICAgcmV0dXJuIGZ1bmN0aW9uICgpIHtcbiAgICAgICAgICBzdG9wcGVkID0gdHJ1ZTtcbiAgICAgICAgfTtcbiAgICAgIH1cbiAgICAgIGRvbmUoKTtcbiAgICB9LFxuICAgIGJlZm9yZVJlbW92ZUNsYXNzOiBmdW5jdGlvbiAoZWxlbWVudCwgY2xhc3NOYW1lLCBkb25lKSB7XG4gICAgICAvLyBEdWUgdG8gdHJhbnNjbHVzaW9uLCBub1RyYW5zaXRpb24gcHJvcGVydHkgaXMgb24gcGFyZW50J3Mgc2NvcGVcbiAgICAgIGlmIChjbGFzc05hbWUgPT09ICdhY3RpdmUnICYmIGVsZW1lbnQucGFyZW50KCkgJiYgZWxlbWVudC5wYXJlbnQoKS5wYXJlbnQoKSAmJlxuICAgICAgICAgICFlbGVtZW50LnBhcmVudCgpLnBhcmVudCgpLmRhdGEoTk9fVFJBTlNJVElPTikpIHtcbiAgICAgICAgdmFyIHN0b3BwZWQgPSBmYWxzZTtcbiAgICAgICAgdmFyIGRpcmVjdGlvbiA9IGVsZW1lbnQuZGF0YShTTElERV9ESVJFQ1RJT04pO1xuICAgICAgICB2YXIgZGlyZWN0aW9uQ2xhc3MgPSBkaXJlY3Rpb24gPT0gJ25leHQnID8gJ2xlZnQnIDogJ3JpZ2h0JztcbiAgICAgICAgdmFyIHJlbW92ZUNsYXNzRm4gPSByZW1vdmVDbGFzcy5iaW5kKHRoaXMsIGVsZW1lbnQsIGRpcmVjdGlvbkNsYXNzLCBkb25lKTtcblxuICAgICAgICBpZiAoJGFuaW1hdGVDc3MpIHtcbiAgICAgICAgICAkYW5pbWF0ZUNzcyhlbGVtZW50LCB7YWRkQ2xhc3M6IGRpcmVjdGlvbkNsYXNzfSlcbiAgICAgICAgICAgIC5zdGFydCgpXG4gICAgICAgICAgICAuZG9uZShyZW1vdmVDbGFzc0ZuKTtcbiAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICAkYW5pbWF0ZS5hZGRDbGFzcyhlbGVtZW50LCBkaXJlY3Rpb25DbGFzcykudGhlbihmdW5jdGlvbigpIHtcbiAgICAgICAgICAgIGlmICghc3RvcHBlZCkge1xuICAgICAgICAgICAgICByZW1vdmVDbGFzc0ZuKCk7XG4gICAgICAgICAgICB9XG4gICAgICAgICAgICBkb25lKCk7XG4gICAgICAgICAgfSk7XG4gICAgICAgIH1cbiAgICAgICAgcmV0dXJuIGZ1bmN0aW9uKCkge1xuICAgICAgICAgIHN0b3BwZWQgPSB0cnVlO1xuICAgICAgICB9O1xuICAgICAgfVxuICAgICAgZG9uZSgpO1xuICAgIH1cbiAgfTtcbn1dKTtcblxuLyogZGVwcmVjYXRlZCBjYXJvdXNlbCBiZWxvdyAqL1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLmNhcm91c2VsJylcblxuLnZhbHVlKCckY2Fyb3VzZWxTdXBwcmVzc1dhcm5pbmcnLCBmYWxzZSlcblxuLmNvbnRyb2xsZXIoJ0Nhcm91c2VsQ29udHJvbGxlcicsIFsnJHNjb3BlJywgJyRlbGVtZW50JywgJyRjb250cm9sbGVyJywgJyRsb2cnLCAnJGNhcm91c2VsU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJHNjb3BlLCAkZWxlbWVudCwgJGNvbnRyb2xsZXIsICRsb2csICRjYXJvdXNlbFN1cHByZXNzV2FybmluZykge1xuICBpZiAoISRjYXJvdXNlbFN1cHByZXNzV2FybmluZykge1xuICAgICRsb2cud2FybignQ2Fyb3VzZWxDb250cm9sbGVyIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgVWliQ2Fyb3VzZWxDb250cm9sbGVyIGluc3RlYWQuJyk7XG4gIH1cblxuICBhbmd1bGFyLmV4dGVuZCh0aGlzLCAkY29udHJvbGxlcignVWliQ2Fyb3VzZWxDb250cm9sbGVyJywge1xuICAgICRzY29wZTogJHNjb3BlLFxuICAgICRlbGVtZW50OiAkZWxlbWVudFxuICB9KSk7XG59XSlcblxuLmRpcmVjdGl2ZSgnY2Fyb3VzZWwnLCBbJyRsb2cnLCAnJGNhcm91c2VsU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJGxvZywgJGNhcm91c2VsU3VwcHJlc3NXYXJuaW5nKSB7XG4gIHJldHVybiB7XG4gICAgdHJhbnNjbHVkZTogdHJ1ZSxcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIGNvbnRyb2xsZXI6ICdDYXJvdXNlbENvbnRyb2xsZXInLFxuICAgIGNvbnRyb2xsZXJBczogJ2Nhcm91c2VsJyxcbiAgICByZXF1aXJlOiAnY2Fyb3VzZWwnLFxuICAgIHRlbXBsYXRlVXJsOiBmdW5jdGlvbihlbGVtZW50LCBhdHRycykge1xuICAgICAgcmV0dXJuIGF0dHJzLnRlbXBsYXRlVXJsIHx8ICd0ZW1wbGF0ZS9jYXJvdXNlbC9jYXJvdXNlbC5odG1sJztcbiAgICB9LFxuICAgIHNjb3BlOiB7XG4gICAgICBpbnRlcnZhbDogJz0nLFxuICAgICAgbm9UcmFuc2l0aW9uOiAnPScsXG4gICAgICBub1BhdXNlOiAnPScsXG4gICAgICBub1dyYXA6ICcmJ1xuICAgIH0sXG4gICAgbGluazogZnVuY3Rpb24oKSB7XG4gICAgICBpZiAoISRjYXJvdXNlbFN1cHByZXNzV2FybmluZykge1xuICAgICAgICAkbG9nLndhcm4oJ2Nhcm91c2VsIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliLWNhcm91c2VsIGluc3RlYWQuJyk7XG4gICAgICB9XG4gICAgfVxuICB9O1xufV0pXG5cbi5kaXJlY3RpdmUoJ3NsaWRlJywgWyckbG9nJywgJyRjYXJvdXNlbFN1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRsb2csICRjYXJvdXNlbFN1cHByZXNzV2FybmluZykge1xuICByZXR1cm4ge1xuICAgIHJlcXVpcmU6ICdeY2Fyb3VzZWwnLFxuICAgIHRyYW5zY2x1ZGU6IHRydWUsXG4gICAgcmVwbGFjZTogdHJ1ZSxcbiAgICB0ZW1wbGF0ZVVybDogZnVuY3Rpb24oZWxlbWVudCwgYXR0cnMpIHtcbiAgICAgIHJldHVybiBhdHRycy50ZW1wbGF0ZVVybCB8fCAndGVtcGxhdGUvY2Fyb3VzZWwvc2xpZGUuaHRtbCc7XG4gICAgfSxcbiAgICBzY29wZToge1xuICAgICAgYWN0aXZlOiAnPT8nLFxuICAgICAgYWN0dWFsOiAnPT8nLFxuICAgICAgaW5kZXg6ICc9PydcbiAgICB9LFxuICAgIGxpbms6IGZ1bmN0aW9uIChzY29wZSwgZWxlbWVudCwgYXR0cnMsIGNhcm91c2VsQ3RybCkge1xuICAgICAgaWYgKCEkY2Fyb3VzZWxTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgJGxvZy53YXJuKCdzbGlkZSBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi1zbGlkZSBpbnN0ZWFkLicpO1xuICAgICAgfVxuXG4gICAgICBjYXJvdXNlbEN0cmwuYWRkU2xpZGUoc2NvcGUsIGVsZW1lbnQpO1xuICAgICAgLy93aGVuIHRoZSBzY29wZSBpcyBkZXN0cm95ZWQgdGhlbiByZW1vdmUgdGhlIHNsaWRlIGZyb20gdGhlIGN1cnJlbnQgc2xpZGVzIGFycmF5XG4gICAgICBzY29wZS4kb24oJyRkZXN0cm95JywgZnVuY3Rpb24oKSB7XG4gICAgICAgIGNhcm91c2VsQ3RybC5yZW1vdmVTbGlkZShzY29wZSk7XG4gICAgICB9KTtcblxuICAgICAgc2NvcGUuJHdhdGNoKCdhY3RpdmUnLCBmdW5jdGlvbihhY3RpdmUpIHtcbiAgICAgICAgaWYgKGFjdGl2ZSkge1xuICAgICAgICAgIGNhcm91c2VsQ3RybC5zZWxlY3Qoc2NvcGUpO1xuICAgICAgICB9XG4gICAgICB9KTtcbiAgICB9XG4gIH07XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKCd1aS5ib290c3RyYXAuZGF0ZXBhcnNlcicsIFtdKVxuXG4uc2VydmljZSgndWliRGF0ZVBhcnNlcicsIFsnJGxvZycsICckbG9jYWxlJywgJ29yZGVyQnlGaWx0ZXInLCBmdW5jdGlvbigkbG9nLCAkbG9jYWxlLCBvcmRlckJ5RmlsdGVyKSB7XG4gIC8vIFB1bGxlZCBmcm9tIGh0dHBzOi8vZ2l0aHViLmNvbS9tYm9zdG9jay9kMy9ibG9iL21hc3Rlci9zcmMvZm9ybWF0L3JlcXVvdGUuanNcbiAgdmFyIFNQRUNJQUxfQ0hBUkFDVEVSU19SRUdFWFAgPSAvW1xcXFxcXF5cXCRcXCpcXCtcXD9cXHxcXFtcXF1cXChcXClcXC5cXHtcXH1dL2c7XG5cbiAgdmFyIGxvY2FsZUlkO1xuICB2YXIgZm9ybWF0Q29kZVRvUmVnZXg7XG5cbiAgdGhpcy5pbml0ID0gZnVuY3Rpb24oKSB7XG4gICAgbG9jYWxlSWQgPSAkbG9jYWxlLmlkO1xuXG4gICAgdGhpcy5wYXJzZXJzID0ge307XG5cbiAgICBmb3JtYXRDb2RlVG9SZWdleCA9IHtcbiAgICAgICd5eXl5Jzoge1xuICAgICAgICByZWdleDogJ1xcXFxkezR9JyxcbiAgICAgICAgYXBwbHk6IGZ1bmN0aW9uKHZhbHVlKSB7IHRoaXMueWVhciA9ICt2YWx1ZTsgfVxuICAgICAgfSxcbiAgICAgICd5eSc6IHtcbiAgICAgICAgcmVnZXg6ICdcXFxcZHsyfScsXG4gICAgICAgIGFwcGx5OiBmdW5jdGlvbih2YWx1ZSkgeyB0aGlzLnllYXIgPSArdmFsdWUgKyAyMDAwOyB9XG4gICAgICB9LFxuICAgICAgJ3knOiB7XG4gICAgICAgIHJlZ2V4OiAnXFxcXGR7MSw0fScsXG4gICAgICAgIGFwcGx5OiBmdW5jdGlvbih2YWx1ZSkgeyB0aGlzLnllYXIgPSArdmFsdWU7IH1cbiAgICAgIH0sXG4gICAgICAnTU1NTSc6IHtcbiAgICAgICAgcmVnZXg6ICRsb2NhbGUuREFURVRJTUVfRk9STUFUUy5NT05USC5qb2luKCd8JyksXG4gICAgICAgIGFwcGx5OiBmdW5jdGlvbih2YWx1ZSkgeyB0aGlzLm1vbnRoID0gJGxvY2FsZS5EQVRFVElNRV9GT1JNQVRTLk1PTlRILmluZGV4T2YodmFsdWUpOyB9XG4gICAgICB9LFxuICAgICAgJ01NTSc6IHtcbiAgICAgICAgcmVnZXg6ICRsb2NhbGUuREFURVRJTUVfRk9STUFUUy5TSE9SVE1PTlRILmpvaW4oJ3wnKSxcbiAgICAgICAgYXBwbHk6IGZ1bmN0aW9uKHZhbHVlKSB7IHRoaXMubW9udGggPSAkbG9jYWxlLkRBVEVUSU1FX0ZPUk1BVFMuU0hPUlRNT05USC5pbmRleE9mKHZhbHVlKTsgfVxuICAgICAgfSxcbiAgICAgICdNTSc6IHtcbiAgICAgICAgcmVnZXg6ICcwWzEtOV18MVswLTJdJyxcbiAgICAgICAgYXBwbHk6IGZ1bmN0aW9uKHZhbHVlKSB7IHRoaXMubW9udGggPSB2YWx1ZSAtIDE7IH1cbiAgICAgIH0sXG4gICAgICAnTSc6IHtcbiAgICAgICAgcmVnZXg6ICdbMS05XXwxWzAtMl0nLFxuICAgICAgICBhcHBseTogZnVuY3Rpb24odmFsdWUpIHsgdGhpcy5tb250aCA9IHZhbHVlIC0gMTsgfVxuICAgICAgfSxcbiAgICAgICdkZCc6IHtcbiAgICAgICAgcmVnZXg6ICdbMC0yXVswLTldezF9fDNbMC0xXXsxfScsXG4gICAgICAgIGFwcGx5OiBmdW5jdGlvbih2YWx1ZSkgeyB0aGlzLmRhdGUgPSArdmFsdWU7IH1cbiAgICAgIH0sXG4gICAgICAnZCc6IHtcbiAgICAgICAgcmVnZXg6ICdbMS0yXT9bMC05XXsxfXwzWzAtMV17MX0nLFxuICAgICAgICBhcHBseTogZnVuY3Rpb24odmFsdWUpIHsgdGhpcy5kYXRlID0gK3ZhbHVlOyB9XG4gICAgICB9LFxuICAgICAgJ0VFRUUnOiB7XG4gICAgICAgIHJlZ2V4OiAkbG9jYWxlLkRBVEVUSU1FX0ZPUk1BVFMuREFZLmpvaW4oJ3wnKVxuICAgICAgfSxcbiAgICAgICdFRUUnOiB7XG4gICAgICAgIHJlZ2V4OiAkbG9jYWxlLkRBVEVUSU1FX0ZPUk1BVFMuU0hPUlREQVkuam9pbignfCcpXG4gICAgICB9LFxuICAgICAgJ0hIJzoge1xuICAgICAgICByZWdleDogJyg/OjB8MSlbMC05XXwyWzAtM10nLFxuICAgICAgICBhcHBseTogZnVuY3Rpb24odmFsdWUpIHsgdGhpcy5ob3VycyA9ICt2YWx1ZTsgfVxuICAgICAgfSxcbiAgICAgICdoaCc6IHtcbiAgICAgICAgcmVnZXg6ICcwWzAtOV18MVswLTJdJyxcbiAgICAgICAgYXBwbHk6IGZ1bmN0aW9uKHZhbHVlKSB7IHRoaXMuaG91cnMgPSArdmFsdWU7IH1cbiAgICAgIH0sXG4gICAgICAnSCc6IHtcbiAgICAgICAgcmVnZXg6ICcxP1swLTldfDJbMC0zXScsXG4gICAgICAgIGFwcGx5OiBmdW5jdGlvbih2YWx1ZSkgeyB0aGlzLmhvdXJzID0gK3ZhbHVlOyB9XG4gICAgICB9LFxuICAgICAgJ2gnOiB7XG4gICAgICAgIHJlZ2V4OiAnWzAtOV18MVswLTJdJyxcbiAgICAgICAgYXBwbHk6IGZ1bmN0aW9uKHZhbHVlKSB7IHRoaXMuaG91cnMgPSArdmFsdWU7IH1cbiAgICAgIH0sXG4gICAgICAnbW0nOiB7XG4gICAgICAgIHJlZ2V4OiAnWzAtNV1bMC05XScsXG4gICAgICAgIGFwcGx5OiBmdW5jdGlvbih2YWx1ZSkgeyB0aGlzLm1pbnV0ZXMgPSArdmFsdWU7IH1cbiAgICAgIH0sXG4gICAgICAnbSc6IHtcbiAgICAgICAgcmVnZXg6ICdbMC05XXxbMS01XVswLTldJyxcbiAgICAgICAgYXBwbHk6IGZ1bmN0aW9uKHZhbHVlKSB7IHRoaXMubWludXRlcyA9ICt2YWx1ZTsgfVxuICAgICAgfSxcbiAgICAgICdzc3MnOiB7XG4gICAgICAgIHJlZ2V4OiAnWzAtOV1bMC05XVswLTldJyxcbiAgICAgICAgYXBwbHk6IGZ1bmN0aW9uKHZhbHVlKSB7IHRoaXMubWlsbGlzZWNvbmRzID0gK3ZhbHVlOyB9XG4gICAgICB9LFxuICAgICAgJ3NzJzoge1xuICAgICAgICByZWdleDogJ1swLTVdWzAtOV0nLFxuICAgICAgICBhcHBseTogZnVuY3Rpb24odmFsdWUpIHsgdGhpcy5zZWNvbmRzID0gK3ZhbHVlOyB9XG4gICAgICB9LFxuICAgICAgJ3MnOiB7XG4gICAgICAgIHJlZ2V4OiAnWzAtOV18WzEtNV1bMC05XScsXG4gICAgICAgIGFwcGx5OiBmdW5jdGlvbih2YWx1ZSkgeyB0aGlzLnNlY29uZHMgPSArdmFsdWU7IH1cbiAgICAgIH0sXG4gICAgICAnYSc6IHtcbiAgICAgICAgcmVnZXg6ICRsb2NhbGUuREFURVRJTUVfRk9STUFUUy5BTVBNUy5qb2luKCd8JyksXG4gICAgICAgIGFwcGx5OiBmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgICAgIGlmICh0aGlzLmhvdXJzID09PSAxMikge1xuICAgICAgICAgICAgdGhpcy5ob3VycyA9IDA7XG4gICAgICAgICAgfVxuXG4gICAgICAgICAgaWYgKHZhbHVlID09PSAnUE0nKSB7XG4gICAgICAgICAgICB0aGlzLmhvdXJzICs9IDEyO1xuICAgICAgICAgIH1cbiAgICAgICAgfVxuICAgICAgfVxuICAgIH07XG4gIH07XG5cbiAgdGhpcy5pbml0KCk7XG5cbiAgZnVuY3Rpb24gY3JlYXRlUGFyc2VyKGZvcm1hdCkge1xuICAgIHZhciBtYXAgPSBbXSwgcmVnZXggPSBmb3JtYXQuc3BsaXQoJycpO1xuXG4gICAgYW5ndWxhci5mb3JFYWNoKGZvcm1hdENvZGVUb1JlZ2V4LCBmdW5jdGlvbihkYXRhLCBjb2RlKSB7XG4gICAgICB2YXIgaW5kZXggPSBmb3JtYXQuaW5kZXhPZihjb2RlKTtcblxuICAgICAgaWYgKGluZGV4ID4gLTEpIHtcbiAgICAgICAgZm9ybWF0ID0gZm9ybWF0LnNwbGl0KCcnKTtcblxuICAgICAgICByZWdleFtpbmRleF0gPSAnKCcgKyBkYXRhLnJlZ2V4ICsgJyknO1xuICAgICAgICBmb3JtYXRbaW5kZXhdID0gJyQnOyAvLyBDdXN0b20gc3ltYm9sIHRvIGRlZmluZSBjb25zdW1lZCBwYXJ0IG9mIGZvcm1hdFxuICAgICAgICBmb3IgKHZhciBpID0gaW5kZXggKyAxLCBuID0gaW5kZXggKyBjb2RlLmxlbmd0aDsgaSA8IG47IGkrKykge1xuICAgICAgICAgIHJlZ2V4W2ldID0gJyc7XG4gICAgICAgICAgZm9ybWF0W2ldID0gJyQnO1xuICAgICAgICB9XG4gICAgICAgIGZvcm1hdCA9IGZvcm1hdC5qb2luKCcnKTtcblxuICAgICAgICBtYXAucHVzaCh7IGluZGV4OiBpbmRleCwgYXBwbHk6IGRhdGEuYXBwbHkgfSk7XG4gICAgICB9XG4gICAgfSk7XG5cbiAgICByZXR1cm4ge1xuICAgICAgcmVnZXg6IG5ldyBSZWdFeHAoJ14nICsgcmVnZXguam9pbignJykgKyAnJCcpLFxuICAgICAgbWFwOiBvcmRlckJ5RmlsdGVyKG1hcCwgJ2luZGV4JylcbiAgICB9O1xuICB9XG5cbiAgdGhpcy5wYXJzZSA9IGZ1bmN0aW9uKGlucHV0LCBmb3JtYXQsIGJhc2VEYXRlKSB7XG4gICAgaWYgKCFhbmd1bGFyLmlzU3RyaW5nKGlucHV0KSB8fCAhZm9ybWF0KSB7XG4gICAgICByZXR1cm4gaW5wdXQ7XG4gICAgfVxuXG4gICAgZm9ybWF0ID0gJGxvY2FsZS5EQVRFVElNRV9GT1JNQVRTW2Zvcm1hdF0gfHwgZm9ybWF0O1xuICAgIGZvcm1hdCA9IGZvcm1hdC5yZXBsYWNlKFNQRUNJQUxfQ0hBUkFDVEVSU19SRUdFWFAsICdcXFxcJCYnKTtcblxuICAgIGlmICgkbG9jYWxlLmlkICE9PSBsb2NhbGVJZCkge1xuICAgICAgdGhpcy5pbml0KCk7XG4gICAgfVxuXG4gICAgaWYgKCF0aGlzLnBhcnNlcnNbZm9ybWF0XSkge1xuICAgICAgdGhpcy5wYXJzZXJzW2Zvcm1hdF0gPSBjcmVhdGVQYXJzZXIoZm9ybWF0KTtcbiAgICB9XG5cbiAgICB2YXIgcGFyc2VyID0gdGhpcy5wYXJzZXJzW2Zvcm1hdF0sXG4gICAgICAgIHJlZ2V4ID0gcGFyc2VyLnJlZ2V4LFxuICAgICAgICBtYXAgPSBwYXJzZXIubWFwLFxuICAgICAgICByZXN1bHRzID0gaW5wdXQubWF0Y2gocmVnZXgpO1xuXG4gICAgaWYgKHJlc3VsdHMgJiYgcmVzdWx0cy5sZW5ndGgpIHtcbiAgICAgIHZhciBmaWVsZHMsIGR0O1xuICAgICAgaWYgKGFuZ3VsYXIuaXNEYXRlKGJhc2VEYXRlKSAmJiAhaXNOYU4oYmFzZURhdGUuZ2V0VGltZSgpKSkge1xuICAgICAgICBmaWVsZHMgPSB7XG4gICAgICAgICAgeWVhcjogYmFzZURhdGUuZ2V0RnVsbFllYXIoKSxcbiAgICAgICAgICBtb250aDogYmFzZURhdGUuZ2V0TW9udGgoKSxcbiAgICAgICAgICBkYXRlOiBiYXNlRGF0ZS5nZXREYXRlKCksXG4gICAgICAgICAgaG91cnM6IGJhc2VEYXRlLmdldEhvdXJzKCksXG4gICAgICAgICAgbWludXRlczogYmFzZURhdGUuZ2V0TWludXRlcygpLFxuICAgICAgICAgIHNlY29uZHM6IGJhc2VEYXRlLmdldFNlY29uZHMoKSxcbiAgICAgICAgICBtaWxsaXNlY29uZHM6IGJhc2VEYXRlLmdldE1pbGxpc2Vjb25kcygpXG4gICAgICAgIH07XG4gICAgICB9IGVsc2Uge1xuICAgICAgICBpZiAoYmFzZURhdGUpIHtcbiAgICAgICAgICAkbG9nLndhcm4oJ2RhdGVwYXJzZXI6JywgJ2Jhc2VEYXRlIGlzIG5vdCBhIHZhbGlkIGRhdGUnKTtcbiAgICAgICAgfVxuICAgICAgICBmaWVsZHMgPSB7IHllYXI6IDE5MDAsIG1vbnRoOiAwLCBkYXRlOiAxLCBob3VyczogMCwgbWludXRlczogMCwgc2Vjb25kczogMCwgbWlsbGlzZWNvbmRzOiAwIH07XG4gICAgICB9XG5cbiAgICAgIGZvciAodmFyIGkgPSAxLCBuID0gcmVzdWx0cy5sZW5ndGg7IGkgPCBuOyBpKyspIHtcbiAgICAgICAgdmFyIG1hcHBlciA9IG1hcFtpLTFdO1xuICAgICAgICBpZiAobWFwcGVyLmFwcGx5KSB7XG4gICAgICAgICAgbWFwcGVyLmFwcGx5LmNhbGwoZmllbGRzLCByZXN1bHRzW2ldKTtcbiAgICAgICAgfVxuICAgICAgfVxuXG4gICAgICBpZiAoaXNWYWxpZChmaWVsZHMueWVhciwgZmllbGRzLm1vbnRoLCBmaWVsZHMuZGF0ZSkpIHtcbiAgICAgICAgaWYgKGFuZ3VsYXIuaXNEYXRlKGJhc2VEYXRlKSAmJiAhaXNOYU4oYmFzZURhdGUuZ2V0VGltZSgpKSkge1xuICAgICAgICAgIGR0ID0gbmV3IERhdGUoYmFzZURhdGUpO1xuICAgICAgICAgIGR0LnNldEZ1bGxZZWFyKGZpZWxkcy55ZWFyLCBmaWVsZHMubW9udGgsIGZpZWxkcy5kYXRlLFxuICAgICAgICAgICAgZmllbGRzLmhvdXJzLCBmaWVsZHMubWludXRlcywgZmllbGRzLnNlY29uZHMsXG4gICAgICAgICAgICBmaWVsZHMubWlsbGlzZWNvbmRzIHx8IDApO1xuICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgIGR0ID0gbmV3IERhdGUoZmllbGRzLnllYXIsIGZpZWxkcy5tb250aCwgZmllbGRzLmRhdGUsXG4gICAgICAgICAgICBmaWVsZHMuaG91cnMsIGZpZWxkcy5taW51dGVzLCBmaWVsZHMuc2Vjb25kcyxcbiAgICAgICAgICAgIGZpZWxkcy5taWxsaXNlY29uZHMgfHwgMCk7XG4gICAgICAgIH1cbiAgICAgIH1cblxuICAgICAgcmV0dXJuIGR0O1xuICAgIH1cbiAgfTtcblxuICAvLyBDaGVjayBpZiBkYXRlIGlzIHZhbGlkIGZvciBzcGVjaWZpYyBtb250aCAoYW5kIHllYXIgZm9yIEZlYnJ1YXJ5KS5cbiAgLy8gTW9udGg6IDAgPSBKYW4sIDEgPSBGZWIsIGV0Y1xuICBmdW5jdGlvbiBpc1ZhbGlkKHllYXIsIG1vbnRoLCBkYXRlKSB7XG4gICAgaWYgKGRhdGUgPCAxKSB7XG4gICAgICByZXR1cm4gZmFsc2U7XG4gICAgfVxuXG4gICAgaWYgKG1vbnRoID09PSAxICYmIGRhdGUgPiAyOCkge1xuICAgICAgcmV0dXJuIGRhdGUgPT09IDI5ICYmICgoeWVhciAlIDQgPT09IDAgJiYgeWVhciAlIDEwMCAhPT0gMCkgfHwgeWVhciAlIDQwMCA9PT0gMCk7XG4gICAgfVxuXG4gICAgaWYgKG1vbnRoID09PSAzIHx8IG1vbnRoID09PSA1IHx8IG1vbnRoID09PSA4IHx8IG1vbnRoID09PSAxMCkge1xuICAgICAgcmV0dXJuIGRhdGUgPCAzMTtcbiAgICB9XG5cbiAgICByZXR1cm4gdHJ1ZTtcbiAgfVxufV0pO1xuXG4vKiBEZXByZWNhdGVkIGRhdGVwYXJzZXIgYmVsb3cgKi9cblxuYW5ndWxhci5tb2R1bGUoJ3VpLmJvb3RzdHJhcC5kYXRlcGFyc2VyJylcblxuLnZhbHVlKCckZGF0ZVBhcnNlclN1cHByZXNzV2FybmluZycsIGZhbHNlKVxuXG4uc2VydmljZSgnZGF0ZVBhcnNlcicsIFsnJGxvZycsICckZGF0ZVBhcnNlclN1cHByZXNzV2FybmluZycsICd1aWJEYXRlUGFyc2VyJywgZnVuY3Rpb24oJGxvZywgJGRhdGVQYXJzZXJTdXBwcmVzc1dhcm5pbmcsIHVpYkRhdGVQYXJzZXIpIHtcbiAgaWYgKCEkZGF0ZVBhcnNlclN1cHByZXNzV2FybmluZykge1xuICAgICRsb2cud2FybignZGF0ZVBhcnNlciBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYkRhdGVQYXJzZXIgaW5zdGVhZC4nKTtcbiAgfVxuXG4gIGFuZ3VsYXIuZXh0ZW5kKHRoaXMsIHVpYkRhdGVQYXJzZXIpO1xufV0pO1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLnBvc2l0aW9uJywgW10pXG5cbi8qKlxuICogQSBzZXQgb2YgdXRpbGl0eSBtZXRob2RzIHRoYXQgY2FuIGJlIHVzZSB0byByZXRyaWV2ZSBwb3NpdGlvbiBvZiBET00gZWxlbWVudHMuXG4gKiBJdCBpcyBtZWFudCB0byBiZSB1c2VkIHdoZXJlIHdlIG5lZWQgdG8gYWJzb2x1dGUtcG9zaXRpb24gRE9NIGVsZW1lbnRzIGluXG4gKiByZWxhdGlvbiB0byBvdGhlciwgZXhpc3RpbmcgZWxlbWVudHMgKHRoaXMgaXMgdGhlIGNhc2UgZm9yIHRvb2x0aXBzLCBwb3BvdmVycyxcbiAqIHR5cGVhaGVhZCBzdWdnZXN0aW9ucyBldGMuKS5cbiAqL1xuICAuZmFjdG9yeSgnJHVpYlBvc2l0aW9uJywgWyckZG9jdW1lbnQnLCAnJHdpbmRvdycsIGZ1bmN0aW9uKCRkb2N1bWVudCwgJHdpbmRvdykge1xuICAgIGZ1bmN0aW9uIGdldFN0eWxlKGVsLCBjc3Nwcm9wKSB7XG4gICAgICBpZiAoZWwuY3VycmVudFN0eWxlKSB7IC8vSUVcbiAgICAgICAgcmV0dXJuIGVsLmN1cnJlbnRTdHlsZVtjc3Nwcm9wXTtcbiAgICAgIH0gZWxzZSBpZiAoJHdpbmRvdy5nZXRDb21wdXRlZFN0eWxlKSB7XG4gICAgICAgIHJldHVybiAkd2luZG93LmdldENvbXB1dGVkU3R5bGUoZWwpW2Nzc3Byb3BdO1xuICAgICAgfVxuICAgICAgLy8gZmluYWxseSB0cnkgYW5kIGdldCBpbmxpbmUgc3R5bGVcbiAgICAgIHJldHVybiBlbC5zdHlsZVtjc3Nwcm9wXTtcbiAgICB9XG5cbiAgICAvKipcbiAgICAgKiBDaGVja3MgaWYgYSBnaXZlbiBlbGVtZW50IGlzIHN0YXRpY2FsbHkgcG9zaXRpb25lZFxuICAgICAqIEBwYXJhbSBlbGVtZW50IC0gcmF3IERPTSBlbGVtZW50XG4gICAgICovXG4gICAgZnVuY3Rpb24gaXNTdGF0aWNQb3NpdGlvbmVkKGVsZW1lbnQpIHtcbiAgICAgIHJldHVybiAoZ2V0U3R5bGUoZWxlbWVudCwgJ3Bvc2l0aW9uJykgfHwgJ3N0YXRpYycgKSA9PT0gJ3N0YXRpYyc7XG4gICAgfVxuXG4gICAgLyoqXG4gICAgICogcmV0dXJucyB0aGUgY2xvc2VzdCwgbm9uLXN0YXRpY2FsbHkgcG9zaXRpb25lZCBwYXJlbnRPZmZzZXQgb2YgYSBnaXZlbiBlbGVtZW50XG4gICAgICogQHBhcmFtIGVsZW1lbnRcbiAgICAgKi9cbiAgICB2YXIgcGFyZW50T2Zmc2V0RWwgPSBmdW5jdGlvbihlbGVtZW50KSB7XG4gICAgICB2YXIgZG9jRG9tRWwgPSAkZG9jdW1lbnRbMF07XG4gICAgICB2YXIgb2Zmc2V0UGFyZW50ID0gZWxlbWVudC5vZmZzZXRQYXJlbnQgfHwgZG9jRG9tRWw7XG4gICAgICB3aGlsZSAob2Zmc2V0UGFyZW50ICYmIG9mZnNldFBhcmVudCAhPT0gZG9jRG9tRWwgJiYgaXNTdGF0aWNQb3NpdGlvbmVkKG9mZnNldFBhcmVudCkgKSB7XG4gICAgICAgIG9mZnNldFBhcmVudCA9IG9mZnNldFBhcmVudC5vZmZzZXRQYXJlbnQ7XG4gICAgICB9XG4gICAgICByZXR1cm4gb2Zmc2V0UGFyZW50IHx8IGRvY0RvbUVsO1xuICAgIH07XG5cbiAgICByZXR1cm4ge1xuICAgICAgLyoqXG4gICAgICAgKiBQcm92aWRlcyByZWFkLW9ubHkgZXF1aXZhbGVudCBvZiBqUXVlcnkncyBwb3NpdGlvbiBmdW5jdGlvbjpcbiAgICAgICAqIGh0dHA6Ly9hcGkuanF1ZXJ5LmNvbS9wb3NpdGlvbi9cbiAgICAgICAqL1xuICAgICAgcG9zaXRpb246IGZ1bmN0aW9uKGVsZW1lbnQpIHtcbiAgICAgICAgdmFyIGVsQkNSID0gdGhpcy5vZmZzZXQoZWxlbWVudCk7XG4gICAgICAgIHZhciBvZmZzZXRQYXJlbnRCQ1IgPSB7IHRvcDogMCwgbGVmdDogMCB9O1xuICAgICAgICB2YXIgb2Zmc2V0UGFyZW50RWwgPSBwYXJlbnRPZmZzZXRFbChlbGVtZW50WzBdKTtcbiAgICAgICAgaWYgKG9mZnNldFBhcmVudEVsICE9ICRkb2N1bWVudFswXSkge1xuICAgICAgICAgIG9mZnNldFBhcmVudEJDUiA9IHRoaXMub2Zmc2V0KGFuZ3VsYXIuZWxlbWVudChvZmZzZXRQYXJlbnRFbCkpO1xuICAgICAgICAgIG9mZnNldFBhcmVudEJDUi50b3AgKz0gb2Zmc2V0UGFyZW50RWwuY2xpZW50VG9wIC0gb2Zmc2V0UGFyZW50RWwuc2Nyb2xsVG9wO1xuICAgICAgICAgIG9mZnNldFBhcmVudEJDUi5sZWZ0ICs9IG9mZnNldFBhcmVudEVsLmNsaWVudExlZnQgLSBvZmZzZXRQYXJlbnRFbC5zY3JvbGxMZWZ0O1xuICAgICAgICB9XG5cbiAgICAgICAgdmFyIGJvdW5kaW5nQ2xpZW50UmVjdCA9IGVsZW1lbnRbMF0uZ2V0Qm91bmRpbmdDbGllbnRSZWN0KCk7XG4gICAgICAgIHJldHVybiB7XG4gICAgICAgICAgd2lkdGg6IGJvdW5kaW5nQ2xpZW50UmVjdC53aWR0aCB8fCBlbGVtZW50LnByb3AoJ29mZnNldFdpZHRoJyksXG4gICAgICAgICAgaGVpZ2h0OiBib3VuZGluZ0NsaWVudFJlY3QuaGVpZ2h0IHx8IGVsZW1lbnQucHJvcCgnb2Zmc2V0SGVpZ2h0JyksXG4gICAgICAgICAgdG9wOiBlbEJDUi50b3AgLSBvZmZzZXRQYXJlbnRCQ1IudG9wLFxuICAgICAgICAgIGxlZnQ6IGVsQkNSLmxlZnQgLSBvZmZzZXRQYXJlbnRCQ1IubGVmdFxuICAgICAgICB9O1xuICAgICAgfSxcblxuICAgICAgLyoqXG4gICAgICAgKiBQcm92aWRlcyByZWFkLW9ubHkgZXF1aXZhbGVudCBvZiBqUXVlcnkncyBvZmZzZXQgZnVuY3Rpb246XG4gICAgICAgKiBodHRwOi8vYXBpLmpxdWVyeS5jb20vb2Zmc2V0L1xuICAgICAgICovXG4gICAgICBvZmZzZXQ6IGZ1bmN0aW9uKGVsZW1lbnQpIHtcbiAgICAgICAgdmFyIGJvdW5kaW5nQ2xpZW50UmVjdCA9IGVsZW1lbnRbMF0uZ2V0Qm91bmRpbmdDbGllbnRSZWN0KCk7XG4gICAgICAgIHJldHVybiB7XG4gICAgICAgICAgd2lkdGg6IGJvdW5kaW5nQ2xpZW50UmVjdC53aWR0aCB8fCBlbGVtZW50LnByb3AoJ29mZnNldFdpZHRoJyksXG4gICAgICAgICAgaGVpZ2h0OiBib3VuZGluZ0NsaWVudFJlY3QuaGVpZ2h0IHx8IGVsZW1lbnQucHJvcCgnb2Zmc2V0SGVpZ2h0JyksXG4gICAgICAgICAgdG9wOiBib3VuZGluZ0NsaWVudFJlY3QudG9wICsgKCR3aW5kb3cucGFnZVlPZmZzZXQgfHwgJGRvY3VtZW50WzBdLmRvY3VtZW50RWxlbWVudC5zY3JvbGxUb3ApLFxuICAgICAgICAgIGxlZnQ6IGJvdW5kaW5nQ2xpZW50UmVjdC5sZWZ0ICsgKCR3aW5kb3cucGFnZVhPZmZzZXQgfHwgJGRvY3VtZW50WzBdLmRvY3VtZW50RWxlbWVudC5zY3JvbGxMZWZ0KVxuICAgICAgICB9O1xuICAgICAgfSxcblxuICAgICAgLyoqXG4gICAgICAgKiBQcm92aWRlcyBjb29yZGluYXRlcyBmb3IgdGhlIHRhcmdldEVsIGluIHJlbGF0aW9uIHRvIGhvc3RFbFxuICAgICAgICovXG4gICAgICBwb3NpdGlvbkVsZW1lbnRzOiBmdW5jdGlvbihob3N0RWwsIHRhcmdldEVsLCBwb3NpdGlvblN0ciwgYXBwZW5kVG9Cb2R5KSB7XG4gICAgICAgIHZhciBwb3NpdGlvblN0clBhcnRzID0gcG9zaXRpb25TdHIuc3BsaXQoJy0nKTtcbiAgICAgICAgdmFyIHBvczAgPSBwb3NpdGlvblN0clBhcnRzWzBdLCBwb3MxID0gcG9zaXRpb25TdHJQYXJ0c1sxXSB8fCAnY2VudGVyJztcblxuICAgICAgICB2YXIgaG9zdEVsUG9zLFxuICAgICAgICAgIHRhcmdldEVsV2lkdGgsXG4gICAgICAgICAgdGFyZ2V0RWxIZWlnaHQsXG4gICAgICAgICAgdGFyZ2V0RWxQb3M7XG5cbiAgICAgICAgaG9zdEVsUG9zID0gYXBwZW5kVG9Cb2R5ID8gdGhpcy5vZmZzZXQoaG9zdEVsKSA6IHRoaXMucG9zaXRpb24oaG9zdEVsKTtcblxuICAgICAgICB0YXJnZXRFbFdpZHRoID0gdGFyZ2V0RWwucHJvcCgnb2Zmc2V0V2lkdGgnKTtcbiAgICAgICAgdGFyZ2V0RWxIZWlnaHQgPSB0YXJnZXRFbC5wcm9wKCdvZmZzZXRIZWlnaHQnKTtcblxuICAgICAgICB2YXIgc2hpZnRXaWR0aCA9IHtcbiAgICAgICAgICBjZW50ZXI6IGZ1bmN0aW9uKCkge1xuICAgICAgICAgICAgcmV0dXJuIGhvc3RFbFBvcy5sZWZ0ICsgaG9zdEVsUG9zLndpZHRoIC8gMiAtIHRhcmdldEVsV2lkdGggLyAyO1xuICAgICAgICAgIH0sXG4gICAgICAgICAgbGVmdDogZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICByZXR1cm4gaG9zdEVsUG9zLmxlZnQ7XG4gICAgICAgICAgfSxcbiAgICAgICAgICByaWdodDogZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICByZXR1cm4gaG9zdEVsUG9zLmxlZnQgKyBob3N0RWxQb3Mud2lkdGg7XG4gICAgICAgICAgfVxuICAgICAgICB9O1xuXG4gICAgICAgIHZhciBzaGlmdEhlaWdodCA9IHtcbiAgICAgICAgICBjZW50ZXI6IGZ1bmN0aW9uKCkge1xuICAgICAgICAgICAgcmV0dXJuIGhvc3RFbFBvcy50b3AgKyBob3N0RWxQb3MuaGVpZ2h0IC8gMiAtIHRhcmdldEVsSGVpZ2h0IC8gMjtcbiAgICAgICAgICB9LFxuICAgICAgICAgIHRvcDogZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICByZXR1cm4gaG9zdEVsUG9zLnRvcDtcbiAgICAgICAgICB9LFxuICAgICAgICAgIGJvdHRvbTogZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICByZXR1cm4gaG9zdEVsUG9zLnRvcCArIGhvc3RFbFBvcy5oZWlnaHQ7XG4gICAgICAgICAgfVxuICAgICAgICB9O1xuXG4gICAgICAgIHN3aXRjaCAocG9zMCkge1xuICAgICAgICAgIGNhc2UgJ3JpZ2h0JzpcbiAgICAgICAgICAgIHRhcmdldEVsUG9zID0ge1xuICAgICAgICAgICAgICB0b3A6IHNoaWZ0SGVpZ2h0W3BvczFdKCksXG4gICAgICAgICAgICAgIGxlZnQ6IHNoaWZ0V2lkdGhbcG9zMF0oKVxuICAgICAgICAgICAgfTtcbiAgICAgICAgICAgIGJyZWFrO1xuICAgICAgICAgIGNhc2UgJ2xlZnQnOlxuICAgICAgICAgICAgdGFyZ2V0RWxQb3MgPSB7XG4gICAgICAgICAgICAgIHRvcDogc2hpZnRIZWlnaHRbcG9zMV0oKSxcbiAgICAgICAgICAgICAgbGVmdDogaG9zdEVsUG9zLmxlZnQgLSB0YXJnZXRFbFdpZHRoXG4gICAgICAgICAgICB9O1xuICAgICAgICAgICAgYnJlYWs7XG4gICAgICAgICAgY2FzZSAnYm90dG9tJzpcbiAgICAgICAgICAgIHRhcmdldEVsUG9zID0ge1xuICAgICAgICAgICAgICB0b3A6IHNoaWZ0SGVpZ2h0W3BvczBdKCksXG4gICAgICAgICAgICAgIGxlZnQ6IHNoaWZ0V2lkdGhbcG9zMV0oKVxuICAgICAgICAgICAgfTtcbiAgICAgICAgICAgIGJyZWFrO1xuICAgICAgICAgIGRlZmF1bHQ6XG4gICAgICAgICAgICB0YXJnZXRFbFBvcyA9IHtcbiAgICAgICAgICAgICAgdG9wOiBob3N0RWxQb3MudG9wIC0gdGFyZ2V0RWxIZWlnaHQsXG4gICAgICAgICAgICAgIGxlZnQ6IHNoaWZ0V2lkdGhbcG9zMV0oKVxuICAgICAgICAgICAgfTtcbiAgICAgICAgICAgIGJyZWFrO1xuICAgICAgICB9XG5cbiAgICAgICAgcmV0dXJuIHRhcmdldEVsUG9zO1xuICAgICAgfVxuICAgIH07XG4gIH1dKTtcblxuLyogRGVwcmVjYXRlZCBwb3NpdGlvbiBiZWxvdyAqL1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLnBvc2l0aW9uJylcblxuLnZhbHVlKCckcG9zaXRpb25TdXBwcmVzc1dhcm5pbmcnLCBmYWxzZSlcblxuLnNlcnZpY2UoJyRwb3NpdGlvbicsIFsnJGxvZycsICckcG9zaXRpb25TdXBwcmVzc1dhcm5pbmcnLCAnJHVpYlBvc2l0aW9uJywgZnVuY3Rpb24oJGxvZywgJHBvc2l0aW9uU3VwcHJlc3NXYXJuaW5nLCAkdWliUG9zaXRpb24pIHtcbiAgaWYgKCEkcG9zaXRpb25TdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAkbG9nLndhcm4oJyRwb3NpdGlvbiBpcyBub3cgZGVwcmVjYXRlZC4gVXNlICR1aWJQb3NpdGlvbiBpbnN0ZWFkLicpO1xuICB9XG5cbiAgYW5ndWxhci5leHRlbmQodGhpcywgJHVpYlBvc2l0aW9uKTtcbn1dKTtcblxuYW5ndWxhci5tb2R1bGUoJ3VpLmJvb3RzdHJhcC5kYXRlcGlja2VyJywgWyd1aS5ib290c3RyYXAuZGF0ZXBhcnNlcicsICd1aS5ib290c3RyYXAucG9zaXRpb24nXSlcblxuLnZhbHVlKCckZGF0ZXBpY2tlclN1cHByZXNzRXJyb3InLCBmYWxzZSlcblxuLmNvbnN0YW50KCd1aWJEYXRlcGlja2VyQ29uZmlnJywge1xuICBmb3JtYXREYXk6ICdkZCcsXG4gIGZvcm1hdE1vbnRoOiAnTU1NTScsXG4gIGZvcm1hdFllYXI6ICd5eXl5JyxcbiAgZm9ybWF0RGF5SGVhZGVyOiAnRUVFJyxcbiAgZm9ybWF0RGF5VGl0bGU6ICdNTU1NIHl5eXknLFxuICBmb3JtYXRNb250aFRpdGxlOiAneXl5eScsXG4gIGRhdGVwaWNrZXJNb2RlOiAnZGF5JyxcbiAgbWluTW9kZTogJ2RheScsXG4gIG1heE1vZGU6ICd5ZWFyJyxcbiAgc2hvd1dlZWtzOiB0cnVlLFxuICBzdGFydGluZ0RheTogMCxcbiAgeWVhclJhbmdlOiAyMCxcbiAgbWluRGF0ZTogbnVsbCxcbiAgbWF4RGF0ZTogbnVsbCxcbiAgc2hvcnRjdXRQcm9wYWdhdGlvbjogZmFsc2Vcbn0pXG5cbi5jb250cm9sbGVyKCdVaWJEYXRlcGlja2VyQ29udHJvbGxlcicsIFsnJHNjb3BlJywgJyRhdHRycycsICckcGFyc2UnLCAnJGludGVycG9sYXRlJywgJyRsb2cnLCAnZGF0ZUZpbHRlcicsICd1aWJEYXRlcGlja2VyQ29uZmlnJywgJyRkYXRlcGlja2VyU3VwcHJlc3NFcnJvcicsIGZ1bmN0aW9uKCRzY29wZSwgJGF0dHJzLCAkcGFyc2UsICRpbnRlcnBvbGF0ZSwgJGxvZywgZGF0ZUZpbHRlciwgZGF0ZXBpY2tlckNvbmZpZywgJGRhdGVwaWNrZXJTdXBwcmVzc0Vycm9yKSB7XG4gIHZhciBzZWxmID0gdGhpcyxcbiAgICAgIG5nTW9kZWxDdHJsID0geyAkc2V0Vmlld1ZhbHVlOiBhbmd1bGFyLm5vb3AgfTsgLy8gbnVsbE1vZGVsQ3RybDtcblxuICAvLyBNb2RlcyBjaGFpblxuICB0aGlzLm1vZGVzID0gWydkYXknLCAnbW9udGgnLCAneWVhciddO1xuXG4gIC8vIENvbmZpZ3VyYXRpb24gYXR0cmlidXRlc1xuICBhbmd1bGFyLmZvckVhY2goWydmb3JtYXREYXknLCAnZm9ybWF0TW9udGgnLCAnZm9ybWF0WWVhcicsICdmb3JtYXREYXlIZWFkZXInLCAnZm9ybWF0RGF5VGl0bGUnLCAnZm9ybWF0TW9udGhUaXRsZScsXG4gICAgICAgICAgICAgICAgICAgJ3Nob3dXZWVrcycsICdzdGFydGluZ0RheScsICd5ZWFyUmFuZ2UnLCAnc2hvcnRjdXRQcm9wYWdhdGlvbiddLCBmdW5jdGlvbihrZXksIGluZGV4KSB7XG4gICAgc2VsZltrZXldID0gYW5ndWxhci5pc0RlZmluZWQoJGF0dHJzW2tleV0pID8gKGluZGV4IDwgNiA/ICRpbnRlcnBvbGF0ZSgkYXR0cnNba2V5XSkoJHNjb3BlLiRwYXJlbnQpIDogJHNjb3BlLiRwYXJlbnQuJGV2YWwoJGF0dHJzW2tleV0pKSA6IGRhdGVwaWNrZXJDb25maWdba2V5XTtcbiAgfSk7XG5cbiAgLy8gV2F0Y2hhYmxlIGRhdGUgYXR0cmlidXRlc1xuICBhbmd1bGFyLmZvckVhY2goWydtaW5EYXRlJywgJ21heERhdGUnXSwgZnVuY3Rpb24oa2V5KSB7XG4gICAgaWYgKCRhdHRyc1trZXldKSB7XG4gICAgICAkc2NvcGUuJHBhcmVudC4kd2F0Y2goJHBhcnNlKCRhdHRyc1trZXldKSwgZnVuY3Rpb24odmFsdWUpIHtcbiAgICAgICAgc2VsZltrZXldID0gdmFsdWUgPyBuZXcgRGF0ZSh2YWx1ZSkgOiBudWxsO1xuICAgICAgICBzZWxmLnJlZnJlc2hWaWV3KCk7XG4gICAgICB9KTtcbiAgICB9IGVsc2Uge1xuICAgICAgc2VsZltrZXldID0gZGF0ZXBpY2tlckNvbmZpZ1trZXldID8gbmV3IERhdGUoZGF0ZXBpY2tlckNvbmZpZ1trZXldKSA6IG51bGw7XG4gICAgfVxuICB9KTtcblxuICBhbmd1bGFyLmZvckVhY2goWydtaW5Nb2RlJywgJ21heE1vZGUnXSwgZnVuY3Rpb24oa2V5KSB7XG4gICAgaWYgKCRhdHRyc1trZXldKSB7XG4gICAgICAkc2NvcGUuJHBhcmVudC4kd2F0Y2goJHBhcnNlKCRhdHRyc1trZXldKSwgZnVuY3Rpb24odmFsdWUpIHtcbiAgICAgICAgc2VsZltrZXldID0gYW5ndWxhci5pc0RlZmluZWQodmFsdWUpID8gdmFsdWUgOiAkYXR0cnNba2V5XTtcbiAgICAgICAgJHNjb3BlW2tleV0gPSBzZWxmW2tleV07XG4gICAgICAgIGlmICgoa2V5ID09ICdtaW5Nb2RlJyAmJiBzZWxmLm1vZGVzLmluZGV4T2YoJHNjb3BlLmRhdGVwaWNrZXJNb2RlKSA8IHNlbGYubW9kZXMuaW5kZXhPZihzZWxmW2tleV0pKSB8fCAoa2V5ID09ICdtYXhNb2RlJyAmJiBzZWxmLm1vZGVzLmluZGV4T2YoJHNjb3BlLmRhdGVwaWNrZXJNb2RlKSA+IHNlbGYubW9kZXMuaW5kZXhPZihzZWxmW2tleV0pKSkge1xuICAgICAgICAgICRzY29wZS5kYXRlcGlja2VyTW9kZSA9IHNlbGZba2V5XTtcbiAgICAgICAgfVxuICAgICAgfSk7XG4gICAgfSBlbHNlIHtcbiAgICAgIHNlbGZba2V5XSA9IGRhdGVwaWNrZXJDb25maWdba2V5XSB8fCBudWxsO1xuICAgICAgJHNjb3BlW2tleV0gPSBzZWxmW2tleV07XG4gICAgfVxuICB9KTtcblxuICAkc2NvcGUuZGF0ZXBpY2tlck1vZGUgPSAkc2NvcGUuZGF0ZXBpY2tlck1vZGUgfHwgZGF0ZXBpY2tlckNvbmZpZy5kYXRlcGlja2VyTW9kZTtcbiAgJHNjb3BlLnVuaXF1ZUlkID0gJ2RhdGVwaWNrZXItJyArICRzY29wZS4kaWQgKyAnLScgKyBNYXRoLmZsb29yKE1hdGgucmFuZG9tKCkgKiAxMDAwMCk7XG5cbiAgaWYgKGFuZ3VsYXIuaXNEZWZpbmVkKCRhdHRycy5pbml0RGF0ZSkpIHtcbiAgICB0aGlzLmFjdGl2ZURhdGUgPSAkc2NvcGUuJHBhcmVudC4kZXZhbCgkYXR0cnMuaW5pdERhdGUpIHx8IG5ldyBEYXRlKCk7XG4gICAgJHNjb3BlLiRwYXJlbnQuJHdhdGNoKCRhdHRycy5pbml0RGF0ZSwgZnVuY3Rpb24oaW5pdERhdGUpIHtcbiAgICAgIGlmIChpbml0RGF0ZSAmJiAobmdNb2RlbEN0cmwuJGlzRW1wdHkobmdNb2RlbEN0cmwuJG1vZGVsVmFsdWUpIHx8IG5nTW9kZWxDdHJsLiRpbnZhbGlkKSkge1xuICAgICAgICBzZWxmLmFjdGl2ZURhdGUgPSBpbml0RGF0ZTtcbiAgICAgICAgc2VsZi5yZWZyZXNoVmlldygpO1xuICAgICAgfVxuICAgIH0pO1xuICB9IGVsc2Uge1xuICAgIHRoaXMuYWN0aXZlRGF0ZSA9IG5ldyBEYXRlKCk7XG4gIH1cblxuICAkc2NvcGUuaXNBY3RpdmUgPSBmdW5jdGlvbihkYXRlT2JqZWN0KSB7XG4gICAgaWYgKHNlbGYuY29tcGFyZShkYXRlT2JqZWN0LmRhdGUsIHNlbGYuYWN0aXZlRGF0ZSkgPT09IDApIHtcbiAgICAgICRzY29wZS5hY3RpdmVEYXRlSWQgPSBkYXRlT2JqZWN0LnVpZDtcbiAgICAgIHJldHVybiB0cnVlO1xuICAgIH1cbiAgICByZXR1cm4gZmFsc2U7XG4gIH07XG5cbiAgdGhpcy5pbml0ID0gZnVuY3Rpb24obmdNb2RlbEN0cmxfKSB7XG4gICAgbmdNb2RlbEN0cmwgPSBuZ01vZGVsQ3RybF87XG5cbiAgICBuZ01vZGVsQ3RybC4kcmVuZGVyID0gZnVuY3Rpb24oKSB7XG4gICAgICBzZWxmLnJlbmRlcigpO1xuICAgIH07XG4gIH07XG5cbiAgdGhpcy5yZW5kZXIgPSBmdW5jdGlvbigpIHtcbiAgICBpZiAobmdNb2RlbEN0cmwuJHZpZXdWYWx1ZSkge1xuICAgICAgdmFyIGRhdGUgPSBuZXcgRGF0ZShuZ01vZGVsQ3RybC4kdmlld1ZhbHVlKSxcbiAgICAgICAgICBpc1ZhbGlkID0gIWlzTmFOKGRhdGUpO1xuXG4gICAgICBpZiAoaXNWYWxpZCkge1xuICAgICAgICB0aGlzLmFjdGl2ZURhdGUgPSBkYXRlO1xuICAgICAgfSBlbHNlIGlmICghJGRhdGVwaWNrZXJTdXBwcmVzc0Vycm9yKSB7XG4gICAgICAgICRsb2cuZXJyb3IoJ0RhdGVwaWNrZXIgZGlyZWN0aXZlOiBcIm5nLW1vZGVsXCIgdmFsdWUgbXVzdCBiZSBhIERhdGUgb2JqZWN0LCBhIG51bWJlciBvZiBtaWxsaXNlY29uZHMgc2luY2UgMDEuMDEuMTk3MCBvciBhIHN0cmluZyByZXByZXNlbnRpbmcgYW4gUkZDMjgyMiBvciBJU08gODYwMSBkYXRlLicpO1xuICAgICAgfVxuICAgIH1cbiAgICB0aGlzLnJlZnJlc2hWaWV3KCk7XG4gIH07XG5cbiAgdGhpcy5yZWZyZXNoVmlldyA9IGZ1bmN0aW9uKCkge1xuICAgIGlmICh0aGlzLmVsZW1lbnQpIHtcbiAgICAgIHRoaXMuX3JlZnJlc2hWaWV3KCk7XG5cbiAgICAgIHZhciBkYXRlID0gbmdNb2RlbEN0cmwuJHZpZXdWYWx1ZSA/IG5ldyBEYXRlKG5nTW9kZWxDdHJsLiR2aWV3VmFsdWUpIDogbnVsbDtcbiAgICAgIG5nTW9kZWxDdHJsLiRzZXRWYWxpZGl0eSgnZGF0ZURpc2FibGVkJywgIWRhdGUgfHwgKHRoaXMuZWxlbWVudCAmJiAhdGhpcy5pc0Rpc2FibGVkKGRhdGUpKSk7XG4gICAgfVxuICB9O1xuXG4gIHRoaXMuY3JlYXRlRGF0ZU9iamVjdCA9IGZ1bmN0aW9uKGRhdGUsIGZvcm1hdCkge1xuICAgIHZhciBtb2RlbCA9IG5nTW9kZWxDdHJsLiR2aWV3VmFsdWUgPyBuZXcgRGF0ZShuZ01vZGVsQ3RybC4kdmlld1ZhbHVlKSA6IG51bGw7XG4gICAgcmV0dXJuIHtcbiAgICAgIGRhdGU6IGRhdGUsXG4gICAgICBsYWJlbDogZGF0ZUZpbHRlcihkYXRlLCBmb3JtYXQpLFxuICAgICAgc2VsZWN0ZWQ6IG1vZGVsICYmIHRoaXMuY29tcGFyZShkYXRlLCBtb2RlbCkgPT09IDAsXG4gICAgICBkaXNhYmxlZDogdGhpcy5pc0Rpc2FibGVkKGRhdGUpLFxuICAgICAgY3VycmVudDogdGhpcy5jb21wYXJlKGRhdGUsIG5ldyBEYXRlKCkpID09PSAwLFxuICAgICAgY3VzdG9tQ2xhc3M6IHRoaXMuY3VzdG9tQ2xhc3MoZGF0ZSlcbiAgICB9O1xuICB9O1xuXG4gIHRoaXMuaXNEaXNhYmxlZCA9IGZ1bmN0aW9uKGRhdGUpIHtcbiAgICByZXR1cm4gKCh0aGlzLm1pbkRhdGUgJiYgdGhpcy5jb21wYXJlKGRhdGUsIHRoaXMubWluRGF0ZSkgPCAwKSB8fCAodGhpcy5tYXhEYXRlICYmIHRoaXMuY29tcGFyZShkYXRlLCB0aGlzLm1heERhdGUpID4gMCkgfHwgKCRhdHRycy5kYXRlRGlzYWJsZWQgJiYgJHNjb3BlLmRhdGVEaXNhYmxlZCh7ZGF0ZTogZGF0ZSwgbW9kZTogJHNjb3BlLmRhdGVwaWNrZXJNb2RlfSkpKTtcbiAgfTtcblxuICB0aGlzLmN1c3RvbUNsYXNzID0gZnVuY3Rpb24oZGF0ZSkge1xuICAgIHJldHVybiAkc2NvcGUuY3VzdG9tQ2xhc3Moe2RhdGU6IGRhdGUsIG1vZGU6ICRzY29wZS5kYXRlcGlja2VyTW9kZX0pO1xuICB9O1xuXG4gIC8vIFNwbGl0IGFycmF5IGludG8gc21hbGxlciBhcnJheXNcbiAgdGhpcy5zcGxpdCA9IGZ1bmN0aW9uKGFyciwgc2l6ZSkge1xuICAgIHZhciBhcnJheXMgPSBbXTtcbiAgICB3aGlsZSAoYXJyLmxlbmd0aCA+IDApIHtcbiAgICAgIGFycmF5cy5wdXNoKGFyci5zcGxpY2UoMCwgc2l6ZSkpO1xuICAgIH1cbiAgICByZXR1cm4gYXJyYXlzO1xuICB9O1xuXG4gICRzY29wZS5zZWxlY3QgPSBmdW5jdGlvbihkYXRlKSB7XG4gICAgaWYgKCRzY29wZS5kYXRlcGlja2VyTW9kZSA9PT0gc2VsZi5taW5Nb2RlKSB7XG4gICAgICB2YXIgZHQgPSBuZ01vZGVsQ3RybC4kdmlld1ZhbHVlID8gbmV3IERhdGUobmdNb2RlbEN0cmwuJHZpZXdWYWx1ZSkgOiBuZXcgRGF0ZSgwLCAwLCAwLCAwLCAwLCAwLCAwKTtcbiAgICAgIGR0LnNldEZ1bGxZZWFyKGRhdGUuZ2V0RnVsbFllYXIoKSwgZGF0ZS5nZXRNb250aCgpLCBkYXRlLmdldERhdGUoKSk7XG4gICAgICBuZ01vZGVsQ3RybC4kc2V0Vmlld1ZhbHVlKGR0KTtcbiAgICAgIG5nTW9kZWxDdHJsLiRyZW5kZXIoKTtcbiAgICB9IGVsc2Uge1xuICAgICAgc2VsZi5hY3RpdmVEYXRlID0gZGF0ZTtcbiAgICAgICRzY29wZS5kYXRlcGlja2VyTW9kZSA9IHNlbGYubW9kZXNbc2VsZi5tb2Rlcy5pbmRleE9mKCRzY29wZS5kYXRlcGlja2VyTW9kZSkgLSAxXTtcbiAgICB9XG4gIH07XG5cbiAgJHNjb3BlLm1vdmUgPSBmdW5jdGlvbihkaXJlY3Rpb24pIHtcbiAgICB2YXIgeWVhciA9IHNlbGYuYWN0aXZlRGF0ZS5nZXRGdWxsWWVhcigpICsgZGlyZWN0aW9uICogKHNlbGYuc3RlcC55ZWFycyB8fCAwKSxcbiAgICAgICAgbW9udGggPSBzZWxmLmFjdGl2ZURhdGUuZ2V0TW9udGgoKSArIGRpcmVjdGlvbiAqIChzZWxmLnN0ZXAubW9udGhzIHx8IDApO1xuICAgIHNlbGYuYWN0aXZlRGF0ZS5zZXRGdWxsWWVhcih5ZWFyLCBtb250aCwgMSk7XG4gICAgc2VsZi5yZWZyZXNoVmlldygpO1xuICB9O1xuXG4gICRzY29wZS50b2dnbGVNb2RlID0gZnVuY3Rpb24oZGlyZWN0aW9uKSB7XG4gICAgZGlyZWN0aW9uID0gZGlyZWN0aW9uIHx8IDE7XG5cbiAgICBpZiAoKCRzY29wZS5kYXRlcGlja2VyTW9kZSA9PT0gc2VsZi5tYXhNb2RlICYmIGRpcmVjdGlvbiA9PT0gMSkgfHwgKCRzY29wZS5kYXRlcGlja2VyTW9kZSA9PT0gc2VsZi5taW5Nb2RlICYmIGRpcmVjdGlvbiA9PT0gLTEpKSB7XG4gICAgICByZXR1cm47XG4gICAgfVxuXG4gICAgJHNjb3BlLmRhdGVwaWNrZXJNb2RlID0gc2VsZi5tb2Rlc1tzZWxmLm1vZGVzLmluZGV4T2YoJHNjb3BlLmRhdGVwaWNrZXJNb2RlKSArIGRpcmVjdGlvbl07XG4gIH07XG5cbiAgLy8gS2V5IGV2ZW50IG1hcHBlclxuICAkc2NvcGUua2V5cyA9IHsgMTM6ICdlbnRlcicsIDMyOiAnc3BhY2UnLCAzMzogJ3BhZ2V1cCcsIDM0OiAncGFnZWRvd24nLCAzNTogJ2VuZCcsIDM2OiAnaG9tZScsIDM3OiAnbGVmdCcsIDM4OiAndXAnLCAzOTogJ3JpZ2h0JywgNDA6ICdkb3duJyB9O1xuXG4gIHZhciBmb2N1c0VsZW1lbnQgPSBmdW5jdGlvbigpIHtcbiAgICBzZWxmLmVsZW1lbnRbMF0uZm9jdXMoKTtcbiAgfTtcblxuICAvLyBMaXN0ZW4gZm9yIGZvY3VzIHJlcXVlc3RzIGZyb20gcG9wdXAgZGlyZWN0aXZlXG4gICRzY29wZS4kb24oJ3VpYjpkYXRlcGlja2VyLmZvY3VzJywgZm9jdXNFbGVtZW50KTtcblxuICAkc2NvcGUua2V5ZG93biA9IGZ1bmN0aW9uKGV2dCkge1xuICAgIHZhciBrZXkgPSAkc2NvcGUua2V5c1tldnQud2hpY2hdO1xuXG4gICAgaWYgKCFrZXkgfHwgZXZ0LnNoaWZ0S2V5IHx8IGV2dC5hbHRLZXkpIHtcbiAgICAgIHJldHVybjtcbiAgICB9XG5cbiAgICBldnQucHJldmVudERlZmF1bHQoKTtcbiAgICBpZiAoIXNlbGYuc2hvcnRjdXRQcm9wYWdhdGlvbikge1xuICAgICAgZXZ0LnN0b3BQcm9wYWdhdGlvbigpO1xuICAgIH1cblxuICAgIGlmIChrZXkgPT09ICdlbnRlcicgfHwga2V5ID09PSAnc3BhY2UnKSB7XG4gICAgICBpZiAoc2VsZi5pc0Rpc2FibGVkKHNlbGYuYWN0aXZlRGF0ZSkpIHtcbiAgICAgICAgcmV0dXJuOyAvLyBkbyBub3RoaW5nXG4gICAgICB9XG4gICAgICAkc2NvcGUuc2VsZWN0KHNlbGYuYWN0aXZlRGF0ZSk7XG4gICAgfSBlbHNlIGlmIChldnQuY3RybEtleSAmJiAoa2V5ID09PSAndXAnIHx8IGtleSA9PT0gJ2Rvd24nKSkge1xuICAgICAgJHNjb3BlLnRvZ2dsZU1vZGUoa2V5ID09PSAndXAnID8gMSA6IC0xKTtcbiAgICB9IGVsc2Uge1xuICAgICAgc2VsZi5oYW5kbGVLZXlEb3duKGtleSwgZXZ0KTtcbiAgICAgIHNlbGYucmVmcmVzaFZpZXcoKTtcbiAgICB9XG4gIH07XG59XSlcblxuLmNvbnRyb2xsZXIoJ1VpYkRheXBpY2tlckNvbnRyb2xsZXInLCBbJyRzY29wZScsICckZWxlbWVudCcsICdkYXRlRmlsdGVyJywgZnVuY3Rpb24oc2NvcGUsICRlbGVtZW50LCBkYXRlRmlsdGVyKSB7XG4gIHZhciBEQVlTX0lOX01PTlRIID0gWzMxLCAyOCwgMzEsIDMwLCAzMSwgMzAsIDMxLCAzMSwgMzAsIDMxLCAzMCwgMzFdO1xuXG4gIHRoaXMuc3RlcCA9IHsgbW9udGhzOiAxIH07XG4gIHRoaXMuZWxlbWVudCA9ICRlbGVtZW50O1xuICBmdW5jdGlvbiBnZXREYXlzSW5Nb250aCh5ZWFyLCBtb250aCkge1xuICAgIHJldHVybiAoKG1vbnRoID09PSAxKSAmJiAoeWVhciAlIDQgPT09IDApICYmICgoeWVhciAlIDEwMCAhPT0gMCkgfHwgKHllYXIgJSA0MDAgPT09IDApKSkgPyAyOSA6IERBWVNfSU5fTU9OVEhbbW9udGhdO1xuICB9XG5cbiAgdGhpcy5pbml0ID0gZnVuY3Rpb24oY3RybCkge1xuICAgIGFuZ3VsYXIuZXh0ZW5kKGN0cmwsIHRoaXMpO1xuICAgIHNjb3BlLnNob3dXZWVrcyA9IGN0cmwuc2hvd1dlZWtzO1xuICAgIGN0cmwucmVmcmVzaFZpZXcoKTtcbiAgfTtcblxuICB0aGlzLmdldERhdGVzID0gZnVuY3Rpb24oc3RhcnREYXRlLCBuKSB7XG4gICAgdmFyIGRhdGVzID0gbmV3IEFycmF5KG4pLCBjdXJyZW50ID0gbmV3IERhdGUoc3RhcnREYXRlKSwgaSA9IDAsIGRhdGU7XG4gICAgd2hpbGUgKGkgPCBuKSB7XG4gICAgICBkYXRlID0gbmV3IERhdGUoY3VycmVudCk7XG4gICAgICBkYXRlc1tpKytdID0gZGF0ZTtcbiAgICAgIGN1cnJlbnQuc2V0RGF0ZShjdXJyZW50LmdldERhdGUoKSArIDEpO1xuICAgIH1cbiAgICByZXR1cm4gZGF0ZXM7XG4gIH07XG5cbiAgdGhpcy5fcmVmcmVzaFZpZXcgPSBmdW5jdGlvbigpIHtcbiAgICB2YXIgeWVhciA9IHRoaXMuYWN0aXZlRGF0ZS5nZXRGdWxsWWVhcigpLFxuICAgICAgbW9udGggPSB0aGlzLmFjdGl2ZURhdGUuZ2V0TW9udGgoKSxcbiAgICAgIGZpcnN0RGF5T2ZNb250aCA9IG5ldyBEYXRlKHRoaXMuYWN0aXZlRGF0ZSk7XG5cbiAgICBmaXJzdERheU9mTW9udGguc2V0RnVsbFllYXIoeWVhciwgbW9udGgsIDEpO1xuXG4gICAgdmFyIGRpZmZlcmVuY2UgPSB0aGlzLnN0YXJ0aW5nRGF5IC0gZmlyc3REYXlPZk1vbnRoLmdldERheSgpLFxuICAgICAgbnVtRGlzcGxheWVkRnJvbVByZXZpb3VzTW9udGggPSAoZGlmZmVyZW5jZSA+IDApID8gNyAtIGRpZmZlcmVuY2UgOiAtIGRpZmZlcmVuY2UsXG4gICAgICBmaXJzdERhdGUgPSBuZXcgRGF0ZShmaXJzdERheU9mTW9udGgpO1xuXG4gICAgaWYgKG51bURpc3BsYXllZEZyb21QcmV2aW91c01vbnRoID4gMCkge1xuICAgICAgZmlyc3REYXRlLnNldERhdGUoLW51bURpc3BsYXllZEZyb21QcmV2aW91c01vbnRoICsgMSk7XG4gICAgfVxuXG4gICAgLy8gNDIgaXMgdGhlIG51bWJlciBvZiBkYXlzIG9uIGEgc2l4LW1vbnRoIGNhbGVuZGFyXG4gICAgdmFyIGRheXMgPSB0aGlzLmdldERhdGVzKGZpcnN0RGF0ZSwgNDIpO1xuICAgIGZvciAodmFyIGkgPSAwOyBpIDwgNDI7IGkgKyspIHtcbiAgICAgIGRheXNbaV0gPSBhbmd1bGFyLmV4dGVuZCh0aGlzLmNyZWF0ZURhdGVPYmplY3QoZGF5c1tpXSwgdGhpcy5mb3JtYXREYXkpLCB7XG4gICAgICAgIHNlY29uZGFyeTogZGF5c1tpXS5nZXRNb250aCgpICE9PSBtb250aCxcbiAgICAgICAgdWlkOiBzY29wZS51bmlxdWVJZCArICctJyArIGlcbiAgICAgIH0pO1xuICAgIH1cblxuICAgIHNjb3BlLmxhYmVscyA9IG5ldyBBcnJheSg3KTtcbiAgICBmb3IgKHZhciBqID0gMDsgaiA8IDc7IGorKykge1xuICAgICAgc2NvcGUubGFiZWxzW2pdID0ge1xuICAgICAgICBhYmJyOiBkYXRlRmlsdGVyKGRheXNbal0uZGF0ZSwgdGhpcy5mb3JtYXREYXlIZWFkZXIpLFxuICAgICAgICBmdWxsOiBkYXRlRmlsdGVyKGRheXNbal0uZGF0ZSwgJ0VFRUUnKVxuICAgICAgfTtcbiAgICB9XG5cbiAgICBzY29wZS50aXRsZSA9IGRhdGVGaWx0ZXIodGhpcy5hY3RpdmVEYXRlLCB0aGlzLmZvcm1hdERheVRpdGxlKTtcbiAgICBzY29wZS5yb3dzID0gdGhpcy5zcGxpdChkYXlzLCA3KTtcblxuICAgIGlmIChzY29wZS5zaG93V2Vla3MpIHtcbiAgICAgIHNjb3BlLndlZWtOdW1iZXJzID0gW107XG4gICAgICB2YXIgdGh1cnNkYXlJbmRleCA9ICg0ICsgNyAtIHRoaXMuc3RhcnRpbmdEYXkpICUgNyxcbiAgICAgICAgICBudW1XZWVrcyA9IHNjb3BlLnJvd3MubGVuZ3RoO1xuICAgICAgZm9yICh2YXIgY3VyV2VlayA9IDA7IGN1cldlZWsgPCBudW1XZWVrczsgY3VyV2VlaysrKSB7XG4gICAgICAgIHNjb3BlLndlZWtOdW1iZXJzLnB1c2goXG4gICAgICAgICAgZ2V0SVNPODYwMVdlZWtOdW1iZXIoc2NvcGUucm93c1tjdXJXZWVrXVt0aHVyc2RheUluZGV4XS5kYXRlKSk7XG4gICAgICB9XG4gICAgfVxuICB9O1xuXG4gIHRoaXMuY29tcGFyZSA9IGZ1bmN0aW9uKGRhdGUxLCBkYXRlMikge1xuICAgIHJldHVybiAobmV3IERhdGUoZGF0ZTEuZ2V0RnVsbFllYXIoKSwgZGF0ZTEuZ2V0TW9udGgoKSwgZGF0ZTEuZ2V0RGF0ZSgpKSAtIG5ldyBEYXRlKGRhdGUyLmdldEZ1bGxZZWFyKCksIGRhdGUyLmdldE1vbnRoKCksIGRhdGUyLmdldERhdGUoKSkpO1xuICB9O1xuXG4gIGZ1bmN0aW9uIGdldElTTzg2MDFXZWVrTnVtYmVyKGRhdGUpIHtcbiAgICB2YXIgY2hlY2tEYXRlID0gbmV3IERhdGUoZGF0ZSk7XG4gICAgY2hlY2tEYXRlLnNldERhdGUoY2hlY2tEYXRlLmdldERhdGUoKSArIDQgLSAoY2hlY2tEYXRlLmdldERheSgpIHx8IDcpKTsgLy8gVGh1cnNkYXlcbiAgICB2YXIgdGltZSA9IGNoZWNrRGF0ZS5nZXRUaW1lKCk7XG4gICAgY2hlY2tEYXRlLnNldE1vbnRoKDApOyAvLyBDb21wYXJlIHdpdGggSmFuIDFcbiAgICBjaGVja0RhdGUuc2V0RGF0ZSgxKTtcbiAgICByZXR1cm4gTWF0aC5mbG9vcihNYXRoLnJvdW5kKCh0aW1lIC0gY2hlY2tEYXRlKSAvIDg2NDAwMDAwKSAvIDcpICsgMTtcbiAgfVxuXG4gIHRoaXMuaGFuZGxlS2V5RG93biA9IGZ1bmN0aW9uKGtleSwgZXZ0KSB7XG4gICAgdmFyIGRhdGUgPSB0aGlzLmFjdGl2ZURhdGUuZ2V0RGF0ZSgpO1xuXG4gICAgaWYgKGtleSA9PT0gJ2xlZnQnKSB7XG4gICAgICBkYXRlID0gZGF0ZSAtIDE7ICAgLy8gdXBcbiAgICB9IGVsc2UgaWYgKGtleSA9PT0gJ3VwJykge1xuICAgICAgZGF0ZSA9IGRhdGUgLSA3OyAgIC8vIGRvd25cbiAgICB9IGVsc2UgaWYgKGtleSA9PT0gJ3JpZ2h0Jykge1xuICAgICAgZGF0ZSA9IGRhdGUgKyAxOyAgIC8vIGRvd25cbiAgICB9IGVsc2UgaWYgKGtleSA9PT0gJ2Rvd24nKSB7XG4gICAgICBkYXRlID0gZGF0ZSArIDc7XG4gICAgfSBlbHNlIGlmIChrZXkgPT09ICdwYWdldXAnIHx8IGtleSA9PT0gJ3BhZ2Vkb3duJykge1xuICAgICAgdmFyIG1vbnRoID0gdGhpcy5hY3RpdmVEYXRlLmdldE1vbnRoKCkgKyAoa2V5ID09PSAncGFnZXVwJyA/IC0gMSA6IDEpO1xuICAgICAgdGhpcy5hY3RpdmVEYXRlLnNldE1vbnRoKG1vbnRoLCAxKTtcbiAgICAgIGRhdGUgPSBNYXRoLm1pbihnZXREYXlzSW5Nb250aCh0aGlzLmFjdGl2ZURhdGUuZ2V0RnVsbFllYXIoKSwgdGhpcy5hY3RpdmVEYXRlLmdldE1vbnRoKCkpLCBkYXRlKTtcbiAgICB9IGVsc2UgaWYgKGtleSA9PT0gJ2hvbWUnKSB7XG4gICAgICBkYXRlID0gMTtcbiAgICB9IGVsc2UgaWYgKGtleSA9PT0gJ2VuZCcpIHtcbiAgICAgIGRhdGUgPSBnZXREYXlzSW5Nb250aCh0aGlzLmFjdGl2ZURhdGUuZ2V0RnVsbFllYXIoKSwgdGhpcy5hY3RpdmVEYXRlLmdldE1vbnRoKCkpO1xuICAgIH1cbiAgICB0aGlzLmFjdGl2ZURhdGUuc2V0RGF0ZShkYXRlKTtcbiAgfTtcbn1dKVxuXG4uY29udHJvbGxlcignVWliTW9udGhwaWNrZXJDb250cm9sbGVyJywgWyckc2NvcGUnLCAnJGVsZW1lbnQnLCAnZGF0ZUZpbHRlcicsIGZ1bmN0aW9uKHNjb3BlLCAkZWxlbWVudCwgZGF0ZUZpbHRlcikge1xuICB0aGlzLnN0ZXAgPSB7IHllYXJzOiAxIH07XG4gIHRoaXMuZWxlbWVudCA9ICRlbGVtZW50O1xuXG4gIHRoaXMuaW5pdCA9IGZ1bmN0aW9uKGN0cmwpIHtcbiAgICBhbmd1bGFyLmV4dGVuZChjdHJsLCB0aGlzKTtcbiAgICBjdHJsLnJlZnJlc2hWaWV3KCk7XG4gIH07XG5cbiAgdGhpcy5fcmVmcmVzaFZpZXcgPSBmdW5jdGlvbigpIHtcbiAgICB2YXIgbW9udGhzID0gbmV3IEFycmF5KDEyKSxcbiAgICAgICAgeWVhciA9IHRoaXMuYWN0aXZlRGF0ZS5nZXRGdWxsWWVhcigpLFxuICAgICAgICBkYXRlO1xuXG4gICAgZm9yICh2YXIgaSA9IDA7IGkgPCAxMjsgaSsrKSB7XG4gICAgICBkYXRlID0gbmV3IERhdGUodGhpcy5hY3RpdmVEYXRlKTtcbiAgICAgIGRhdGUuc2V0RnVsbFllYXIoeWVhciwgaSwgMSk7XG4gICAgICBtb250aHNbaV0gPSBhbmd1bGFyLmV4dGVuZCh0aGlzLmNyZWF0ZURhdGVPYmplY3QoZGF0ZSwgdGhpcy5mb3JtYXRNb250aCksIHtcbiAgICAgICAgdWlkOiBzY29wZS51bmlxdWVJZCArICctJyArIGlcbiAgICAgIH0pO1xuICAgIH1cblxuICAgIHNjb3BlLnRpdGxlID0gZGF0ZUZpbHRlcih0aGlzLmFjdGl2ZURhdGUsIHRoaXMuZm9ybWF0TW9udGhUaXRsZSk7XG4gICAgc2NvcGUucm93cyA9IHRoaXMuc3BsaXQobW9udGhzLCAzKTtcbiAgfTtcblxuICB0aGlzLmNvbXBhcmUgPSBmdW5jdGlvbihkYXRlMSwgZGF0ZTIpIHtcbiAgICByZXR1cm4gbmV3IERhdGUoZGF0ZTEuZ2V0RnVsbFllYXIoKSwgZGF0ZTEuZ2V0TW9udGgoKSkgLSBuZXcgRGF0ZShkYXRlMi5nZXRGdWxsWWVhcigpLCBkYXRlMi5nZXRNb250aCgpKTtcbiAgfTtcblxuICB0aGlzLmhhbmRsZUtleURvd24gPSBmdW5jdGlvbihrZXksIGV2dCkge1xuICAgIHZhciBkYXRlID0gdGhpcy5hY3RpdmVEYXRlLmdldE1vbnRoKCk7XG5cbiAgICBpZiAoa2V5ID09PSAnbGVmdCcpIHtcbiAgICAgIGRhdGUgPSBkYXRlIC0gMTsgICAvLyB1cFxuICAgIH0gZWxzZSBpZiAoa2V5ID09PSAndXAnKSB7XG4gICAgICBkYXRlID0gZGF0ZSAtIDM7ICAgLy8gZG93blxuICAgIH0gZWxzZSBpZiAoa2V5ID09PSAncmlnaHQnKSB7XG4gICAgICBkYXRlID0gZGF0ZSArIDE7ICAgLy8gZG93blxuICAgIH0gZWxzZSBpZiAoa2V5ID09PSAnZG93bicpIHtcbiAgICAgIGRhdGUgPSBkYXRlICsgMztcbiAgICB9IGVsc2UgaWYgKGtleSA9PT0gJ3BhZ2V1cCcgfHwga2V5ID09PSAncGFnZWRvd24nKSB7XG4gICAgICB2YXIgeWVhciA9IHRoaXMuYWN0aXZlRGF0ZS5nZXRGdWxsWWVhcigpICsgKGtleSA9PT0gJ3BhZ2V1cCcgPyAtIDEgOiAxKTtcbiAgICAgIHRoaXMuYWN0aXZlRGF0ZS5zZXRGdWxsWWVhcih5ZWFyKTtcbiAgICB9IGVsc2UgaWYgKGtleSA9PT0gJ2hvbWUnKSB7XG4gICAgICBkYXRlID0gMDtcbiAgICB9IGVsc2UgaWYgKGtleSA9PT0gJ2VuZCcpIHtcbiAgICAgIGRhdGUgPSAxMTtcbiAgICB9XG4gICAgdGhpcy5hY3RpdmVEYXRlLnNldE1vbnRoKGRhdGUpO1xuICB9O1xufV0pXG5cbi5jb250cm9sbGVyKCdVaWJZZWFycGlja2VyQ29udHJvbGxlcicsIFsnJHNjb3BlJywgJyRlbGVtZW50JywgJ2RhdGVGaWx0ZXInLCBmdW5jdGlvbihzY29wZSwgJGVsZW1lbnQsIGRhdGVGaWx0ZXIpIHtcbiAgdmFyIHJhbmdlO1xuICB0aGlzLmVsZW1lbnQgPSAkZWxlbWVudDtcblxuICBmdW5jdGlvbiBnZXRTdGFydGluZ1llYXIoeWVhcikge1xuICAgIHJldHVybiBwYXJzZUludCgoeWVhciAtIDEpIC8gcmFuZ2UsIDEwKSAqIHJhbmdlICsgMTtcbiAgfVxuXG4gIHRoaXMueWVhcnBpY2tlckluaXQgPSBmdW5jdGlvbigpIHtcbiAgICByYW5nZSA9IHRoaXMueWVhclJhbmdlO1xuICAgIHRoaXMuc3RlcCA9IHsgeWVhcnM6IHJhbmdlIH07XG4gIH07XG5cbiAgdGhpcy5fcmVmcmVzaFZpZXcgPSBmdW5jdGlvbigpIHtcbiAgICB2YXIgeWVhcnMgPSBuZXcgQXJyYXkocmFuZ2UpLCBkYXRlO1xuXG4gICAgZm9yICh2YXIgaSA9IDAsIHN0YXJ0ID0gZ2V0U3RhcnRpbmdZZWFyKHRoaXMuYWN0aXZlRGF0ZS5nZXRGdWxsWWVhcigpKTsgaSA8IHJhbmdlOyBpKyspIHtcbiAgICAgIGRhdGUgPSBuZXcgRGF0ZSh0aGlzLmFjdGl2ZURhdGUpO1xuICAgICAgZGF0ZS5zZXRGdWxsWWVhcihzdGFydCArIGksIDAsIDEpO1xuICAgICAgeWVhcnNbaV0gPSBhbmd1bGFyLmV4dGVuZCh0aGlzLmNyZWF0ZURhdGVPYmplY3QoZGF0ZSwgdGhpcy5mb3JtYXRZZWFyKSwge1xuICAgICAgICB1aWQ6IHNjb3BlLnVuaXF1ZUlkICsgJy0nICsgaVxuICAgICAgfSk7XG4gICAgfVxuXG4gICAgc2NvcGUudGl0bGUgPSBbeWVhcnNbMF0ubGFiZWwsIHllYXJzW3JhbmdlIC0gMV0ubGFiZWxdLmpvaW4oJyAtICcpO1xuICAgIHNjb3BlLnJvd3MgPSB0aGlzLnNwbGl0KHllYXJzLCA1KTtcbiAgfTtcblxuICB0aGlzLmNvbXBhcmUgPSBmdW5jdGlvbihkYXRlMSwgZGF0ZTIpIHtcbiAgICByZXR1cm4gZGF0ZTEuZ2V0RnVsbFllYXIoKSAtIGRhdGUyLmdldEZ1bGxZZWFyKCk7XG4gIH07XG5cbiAgdGhpcy5oYW5kbGVLZXlEb3duID0gZnVuY3Rpb24oa2V5LCBldnQpIHtcbiAgICB2YXIgZGF0ZSA9IHRoaXMuYWN0aXZlRGF0ZS5nZXRGdWxsWWVhcigpO1xuXG4gICAgaWYgKGtleSA9PT0gJ2xlZnQnKSB7XG4gICAgICBkYXRlID0gZGF0ZSAtIDE7ICAgLy8gdXBcbiAgICB9IGVsc2UgaWYgKGtleSA9PT0gJ3VwJykge1xuICAgICAgZGF0ZSA9IGRhdGUgLSA1OyAgIC8vIGRvd25cbiAgICB9IGVsc2UgaWYgKGtleSA9PT0gJ3JpZ2h0Jykge1xuICAgICAgZGF0ZSA9IGRhdGUgKyAxOyAgIC8vIGRvd25cbiAgICB9IGVsc2UgaWYgKGtleSA9PT0gJ2Rvd24nKSB7XG4gICAgICBkYXRlID0gZGF0ZSArIDU7XG4gICAgfSBlbHNlIGlmIChrZXkgPT09ICdwYWdldXAnIHx8IGtleSA9PT0gJ3BhZ2Vkb3duJykge1xuICAgICAgZGF0ZSArPSAoa2V5ID09PSAncGFnZXVwJyA/IC0gMSA6IDEpICogdGhpcy5zdGVwLnllYXJzO1xuICAgIH0gZWxzZSBpZiAoa2V5ID09PSAnaG9tZScpIHtcbiAgICAgIGRhdGUgPSBnZXRTdGFydGluZ1llYXIodGhpcy5hY3RpdmVEYXRlLmdldEZ1bGxZZWFyKCkpO1xuICAgIH0gZWxzZSBpZiAoa2V5ID09PSAnZW5kJykge1xuICAgICAgZGF0ZSA9IGdldFN0YXJ0aW5nWWVhcih0aGlzLmFjdGl2ZURhdGUuZ2V0RnVsbFllYXIoKSkgKyByYW5nZSAtIDE7XG4gICAgfVxuICAgIHRoaXMuYWN0aXZlRGF0ZS5zZXRGdWxsWWVhcihkYXRlKTtcbiAgfTtcbn1dKVxuXG4uZGlyZWN0aXZlKCd1aWJEYXRlcGlja2VyJywgZnVuY3Rpb24oKSB7XG4gIHJldHVybiB7XG4gICAgcmVwbGFjZTogdHJ1ZSxcbiAgICB0ZW1wbGF0ZVVybDogZnVuY3Rpb24oZWxlbWVudCwgYXR0cnMpIHtcbiAgICAgIHJldHVybiBhdHRycy50ZW1wbGF0ZVVybCB8fCAndGVtcGxhdGUvZGF0ZXBpY2tlci9kYXRlcGlja2VyLmh0bWwnO1xuICAgIH0sXG4gICAgc2NvcGU6IHtcbiAgICAgIGRhdGVwaWNrZXJNb2RlOiAnPT8nLFxuICAgICAgZGF0ZURpc2FibGVkOiAnJicsXG4gICAgICBjdXN0b21DbGFzczogJyYnLFxuICAgICAgc2hvcnRjdXRQcm9wYWdhdGlvbjogJyY/J1xuICAgIH0sXG4gICAgcmVxdWlyZTogWyd1aWJEYXRlcGlja2VyJywgJ15uZ01vZGVsJ10sXG4gICAgY29udHJvbGxlcjogJ1VpYkRhdGVwaWNrZXJDb250cm9sbGVyJyxcbiAgICBjb250cm9sbGVyQXM6ICdkYXRlcGlja2VyJyxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMsIGN0cmxzKSB7XG4gICAgICB2YXIgZGF0ZXBpY2tlckN0cmwgPSBjdHJsc1swXSwgbmdNb2RlbEN0cmwgPSBjdHJsc1sxXTtcblxuICAgICAgZGF0ZXBpY2tlckN0cmwuaW5pdChuZ01vZGVsQ3RybCk7XG4gICAgfVxuICB9O1xufSlcblxuLmRpcmVjdGl2ZSgndWliRGF5cGlja2VyJywgZnVuY3Rpb24oKSB7XG4gIHJldHVybiB7XG4gICAgcmVwbGFjZTogdHJ1ZSxcbiAgICB0ZW1wbGF0ZVVybDogZnVuY3Rpb24oZWxlbWVudCwgYXR0cnMpIHtcbiAgICAgIHJldHVybiBhdHRycy50ZW1wbGF0ZVVybCB8fCAndGVtcGxhdGUvZGF0ZXBpY2tlci9kYXkuaHRtbCc7XG4gICAgfSxcbiAgICByZXF1aXJlOiBbJ14/dWliRGF0ZXBpY2tlcicsICd1aWJEYXlwaWNrZXInLCAnXj9kYXRlcGlja2VyJ10sXG4gICAgY29udHJvbGxlcjogJ1VpYkRheXBpY2tlckNvbnRyb2xsZXInLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgY3RybHMpIHtcbiAgICAgIHZhciBkYXRlcGlja2VyQ3RybCA9IGN0cmxzWzBdIHx8IGN0cmxzWzJdLFxuICAgICAgICBkYXlwaWNrZXJDdHJsID0gY3RybHNbMV07XG5cbiAgICAgIGRheXBpY2tlckN0cmwuaW5pdChkYXRlcGlja2VyQ3RybCk7XG4gICAgfVxuICB9O1xufSlcblxuLmRpcmVjdGl2ZSgndWliTW9udGhwaWNrZXInLCBmdW5jdGlvbigpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHRlbXBsYXRlVXJsOiBmdW5jdGlvbihlbGVtZW50LCBhdHRycykge1xuICAgICAgcmV0dXJuIGF0dHJzLnRlbXBsYXRlVXJsIHx8ICd0ZW1wbGF0ZS9kYXRlcGlja2VyL21vbnRoLmh0bWwnO1xuICAgIH0sXG4gICAgcmVxdWlyZTogWydeP3VpYkRhdGVwaWNrZXInLCAndWliTW9udGhwaWNrZXInLCAnXj9kYXRlcGlja2VyJ10sXG4gICAgY29udHJvbGxlcjogJ1VpYk1vbnRocGlja2VyQ29udHJvbGxlcicsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHJzLCBjdHJscykge1xuICAgICAgdmFyIGRhdGVwaWNrZXJDdHJsID0gY3RybHNbMF0gfHwgY3RybHNbMl0sXG4gICAgICAgIG1vbnRocGlja2VyQ3RybCA9IGN0cmxzWzFdO1xuXG4gICAgICBtb250aHBpY2tlckN0cmwuaW5pdChkYXRlcGlja2VyQ3RybCk7XG4gICAgfVxuICB9O1xufSlcblxuLmRpcmVjdGl2ZSgndWliWWVhcnBpY2tlcicsIGZ1bmN0aW9uKCkge1xuICByZXR1cm4ge1xuICAgIHJlcGxhY2U6IHRydWUsXG4gICAgdGVtcGxhdGVVcmw6IGZ1bmN0aW9uKGVsZW1lbnQsIGF0dHJzKSB7XG4gICAgICByZXR1cm4gYXR0cnMudGVtcGxhdGVVcmwgfHwgJ3RlbXBsYXRlL2RhdGVwaWNrZXIveWVhci5odG1sJztcbiAgICB9LFxuICAgIHJlcXVpcmU6IFsnXj91aWJEYXRlcGlja2VyJywgJ3VpYlllYXJwaWNrZXInLCAnXj9kYXRlcGlja2VyJ10sXG4gICAgY29udHJvbGxlcjogJ1VpYlllYXJwaWNrZXJDb250cm9sbGVyJyxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMsIGN0cmxzKSB7XG4gICAgICB2YXIgY3RybCA9IGN0cmxzWzBdIHx8IGN0cmxzWzJdO1xuICAgICAgYW5ndWxhci5leHRlbmQoY3RybCwgY3RybHNbMV0pO1xuICAgICAgY3RybC55ZWFycGlja2VySW5pdCgpO1xuXG4gICAgICBjdHJsLnJlZnJlc2hWaWV3KCk7XG4gICAgfVxuICB9O1xufSlcblxuLmNvbnN0YW50KCd1aWJEYXRlcGlja2VyUG9wdXBDb25maWcnLCB7XG4gIGRhdGVwaWNrZXJQb3B1cDogJ3l5eXktTU0tZGQnLFxuICBkYXRlcGlja2VyUG9wdXBUZW1wbGF0ZVVybDogJ3RlbXBsYXRlL2RhdGVwaWNrZXIvcG9wdXAuaHRtbCcsXG4gIGRhdGVwaWNrZXJUZW1wbGF0ZVVybDogJ3RlbXBsYXRlL2RhdGVwaWNrZXIvZGF0ZXBpY2tlci5odG1sJyxcbiAgaHRtbDVUeXBlczoge1xuICAgIGRhdGU6ICd5eXl5LU1NLWRkJyxcbiAgICAnZGF0ZXRpbWUtbG9jYWwnOiAneXl5eS1NTS1kZFRISDptbTpzcy5zc3MnLFxuICAgICdtb250aCc6ICd5eXl5LU1NJ1xuICB9LFxuICBjdXJyZW50VGV4dDogJ1RvZGF5JyxcbiAgY2xlYXJUZXh0OiAnQ2xlYXInLFxuICBjbG9zZVRleHQ6ICdEb25lJyxcbiAgY2xvc2VPbkRhdGVTZWxlY3Rpb246IHRydWUsXG4gIGFwcGVuZFRvQm9keTogZmFsc2UsXG4gIHNob3dCdXR0b25CYXI6IHRydWUsXG4gIG9uT3BlbkZvY3VzOiB0cnVlXG59KVxuXG4uY29udHJvbGxlcignVWliRGF0ZXBpY2tlclBvcHVwQ29udHJvbGxlcicsIFsnJHNjb3BlJywgJyRlbGVtZW50JywgJyRhdHRycycsICckY29tcGlsZScsICckcGFyc2UnLCAnJGRvY3VtZW50JywgJyRyb290U2NvcGUnLCAnJHVpYlBvc2l0aW9uJywgJ2RhdGVGaWx0ZXInLCAndWliRGF0ZVBhcnNlcicsICd1aWJEYXRlcGlja2VyUG9wdXBDb25maWcnLCAnJHRpbWVvdXQnLFxuZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHJzLCAkY29tcGlsZSwgJHBhcnNlLCAkZG9jdW1lbnQsICRyb290U2NvcGUsICRwb3NpdGlvbiwgZGF0ZUZpbHRlciwgZGF0ZVBhcnNlciwgZGF0ZXBpY2tlclBvcHVwQ29uZmlnLCAkdGltZW91dCkge1xuICB2YXIgc2VsZiA9IHRoaXM7XG4gIHZhciBjYWNoZSA9IHt9LFxuICAgIGlzSHRtbDVEYXRlSW5wdXQgPSBmYWxzZTtcbiAgdmFyIGRhdGVGb3JtYXQsIGNsb3NlT25EYXRlU2VsZWN0aW9uLCBhcHBlbmRUb0JvZHksIG9uT3BlbkZvY3VzLFxuICAgIGRhdGVwaWNrZXJQb3B1cFRlbXBsYXRlVXJsLCBkYXRlcGlja2VyVGVtcGxhdGVVcmwsIHBvcHVwRWwsIGRhdGVwaWNrZXJFbCxcbiAgICBuZ01vZGVsLCAkcG9wdXA7XG5cbiAgc2NvcGUud2F0Y2hEYXRhID0ge307XG5cbiAgdGhpcy5pbml0ID0gZnVuY3Rpb24oX25nTW9kZWxfKSB7XG4gICAgbmdNb2RlbCA9IF9uZ01vZGVsXztcbiAgICBjbG9zZU9uRGF0ZVNlbGVjdGlvbiA9IGFuZ3VsYXIuaXNEZWZpbmVkKGF0dHJzLmNsb3NlT25EYXRlU2VsZWN0aW9uKSA/IHNjb3BlLiRwYXJlbnQuJGV2YWwoYXR0cnMuY2xvc2VPbkRhdGVTZWxlY3Rpb24pIDogZGF0ZXBpY2tlclBvcHVwQ29uZmlnLmNsb3NlT25EYXRlU2VsZWN0aW9uO1xuICAgIGFwcGVuZFRvQm9keSA9IGFuZ3VsYXIuaXNEZWZpbmVkKGF0dHJzLmRhdGVwaWNrZXJBcHBlbmRUb0JvZHkpID8gc2NvcGUuJHBhcmVudC4kZXZhbChhdHRycy5kYXRlcGlja2VyQXBwZW5kVG9Cb2R5KSA6IGRhdGVwaWNrZXJQb3B1cENvbmZpZy5hcHBlbmRUb0JvZHk7XG4gICAgb25PcGVuRm9jdXMgPSBhbmd1bGFyLmlzRGVmaW5lZChhdHRycy5vbk9wZW5Gb2N1cykgPyBzY29wZS4kcGFyZW50LiRldmFsKGF0dHJzLm9uT3BlbkZvY3VzKSA6IGRhdGVwaWNrZXJQb3B1cENvbmZpZy5vbk9wZW5Gb2N1cztcbiAgICBkYXRlcGlja2VyUG9wdXBUZW1wbGF0ZVVybCA9IGFuZ3VsYXIuaXNEZWZpbmVkKGF0dHJzLmRhdGVwaWNrZXJQb3B1cFRlbXBsYXRlVXJsKSA/IGF0dHJzLmRhdGVwaWNrZXJQb3B1cFRlbXBsYXRlVXJsIDogZGF0ZXBpY2tlclBvcHVwQ29uZmlnLmRhdGVwaWNrZXJQb3B1cFRlbXBsYXRlVXJsO1xuICAgIGRhdGVwaWNrZXJUZW1wbGF0ZVVybCA9IGFuZ3VsYXIuaXNEZWZpbmVkKGF0dHJzLmRhdGVwaWNrZXJUZW1wbGF0ZVVybCkgPyBhdHRycy5kYXRlcGlja2VyVGVtcGxhdGVVcmwgOiBkYXRlcGlja2VyUG9wdXBDb25maWcuZGF0ZXBpY2tlclRlbXBsYXRlVXJsO1xuXG4gICAgc2NvcGUuc2hvd0J1dHRvbkJhciA9IGFuZ3VsYXIuaXNEZWZpbmVkKGF0dHJzLnNob3dCdXR0b25CYXIpID8gc2NvcGUuJHBhcmVudC4kZXZhbChhdHRycy5zaG93QnV0dG9uQmFyKSA6IGRhdGVwaWNrZXJQb3B1cENvbmZpZy5zaG93QnV0dG9uQmFyO1xuXG4gICAgaWYgKGRhdGVwaWNrZXJQb3B1cENvbmZpZy5odG1sNVR5cGVzW2F0dHJzLnR5cGVdKSB7XG4gICAgICBkYXRlRm9ybWF0ID0gZGF0ZXBpY2tlclBvcHVwQ29uZmlnLmh0bWw1VHlwZXNbYXR0cnMudHlwZV07XG4gICAgICBpc0h0bWw1RGF0ZUlucHV0ID0gdHJ1ZTtcbiAgICB9IGVsc2Uge1xuICAgICAgZGF0ZUZvcm1hdCA9IGF0dHJzLmRhdGVwaWNrZXJQb3B1cCB8fCBhdHRycy51aWJEYXRlcGlja2VyUG9wdXAgfHwgZGF0ZXBpY2tlclBvcHVwQ29uZmlnLmRhdGVwaWNrZXJQb3B1cDtcbiAgICAgIGF0dHJzLiRvYnNlcnZlKCd1aWJEYXRlcGlja2VyUG9wdXAnLCBmdW5jdGlvbih2YWx1ZSwgb2xkVmFsdWUpIHtcbiAgICAgICAgICB2YXIgbmV3RGF0ZUZvcm1hdCA9IHZhbHVlIHx8IGRhdGVwaWNrZXJQb3B1cENvbmZpZy5kYXRlcGlja2VyUG9wdXA7XG4gICAgICAgICAgLy8gSW52YWxpZGF0ZSB0aGUgJG1vZGVsVmFsdWUgdG8gZW5zdXJlIHRoYXQgZm9ybWF0dGVycyByZS1ydW5cbiAgICAgICAgICAvLyBGSVhNRTogUmVmYWN0b3Igd2hlbiBQUiBpcyBtZXJnZWQ6IGh0dHBzOi8vZ2l0aHViLmNvbS9hbmd1bGFyL2FuZ3VsYXIuanMvcHVsbC8xMDc2NFxuICAgICAgICAgIGlmIChuZXdEYXRlRm9ybWF0ICE9PSBkYXRlRm9ybWF0KSB7XG4gICAgICAgICAgICBkYXRlRm9ybWF0ID0gbmV3RGF0ZUZvcm1hdDtcbiAgICAgICAgICAgIG5nTW9kZWwuJG1vZGVsVmFsdWUgPSBudWxsO1xuXG4gICAgICAgICAgICBpZiAoIWRhdGVGb3JtYXQpIHtcbiAgICAgICAgICAgICAgdGhyb3cgbmV3IEVycm9yKCd1aWJEYXRlcGlja2VyUG9wdXAgbXVzdCBoYXZlIGEgZGF0ZSBmb3JtYXQgc3BlY2lmaWVkLicpO1xuICAgICAgICAgICAgfVxuICAgICAgICAgIH1cbiAgICAgIH0pO1xuICAgIH1cblxuICAgIGlmICghZGF0ZUZvcm1hdCkge1xuICAgICAgdGhyb3cgbmV3IEVycm9yKCd1aWJEYXRlcGlja2VyUG9wdXAgbXVzdCBoYXZlIGEgZGF0ZSBmb3JtYXQgc3BlY2lmaWVkLicpO1xuICAgIH1cblxuICAgIGlmIChpc0h0bWw1RGF0ZUlucHV0ICYmIGF0dHJzLmRhdGVwaWNrZXJQb3B1cCkge1xuICAgICAgdGhyb3cgbmV3IEVycm9yKCdIVE1MNSBkYXRlIGlucHV0IHR5cGVzIGRvIG5vdCBzdXBwb3J0IGN1c3RvbSBmb3JtYXRzLicpO1xuICAgIH1cblxuICAgIC8vIHBvcHVwIGVsZW1lbnQgdXNlZCB0byBkaXNwbGF5IGNhbGVuZGFyXG4gICAgcG9wdXBFbCA9IGFuZ3VsYXIuZWxlbWVudCgnPGRpdiB1aWItZGF0ZXBpY2tlci1wb3B1cC13cmFwPjxkaXYgdWliLWRhdGVwaWNrZXI+PC9kaXY+PC9kaXY+Jyk7XG4gICAgcG9wdXBFbC5hdHRyKHtcbiAgICAgICduZy1tb2RlbCc6ICdkYXRlJyxcbiAgICAgICduZy1jaGFuZ2UnOiAnZGF0ZVNlbGVjdGlvbihkYXRlKScsXG4gICAgICAndGVtcGxhdGUtdXJsJzogZGF0ZXBpY2tlclBvcHVwVGVtcGxhdGVVcmxcbiAgICB9KTtcblxuICAgIC8vIGRhdGVwaWNrZXIgZWxlbWVudFxuICAgIGRhdGVwaWNrZXJFbCA9IGFuZ3VsYXIuZWxlbWVudChwb3B1cEVsLmNoaWxkcmVuKClbMF0pO1xuICAgIGRhdGVwaWNrZXJFbC5hdHRyKCd0ZW1wbGF0ZS11cmwnLCBkYXRlcGlja2VyVGVtcGxhdGVVcmwpO1xuXG4gICAgaWYgKGlzSHRtbDVEYXRlSW5wdXQpIHtcbiAgICAgIGlmIChhdHRycy50eXBlID09PSAnbW9udGgnKSB7XG4gICAgICAgIGRhdGVwaWNrZXJFbC5hdHRyKCdkYXRlcGlja2VyLW1vZGUnLCAnXCJtb250aFwiJyk7XG4gICAgICAgIGRhdGVwaWNrZXJFbC5hdHRyKCdtaW4tbW9kZScsICdtb250aCcpO1xuICAgICAgfVxuICAgIH1cblxuICAgIGlmIChhdHRycy5kYXRlcGlja2VyT3B0aW9ucykge1xuICAgICAgdmFyIG9wdGlvbnMgPSBzY29wZS4kcGFyZW50LiRldmFsKGF0dHJzLmRhdGVwaWNrZXJPcHRpb25zKTtcbiAgICAgIGlmIChvcHRpb25zICYmIG9wdGlvbnMuaW5pdERhdGUpIHtcbiAgICAgICAgc2NvcGUuaW5pdERhdGUgPSBvcHRpb25zLmluaXREYXRlO1xuICAgICAgICBkYXRlcGlja2VyRWwuYXR0cignaW5pdC1kYXRlJywgJ2luaXREYXRlJyk7XG4gICAgICAgIGRlbGV0ZSBvcHRpb25zLmluaXREYXRlO1xuICAgICAgfVxuICAgICAgYW5ndWxhci5mb3JFYWNoKG9wdGlvbnMsIGZ1bmN0aW9uKHZhbHVlLCBvcHRpb24pIHtcbiAgICAgICAgZGF0ZXBpY2tlckVsLmF0dHIoY2FtZWx0b0Rhc2gob3B0aW9uKSwgdmFsdWUpO1xuICAgICAgfSk7XG4gICAgfVxuXG4gICAgYW5ndWxhci5mb3JFYWNoKFsnbWluTW9kZScsICdtYXhNb2RlJywgJ21pbkRhdGUnLCAnbWF4RGF0ZScsICdkYXRlcGlja2VyTW9kZScsICdpbml0RGF0ZScsICdzaG9ydGN1dFByb3BhZ2F0aW9uJ10sIGZ1bmN0aW9uKGtleSkge1xuICAgICAgaWYgKGF0dHJzW2tleV0pIHtcbiAgICAgICAgdmFyIGdldEF0dHJpYnV0ZSA9ICRwYXJzZShhdHRyc1trZXldKTtcbiAgICAgICAgc2NvcGUuJHBhcmVudC4kd2F0Y2goZ2V0QXR0cmlidXRlLCBmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgICAgIHNjb3BlLndhdGNoRGF0YVtrZXldID0gdmFsdWU7XG4gICAgICAgICAgaWYgKGtleSA9PT0gJ21pbkRhdGUnIHx8IGtleSA9PT0gJ21heERhdGUnKSB7XG4gICAgICAgICAgICBjYWNoZVtrZXldID0gbmV3IERhdGUodmFsdWUpO1xuICAgICAgICAgIH1cbiAgICAgICAgfSk7XG4gICAgICAgIGRhdGVwaWNrZXJFbC5hdHRyKGNhbWVsdG9EYXNoKGtleSksICd3YXRjaERhdGEuJyArIGtleSk7XG5cbiAgICAgICAgLy8gUHJvcGFnYXRlIGNoYW5nZXMgZnJvbSBkYXRlcGlja2VyIHRvIG91dHNpZGVcbiAgICAgICAgaWYgKGtleSA9PT0gJ2RhdGVwaWNrZXJNb2RlJykge1xuICAgICAgICAgIHZhciBzZXRBdHRyaWJ1dGUgPSBnZXRBdHRyaWJ1dGUuYXNzaWduO1xuICAgICAgICAgIHNjb3BlLiR3YXRjaCgnd2F0Y2hEYXRhLicgKyBrZXksIGZ1bmN0aW9uKHZhbHVlLCBvbGR2YWx1ZSkge1xuICAgICAgICAgICAgaWYgKGFuZ3VsYXIuaXNGdW5jdGlvbihzZXRBdHRyaWJ1dGUpICYmIHZhbHVlICE9PSBvbGR2YWx1ZSkge1xuICAgICAgICAgICAgICBzZXRBdHRyaWJ1dGUoc2NvcGUuJHBhcmVudCwgdmFsdWUpO1xuICAgICAgICAgICAgfVxuICAgICAgICAgIH0pO1xuICAgICAgICB9XG4gICAgICB9XG4gICAgfSk7XG4gICAgaWYgKGF0dHJzLmRhdGVEaXNhYmxlZCkge1xuICAgICAgZGF0ZXBpY2tlckVsLmF0dHIoJ2RhdGUtZGlzYWJsZWQnLCAnZGF0ZURpc2FibGVkKHsgZGF0ZTogZGF0ZSwgbW9kZTogbW9kZSB9KScpO1xuICAgIH1cblxuICAgIGlmIChhdHRycy5zaG93V2Vla3MpIHtcbiAgICAgIGRhdGVwaWNrZXJFbC5hdHRyKCdzaG93LXdlZWtzJywgYXR0cnMuc2hvd1dlZWtzKTtcbiAgICB9XG5cbiAgICBpZiAoYXR0cnMuY3VzdG9tQ2xhc3MpIHtcbiAgICAgIGRhdGVwaWNrZXJFbC5hdHRyKCdjdXN0b20tY2xhc3MnLCAnY3VzdG9tQ2xhc3MoeyBkYXRlOiBkYXRlLCBtb2RlOiBtb2RlIH0pJyk7XG4gICAgfVxuXG4gICAgaWYgKCFpc0h0bWw1RGF0ZUlucHV0KSB7XG4gICAgICAvLyBJbnRlcm5hbCBBUEkgdG8gbWFpbnRhaW4gdGhlIGNvcnJlY3QgbmctaW52YWxpZC1ba2V5XSBjbGFzc1xuICAgICAgbmdNb2RlbC4kJHBhcnNlck5hbWUgPSAnZGF0ZSc7XG4gICAgICBuZ01vZGVsLiR2YWxpZGF0b3JzLmRhdGUgPSB2YWxpZGF0b3I7XG4gICAgICBuZ01vZGVsLiRwYXJzZXJzLnVuc2hpZnQocGFyc2VEYXRlKTtcbiAgICAgIG5nTW9kZWwuJGZvcm1hdHRlcnMucHVzaChmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgICBzY29wZS5kYXRlID0gdmFsdWU7XG4gICAgICAgIHJldHVybiBuZ01vZGVsLiRpc0VtcHR5KHZhbHVlKSA/IHZhbHVlIDogZGF0ZUZpbHRlcih2YWx1ZSwgZGF0ZUZvcm1hdCk7XG4gICAgICB9KTtcbiAgICB9IGVsc2Uge1xuICAgICAgbmdNb2RlbC4kZm9ybWF0dGVycy5wdXNoKGZ1bmN0aW9uKHZhbHVlKSB7XG4gICAgICAgIHNjb3BlLmRhdGUgPSB2YWx1ZTtcbiAgICAgICAgcmV0dXJuIHZhbHVlO1xuICAgICAgfSk7XG4gICAgfVxuXG4gICAgLy8gRGV0ZWN0IGNoYW5nZXMgaW4gdGhlIHZpZXcgZnJvbSB0aGUgdGV4dCBib3hcbiAgICBuZ01vZGVsLiR2aWV3Q2hhbmdlTGlzdGVuZXJzLnB1c2goZnVuY3Rpb24oKSB7XG4gICAgICBzY29wZS5kYXRlID0gZGF0ZVBhcnNlci5wYXJzZShuZ01vZGVsLiR2aWV3VmFsdWUsIGRhdGVGb3JtYXQsIHNjb3BlLmRhdGUpO1xuICAgIH0pO1xuXG4gICAgZWxlbWVudC5iaW5kKCdrZXlkb3duJywgaW5wdXRLZXlkb3duQmluZCk7XG5cbiAgICAkcG9wdXAgPSAkY29tcGlsZShwb3B1cEVsKShzY29wZSk7XG4gICAgLy8gUHJldmVudCBqUXVlcnkgY2FjaGUgbWVtb3J5IGxlYWsgKHRlbXBsYXRlIGlzIG5vdyByZWR1bmRhbnQgYWZ0ZXIgbGlua2luZylcbiAgICBwb3B1cEVsLnJlbW92ZSgpO1xuXG4gICAgaWYgKGFwcGVuZFRvQm9keSkge1xuICAgICAgJGRvY3VtZW50LmZpbmQoJ2JvZHknKS5hcHBlbmQoJHBvcHVwKTtcbiAgICB9IGVsc2Uge1xuICAgICAgZWxlbWVudC5hZnRlcigkcG9wdXApO1xuICAgIH1cblxuICAgIHNjb3BlLiRvbignJGRlc3Ryb3knLCBmdW5jdGlvbigpIHtcbiAgICAgIGlmIChzY29wZS5pc09wZW4gPT09IHRydWUpIHtcbiAgICAgICAgaWYgKCEkcm9vdFNjb3BlLiQkcGhhc2UpIHtcbiAgICAgICAgICBzY29wZS4kYXBwbHkoZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICBzY29wZS5pc09wZW4gPSBmYWxzZTtcbiAgICAgICAgICB9KTtcbiAgICAgICAgfVxuICAgICAgfVxuXG4gICAgICAkcG9wdXAucmVtb3ZlKCk7XG4gICAgICBlbGVtZW50LnVuYmluZCgna2V5ZG93bicsIGlucHV0S2V5ZG93bkJpbmQpO1xuICAgICAgJGRvY3VtZW50LnVuYmluZCgnY2xpY2snLCBkb2N1bWVudENsaWNrQmluZCk7XG4gICAgfSk7XG4gIH07XG5cbiAgc2NvcGUuZ2V0VGV4dCA9IGZ1bmN0aW9uKGtleSkge1xuICAgIHJldHVybiBzY29wZVtrZXkgKyAnVGV4dCddIHx8IGRhdGVwaWNrZXJQb3B1cENvbmZpZ1trZXkgKyAnVGV4dCddO1xuICB9O1xuXG4gIHNjb3BlLmlzRGlzYWJsZWQgPSBmdW5jdGlvbihkYXRlKSB7XG4gICAgaWYgKGRhdGUgPT09ICd0b2RheScpIHtcbiAgICAgIGRhdGUgPSBuZXcgRGF0ZSgpO1xuICAgIH1cblxuICAgIHJldHVybiAoKHNjb3BlLndhdGNoRGF0YS5taW5EYXRlICYmIHNjb3BlLmNvbXBhcmUoZGF0ZSwgY2FjaGUubWluRGF0ZSkgPCAwKSB8fFxuICAgICAgKHNjb3BlLndhdGNoRGF0YS5tYXhEYXRlICYmIHNjb3BlLmNvbXBhcmUoZGF0ZSwgY2FjaGUubWF4RGF0ZSkgPiAwKSk7XG4gIH07XG5cbiAgc2NvcGUuY29tcGFyZSA9IGZ1bmN0aW9uKGRhdGUxLCBkYXRlMikge1xuICAgIHJldHVybiAobmV3IERhdGUoZGF0ZTEuZ2V0RnVsbFllYXIoKSwgZGF0ZTEuZ2V0TW9udGgoKSwgZGF0ZTEuZ2V0RGF0ZSgpKSAtIG5ldyBEYXRlKGRhdGUyLmdldEZ1bGxZZWFyKCksIGRhdGUyLmdldE1vbnRoKCksIGRhdGUyLmdldERhdGUoKSkpO1xuICB9O1xuXG4gIC8vIElubmVyIGNoYW5nZVxuICBzY29wZS5kYXRlU2VsZWN0aW9uID0gZnVuY3Rpb24oZHQpIHtcbiAgICBpZiAoYW5ndWxhci5pc0RlZmluZWQoZHQpKSB7XG4gICAgICBzY29wZS5kYXRlID0gZHQ7XG4gICAgfVxuICAgIHZhciBkYXRlID0gc2NvcGUuZGF0ZSA/IGRhdGVGaWx0ZXIoc2NvcGUuZGF0ZSwgZGF0ZUZvcm1hdCkgOiBudWxsOyAvLyBTZXR0aW5nIHRvIE5VTEwgaXMgbmVjZXNzYXJ5IGZvciBmb3JtIHZhbGlkYXRvcnMgdG8gZnVuY3Rpb25cbiAgICBlbGVtZW50LnZhbChkYXRlKTtcbiAgICBuZ01vZGVsLiRzZXRWaWV3VmFsdWUoZGF0ZSk7XG5cbiAgICBpZiAoY2xvc2VPbkRhdGVTZWxlY3Rpb24pIHtcbiAgICAgIHNjb3BlLmlzT3BlbiA9IGZhbHNlO1xuICAgICAgZWxlbWVudFswXS5mb2N1cygpO1xuICAgIH1cbiAgfTtcblxuICBzY29wZS5rZXlkb3duID0gZnVuY3Rpb24oZXZ0KSB7XG4gICAgaWYgKGV2dC53aGljaCA9PT0gMjcpIHtcbiAgICAgIHNjb3BlLmlzT3BlbiA9IGZhbHNlO1xuICAgICAgZWxlbWVudFswXS5mb2N1cygpO1xuICAgIH1cbiAgfTtcblxuICBzY29wZS5zZWxlY3QgPSBmdW5jdGlvbihkYXRlKSB7XG4gICAgaWYgKGRhdGUgPT09ICd0b2RheScpIHtcbiAgICAgIHZhciB0b2RheSA9IG5ldyBEYXRlKCk7XG4gICAgICBpZiAoYW5ndWxhci5pc0RhdGUoc2NvcGUuZGF0ZSkpIHtcbiAgICAgICAgZGF0ZSA9IG5ldyBEYXRlKHNjb3BlLmRhdGUpO1xuICAgICAgICBkYXRlLnNldEZ1bGxZZWFyKHRvZGF5LmdldEZ1bGxZZWFyKCksIHRvZGF5LmdldE1vbnRoKCksIHRvZGF5LmdldERhdGUoKSk7XG4gICAgICB9IGVsc2Uge1xuICAgICAgICBkYXRlID0gbmV3IERhdGUodG9kYXkuc2V0SG91cnMoMCwgMCwgMCwgMCkpO1xuICAgICAgfVxuICAgIH1cbiAgICBzY29wZS5kYXRlU2VsZWN0aW9uKGRhdGUpO1xuICB9O1xuXG4gIHNjb3BlLmNsb3NlID0gZnVuY3Rpb24oKSB7XG4gICAgc2NvcGUuaXNPcGVuID0gZmFsc2U7XG4gICAgZWxlbWVudFswXS5mb2N1cygpO1xuICB9O1xuXG4gIHNjb3BlLiR3YXRjaCgnaXNPcGVuJywgZnVuY3Rpb24odmFsdWUpIHtcbiAgICBpZiAodmFsdWUpIHtcbiAgICAgIHNjb3BlLnBvc2l0aW9uID0gYXBwZW5kVG9Cb2R5ID8gJHBvc2l0aW9uLm9mZnNldChlbGVtZW50KSA6ICRwb3NpdGlvbi5wb3NpdGlvbihlbGVtZW50KTtcbiAgICAgIHNjb3BlLnBvc2l0aW9uLnRvcCA9IHNjb3BlLnBvc2l0aW9uLnRvcCArIGVsZW1lbnQucHJvcCgnb2Zmc2V0SGVpZ2h0Jyk7XG5cbiAgICAgICR0aW1lb3V0KGZ1bmN0aW9uKCkge1xuICAgICAgICBpZiAob25PcGVuRm9jdXMpIHtcbiAgICAgICAgICBzY29wZS4kYnJvYWRjYXN0KCd1aWI6ZGF0ZXBpY2tlci5mb2N1cycpO1xuICAgICAgICB9XG4gICAgICAgICRkb2N1bWVudC5iaW5kKCdjbGljaycsIGRvY3VtZW50Q2xpY2tCaW5kKTtcbiAgICAgIH0sIDAsIGZhbHNlKTtcbiAgICB9IGVsc2Uge1xuICAgICAgJGRvY3VtZW50LnVuYmluZCgnY2xpY2snLCBkb2N1bWVudENsaWNrQmluZCk7XG4gICAgfVxuICB9KTtcblxuICBmdW5jdGlvbiBjYW1lbHRvRGFzaChzdHJpbmcpIHtcbiAgICByZXR1cm4gc3RyaW5nLnJlcGxhY2UoLyhbQS1aXSkvZywgZnVuY3Rpb24oJDEpIHsgcmV0dXJuICctJyArICQxLnRvTG93ZXJDYXNlKCk7IH0pO1xuICB9XG5cbiAgZnVuY3Rpb24gcGFyc2VEYXRlKHZpZXdWYWx1ZSkge1xuICAgIGlmIChhbmd1bGFyLmlzTnVtYmVyKHZpZXdWYWx1ZSkpIHtcbiAgICAgIC8vIHByZXN1bWFibHkgdGltZXN0YW1wIHRvIGRhdGUgb2JqZWN0XG4gICAgICB2aWV3VmFsdWUgPSBuZXcgRGF0ZSh2aWV3VmFsdWUpO1xuICAgIH1cblxuICAgIGlmICghdmlld1ZhbHVlKSB7XG4gICAgICByZXR1cm4gbnVsbDtcbiAgICB9IGVsc2UgaWYgKGFuZ3VsYXIuaXNEYXRlKHZpZXdWYWx1ZSkgJiYgIWlzTmFOKHZpZXdWYWx1ZSkpIHtcbiAgICAgIHJldHVybiB2aWV3VmFsdWU7XG4gICAgfSBlbHNlIGlmIChhbmd1bGFyLmlzU3RyaW5nKHZpZXdWYWx1ZSkpIHtcbiAgICAgIHZhciBkYXRlID0gZGF0ZVBhcnNlci5wYXJzZSh2aWV3VmFsdWUsIGRhdGVGb3JtYXQsIHNjb3BlLmRhdGUpO1xuICAgICAgaWYgKGlzTmFOKGRhdGUpKSB7XG4gICAgICAgIHJldHVybiB1bmRlZmluZWQ7XG4gICAgICB9IGVsc2Uge1xuICAgICAgICByZXR1cm4gZGF0ZTtcbiAgICAgIH1cbiAgICB9IGVsc2Uge1xuICAgICAgcmV0dXJuIHVuZGVmaW5lZDtcbiAgICB9XG4gIH1cblxuICBmdW5jdGlvbiB2YWxpZGF0b3IobW9kZWxWYWx1ZSwgdmlld1ZhbHVlKSB7XG4gICAgdmFyIHZhbHVlID0gbW9kZWxWYWx1ZSB8fCB2aWV3VmFsdWU7XG5cbiAgICBpZiAoIWF0dHJzLm5nUmVxdWlyZWQgJiYgIXZhbHVlKSB7XG4gICAgICByZXR1cm4gdHJ1ZTtcbiAgICB9XG5cbiAgICBpZiAoYW5ndWxhci5pc051bWJlcih2YWx1ZSkpIHtcbiAgICAgIHZhbHVlID0gbmV3IERhdGUodmFsdWUpO1xuICAgIH1cbiAgICBpZiAoIXZhbHVlKSB7XG4gICAgICByZXR1cm4gdHJ1ZTtcbiAgICB9IGVsc2UgaWYgKGFuZ3VsYXIuaXNEYXRlKHZhbHVlKSAmJiAhaXNOYU4odmFsdWUpKSB7XG4gICAgICByZXR1cm4gdHJ1ZTtcbiAgICB9IGVsc2UgaWYgKGFuZ3VsYXIuaXNTdHJpbmcodmFsdWUpKSB7XG4gICAgICB2YXIgZGF0ZSA9IGRhdGVQYXJzZXIucGFyc2UodmFsdWUsIGRhdGVGb3JtYXQpO1xuICAgICAgcmV0dXJuICFpc05hTihkYXRlKTtcbiAgICB9IGVsc2Uge1xuICAgICAgcmV0dXJuIGZhbHNlO1xuICAgIH1cbiAgfVxuXG4gIGZ1bmN0aW9uIGRvY3VtZW50Q2xpY2tCaW5kKGV2ZW50KSB7XG4gICAgdmFyIHBvcHVwID0gJHBvcHVwWzBdO1xuICAgIHZhciBkcENvbnRhaW5zVGFyZ2V0ID0gZWxlbWVudFswXS5jb250YWlucyhldmVudC50YXJnZXQpO1xuICAgIC8vIFRoZSBwb3B1cCBub2RlIG1heSBub3QgYmUgYW4gZWxlbWVudCBub2RlXG4gICAgLy8gSW4gc29tZSBicm93c2VycyAoSUUpIG9ubHkgZWxlbWVudCBub2RlcyBoYXZlIHRoZSAnY29udGFpbnMnIGZ1bmN0aW9uXG4gICAgdmFyIHBvcHVwQ29udGFpbnNUYXJnZXQgPSBwb3B1cC5jb250YWlucyAhPT0gdW5kZWZpbmVkICYmIHBvcHVwLmNvbnRhaW5zKGV2ZW50LnRhcmdldCk7XG4gICAgaWYgKHNjb3BlLmlzT3BlbiAmJiAhKGRwQ29udGFpbnNUYXJnZXQgfHwgcG9wdXBDb250YWluc1RhcmdldCkpIHtcbiAgICAgIHNjb3BlLiRhcHBseShmdW5jdGlvbigpIHtcbiAgICAgICAgc2NvcGUuaXNPcGVuID0gZmFsc2U7XG4gICAgICB9KTtcbiAgICB9XG4gIH1cblxuICBmdW5jdGlvbiBpbnB1dEtleWRvd25CaW5kKGV2dCkge1xuICAgIGlmIChldnQud2hpY2ggPT09IDI3ICYmIHNjb3BlLmlzT3Blbikge1xuICAgICAgZXZ0LnByZXZlbnREZWZhdWx0KCk7XG4gICAgICBldnQuc3RvcFByb3BhZ2F0aW9uKCk7XG4gICAgICBzY29wZS4kYXBwbHkoZnVuY3Rpb24oKSB7XG4gICAgICAgIHNjb3BlLmlzT3BlbiA9IGZhbHNlO1xuICAgICAgfSk7XG4gICAgICBlbGVtZW50WzBdLmZvY3VzKCk7XG4gICAgfSBlbHNlIGlmIChldnQud2hpY2ggPT09IDQwICYmICFzY29wZS5pc09wZW4pIHtcbiAgICAgIGV2dC5wcmV2ZW50RGVmYXVsdCgpO1xuICAgICAgZXZ0LnN0b3BQcm9wYWdhdGlvbigpO1xuICAgICAgc2NvcGUuJGFwcGx5KGZ1bmN0aW9uKCkge1xuICAgICAgICBzY29wZS5pc09wZW4gPSB0cnVlO1xuICAgICAgfSk7XG4gICAgfVxuICB9XG59XSlcblxuLmRpcmVjdGl2ZSgndWliRGF0ZXBpY2tlclBvcHVwJywgZnVuY3Rpb24oKSB7XG4gIHJldHVybiB7XG4gICAgcmVxdWlyZTogWyduZ01vZGVsJywgJ3VpYkRhdGVwaWNrZXJQb3B1cCddLFxuICAgIGNvbnRyb2xsZXI6ICdVaWJEYXRlcGlja2VyUG9wdXBDb250cm9sbGVyJyxcbiAgICBzY29wZToge1xuICAgICAgaXNPcGVuOiAnPT8nLFxuICAgICAgY3VycmVudFRleHQ6ICdAJyxcbiAgICAgIGNsZWFyVGV4dDogJ0AnLFxuICAgICAgY2xvc2VUZXh0OiAnQCcsXG4gICAgICBkYXRlRGlzYWJsZWQ6ICcmJyxcbiAgICAgIGN1c3RvbUNsYXNzOiAnJidcbiAgICB9LFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgY3RybHMpIHtcbiAgICAgIHZhciBuZ01vZGVsID0gY3RybHNbMF0sXG4gICAgICAgIGN0cmwgPSBjdHJsc1sxXTtcblxuICAgICAgY3RybC5pbml0KG5nTW9kZWwpO1xuICAgIH1cbiAgfTtcbn0pXG5cbi5kaXJlY3RpdmUoJ3VpYkRhdGVwaWNrZXJQb3B1cFdyYXAnLCBmdW5jdGlvbigpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHRyYW5zY2x1ZGU6IHRydWUsXG4gICAgdGVtcGxhdGVVcmw6IGZ1bmN0aW9uKGVsZW1lbnQsIGF0dHJzKSB7XG4gICAgICByZXR1cm4gYXR0cnMudGVtcGxhdGVVcmwgfHwgJ3RlbXBsYXRlL2RhdGVwaWNrZXIvcG9wdXAuaHRtbCc7XG4gICAgfVxuICB9O1xufSk7XG5cbi8qIERlcHJlY2F0ZWQgZGF0ZXBpY2tlciBiZWxvdyAqL1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLmRhdGVwaWNrZXInKVxuXG4udmFsdWUoJyRkYXRlcGlja2VyU3VwcHJlc3NXYXJuaW5nJywgZmFsc2UpXG5cbi5jb250cm9sbGVyKCdEYXRlcGlja2VyQ29udHJvbGxlcicsIFsnJHNjb3BlJywgJyRhdHRycycsICckcGFyc2UnLCAnJGludGVycG9sYXRlJywgJyRsb2cnLCAnZGF0ZUZpbHRlcicsICd1aWJEYXRlcGlja2VyQ29uZmlnJywgJyRkYXRlcGlja2VyU3VwcHJlc3NFcnJvcicsICckZGF0ZXBpY2tlclN1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRzY29wZSwgJGF0dHJzLCAkcGFyc2UsICRpbnRlcnBvbGF0ZSwgJGxvZywgZGF0ZUZpbHRlciwgZGF0ZXBpY2tlckNvbmZpZywgJGRhdGVwaWNrZXJTdXBwcmVzc0Vycm9yLCAkZGF0ZXBpY2tlclN1cHByZXNzV2FybmluZykge1xuICBpZiAoISRkYXRlcGlja2VyU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgJGxvZy53YXJuKCdEYXRlcGlja2VyQ29udHJvbGxlciBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIFVpYkRhdGVwaWNrZXJDb250cm9sbGVyIGluc3RlYWQuJyk7XG4gIH1cblxuICB2YXIgc2VsZiA9IHRoaXMsXG4gICAgbmdNb2RlbEN0cmwgPSB7ICRzZXRWaWV3VmFsdWU6IGFuZ3VsYXIubm9vcCB9OyAvLyBudWxsTW9kZWxDdHJsO1xuXG4gIHRoaXMubW9kZXMgPSBbJ2RheScsICdtb250aCcsICd5ZWFyJ107XG5cbiAgYW5ndWxhci5mb3JFYWNoKFsnZm9ybWF0RGF5JywgJ2Zvcm1hdE1vbnRoJywgJ2Zvcm1hdFllYXInLCAnZm9ybWF0RGF5SGVhZGVyJywgJ2Zvcm1hdERheVRpdGxlJywgJ2Zvcm1hdE1vbnRoVGl0bGUnLFxuICAgICdzaG93V2Vla3MnLCAnc3RhcnRpbmdEYXknLCAneWVhclJhbmdlJywgJ3Nob3J0Y3V0UHJvcGFnYXRpb24nXSwgZnVuY3Rpb24oa2V5LCBpbmRleCkge1xuICAgIHNlbGZba2V5XSA9IGFuZ3VsYXIuaXNEZWZpbmVkKCRhdHRyc1trZXldKSA/IChpbmRleCA8IDYgPyAkaW50ZXJwb2xhdGUoJGF0dHJzW2tleV0pKCRzY29wZS4kcGFyZW50KSA6ICRzY29wZS4kcGFyZW50LiRldmFsKCRhdHRyc1trZXldKSkgOiBkYXRlcGlja2VyQ29uZmlnW2tleV07XG4gIH0pO1xuXG4gIGFuZ3VsYXIuZm9yRWFjaChbJ21pbkRhdGUnLCAnbWF4RGF0ZSddLCBmdW5jdGlvbihrZXkpIHtcbiAgICBpZiAoJGF0dHJzW2tleV0pIHtcbiAgICAgICRzY29wZS4kcGFyZW50LiR3YXRjaCgkcGFyc2UoJGF0dHJzW2tleV0pLCBmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgICBzZWxmW2tleV0gPSB2YWx1ZSA/IG5ldyBEYXRlKHZhbHVlKSA6IG51bGw7XG4gICAgICAgIHNlbGYucmVmcmVzaFZpZXcoKTtcbiAgICAgIH0pO1xuICAgIH0gZWxzZSB7XG4gICAgICBzZWxmW2tleV0gPSBkYXRlcGlja2VyQ29uZmlnW2tleV0gPyBuZXcgRGF0ZShkYXRlcGlja2VyQ29uZmlnW2tleV0pIDogbnVsbDtcbiAgICB9XG4gIH0pO1xuXG4gIGFuZ3VsYXIuZm9yRWFjaChbJ21pbk1vZGUnLCAnbWF4TW9kZSddLCBmdW5jdGlvbihrZXkpIHtcbiAgICBpZiAoJGF0dHJzW2tleV0pIHtcbiAgICAgICRzY29wZS4kcGFyZW50LiR3YXRjaCgkcGFyc2UoJGF0dHJzW2tleV0pLCBmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgICBzZWxmW2tleV0gPSBhbmd1bGFyLmlzRGVmaW5lZCh2YWx1ZSkgPyB2YWx1ZSA6ICRhdHRyc1trZXldO1xuICAgICAgICAkc2NvcGVba2V5XSA9IHNlbGZba2V5XTtcbiAgICAgICAgaWYgKChrZXkgPT0gJ21pbk1vZGUnICYmIHNlbGYubW9kZXMuaW5kZXhPZigkc2NvcGUuZGF0ZXBpY2tlck1vZGUpIDwgc2VsZi5tb2Rlcy5pbmRleE9mKHNlbGZba2V5XSkpIHx8IChrZXkgPT0gJ21heE1vZGUnICYmIHNlbGYubW9kZXMuaW5kZXhPZigkc2NvcGUuZGF0ZXBpY2tlck1vZGUpID4gc2VsZi5tb2Rlcy5pbmRleE9mKHNlbGZba2V5XSkpKSB7XG4gICAgICAgICAgJHNjb3BlLmRhdGVwaWNrZXJNb2RlID0gc2VsZltrZXldO1xuICAgICAgICB9XG4gICAgICB9KTtcbiAgICB9IGVsc2Uge1xuICAgICAgc2VsZltrZXldID0gZGF0ZXBpY2tlckNvbmZpZ1trZXldIHx8IG51bGw7XG4gICAgICAkc2NvcGVba2V5XSA9IHNlbGZba2V5XTtcbiAgICB9XG4gIH0pO1xuXG4gICRzY29wZS5kYXRlcGlja2VyTW9kZSA9ICRzY29wZS5kYXRlcGlja2VyTW9kZSB8fCBkYXRlcGlja2VyQ29uZmlnLmRhdGVwaWNrZXJNb2RlO1xuICAkc2NvcGUudW5pcXVlSWQgPSAnZGF0ZXBpY2tlci0nICsgJHNjb3BlLiRpZCArICctJyArIE1hdGguZmxvb3IoTWF0aC5yYW5kb20oKSAqIDEwMDAwKTtcblxuICBpZiAoYW5ndWxhci5pc0RlZmluZWQoJGF0dHJzLmluaXREYXRlKSkge1xuICAgIHRoaXMuYWN0aXZlRGF0ZSA9ICRzY29wZS4kcGFyZW50LiRldmFsKCRhdHRycy5pbml0RGF0ZSkgfHwgbmV3IERhdGUoKTtcbiAgICAkc2NvcGUuJHBhcmVudC4kd2F0Y2goJGF0dHJzLmluaXREYXRlLCBmdW5jdGlvbihpbml0RGF0ZSkge1xuICAgICAgaWYgKGluaXREYXRlICYmIChuZ01vZGVsQ3RybC4kaXNFbXB0eShuZ01vZGVsQ3RybC4kbW9kZWxWYWx1ZSkgfHwgbmdNb2RlbEN0cmwuJGludmFsaWQpKSB7XG4gICAgICAgIHNlbGYuYWN0aXZlRGF0ZSA9IGluaXREYXRlO1xuICAgICAgICBzZWxmLnJlZnJlc2hWaWV3KCk7XG4gICAgICB9XG4gICAgfSk7XG4gIH0gZWxzZSB7XG4gICAgdGhpcy5hY3RpdmVEYXRlID0gbmV3IERhdGUoKTtcbiAgfVxuXG4gICRzY29wZS5pc0FjdGl2ZSA9IGZ1bmN0aW9uKGRhdGVPYmplY3QpIHtcbiAgICBpZiAoc2VsZi5jb21wYXJlKGRhdGVPYmplY3QuZGF0ZSwgc2VsZi5hY3RpdmVEYXRlKSA9PT0gMCkge1xuICAgICAgJHNjb3BlLmFjdGl2ZURhdGVJZCA9IGRhdGVPYmplY3QudWlkO1xuICAgICAgcmV0dXJuIHRydWU7XG4gICAgfVxuICAgIHJldHVybiBmYWxzZTtcbiAgfTtcblxuICB0aGlzLmluaXQgPSBmdW5jdGlvbihuZ01vZGVsQ3RybF8pIHtcbiAgICBuZ01vZGVsQ3RybCA9IG5nTW9kZWxDdHJsXztcblxuICAgIG5nTW9kZWxDdHJsLiRyZW5kZXIgPSBmdW5jdGlvbigpIHtcbiAgICAgIHNlbGYucmVuZGVyKCk7XG4gICAgfTtcbiAgfTtcblxuICB0aGlzLnJlbmRlciA9IGZ1bmN0aW9uKCkge1xuICAgIGlmIChuZ01vZGVsQ3RybC4kdmlld1ZhbHVlKSB7XG4gICAgICB2YXIgZGF0ZSA9IG5ldyBEYXRlKG5nTW9kZWxDdHJsLiR2aWV3VmFsdWUpLFxuICAgICAgICBpc1ZhbGlkID0gIWlzTmFOKGRhdGUpO1xuXG4gICAgICBpZiAoaXNWYWxpZCkge1xuICAgICAgICB0aGlzLmFjdGl2ZURhdGUgPSBkYXRlO1xuICAgICAgfSBlbHNlIGlmICghJGRhdGVwaWNrZXJTdXBwcmVzc0Vycm9yKSB7XG4gICAgICAgICRsb2cuZXJyb3IoJ0RhdGVwaWNrZXIgZGlyZWN0aXZlOiBcIm5nLW1vZGVsXCIgdmFsdWUgbXVzdCBiZSBhIERhdGUgb2JqZWN0LCBhIG51bWJlciBvZiBtaWxsaXNlY29uZHMgc2luY2UgMDEuMDEuMTk3MCBvciBhIHN0cmluZyByZXByZXNlbnRpbmcgYW4gUkZDMjgyMiBvciBJU08gODYwMSBkYXRlLicpO1xuICAgICAgfVxuICAgIH1cbiAgICB0aGlzLnJlZnJlc2hWaWV3KCk7XG4gIH07XG5cbiAgdGhpcy5yZWZyZXNoVmlldyA9IGZ1bmN0aW9uKCkge1xuICAgIGlmICh0aGlzLmVsZW1lbnQpIHtcbiAgICAgIHRoaXMuX3JlZnJlc2hWaWV3KCk7XG5cbiAgICAgIHZhciBkYXRlID0gbmdNb2RlbEN0cmwuJHZpZXdWYWx1ZSA/IG5ldyBEYXRlKG5nTW9kZWxDdHJsLiR2aWV3VmFsdWUpIDogbnVsbDtcbiAgICAgIG5nTW9kZWxDdHJsLiRzZXRWYWxpZGl0eSgnZGF0ZURpc2FibGVkJywgIWRhdGUgfHwgKHRoaXMuZWxlbWVudCAmJiAhdGhpcy5pc0Rpc2FibGVkKGRhdGUpKSk7XG4gICAgfVxuICB9O1xuXG4gIHRoaXMuY3JlYXRlRGF0ZU9iamVjdCA9IGZ1bmN0aW9uKGRhdGUsIGZvcm1hdCkge1xuICAgIHZhciBtb2RlbCA9IG5nTW9kZWxDdHJsLiR2aWV3VmFsdWUgPyBuZXcgRGF0ZShuZ01vZGVsQ3RybC4kdmlld1ZhbHVlKSA6IG51bGw7XG4gICAgcmV0dXJuIHtcbiAgICAgIGRhdGU6IGRhdGUsXG4gICAgICBsYWJlbDogZGF0ZUZpbHRlcihkYXRlLCBmb3JtYXQpLFxuICAgICAgc2VsZWN0ZWQ6IG1vZGVsICYmIHRoaXMuY29tcGFyZShkYXRlLCBtb2RlbCkgPT09IDAsXG4gICAgICBkaXNhYmxlZDogdGhpcy5pc0Rpc2FibGVkKGRhdGUpLFxuICAgICAgY3VycmVudDogdGhpcy5jb21wYXJlKGRhdGUsIG5ldyBEYXRlKCkpID09PSAwLFxuICAgICAgY3VzdG9tQ2xhc3M6IHRoaXMuY3VzdG9tQ2xhc3MoZGF0ZSlcbiAgICB9O1xuICB9O1xuXG4gIHRoaXMuaXNEaXNhYmxlZCA9IGZ1bmN0aW9uKGRhdGUpIHtcbiAgICByZXR1cm4gKCh0aGlzLm1pbkRhdGUgJiYgdGhpcy5jb21wYXJlKGRhdGUsIHRoaXMubWluRGF0ZSkgPCAwKSB8fCAodGhpcy5tYXhEYXRlICYmIHRoaXMuY29tcGFyZShkYXRlLCB0aGlzLm1heERhdGUpID4gMCkgfHwgKCRhdHRycy5kYXRlRGlzYWJsZWQgJiYgJHNjb3BlLmRhdGVEaXNhYmxlZCh7ZGF0ZTogZGF0ZSwgbW9kZTogJHNjb3BlLmRhdGVwaWNrZXJNb2RlfSkpKTtcbiAgfTtcblxuICB0aGlzLmN1c3RvbUNsYXNzID0gZnVuY3Rpb24oZGF0ZSkge1xuICAgIHJldHVybiAkc2NvcGUuY3VzdG9tQ2xhc3Moe2RhdGU6IGRhdGUsIG1vZGU6ICRzY29wZS5kYXRlcGlja2VyTW9kZX0pO1xuICB9O1xuXG4gIC8vIFNwbGl0IGFycmF5IGludG8gc21hbGxlciBhcnJheXNcbiAgdGhpcy5zcGxpdCA9IGZ1bmN0aW9uKGFyciwgc2l6ZSkge1xuICAgIHZhciBhcnJheXMgPSBbXTtcbiAgICB3aGlsZSAoYXJyLmxlbmd0aCA+IDApIHtcbiAgICAgIGFycmF5cy5wdXNoKGFyci5zcGxpY2UoMCwgc2l6ZSkpO1xuICAgIH1cbiAgICByZXR1cm4gYXJyYXlzO1xuICB9O1xuXG4gIHRoaXMuZml4VGltZVpvbmUgPSBmdW5jdGlvbihkYXRlKSB7XG4gICAgdmFyIGhvdXJzID0gZGF0ZS5nZXRIb3VycygpO1xuICAgIGRhdGUuc2V0SG91cnMoaG91cnMgPT09IDIzID8gaG91cnMgKyAyIDogMCk7XG4gIH07XG5cbiAgJHNjb3BlLnNlbGVjdCA9IGZ1bmN0aW9uKGRhdGUpIHtcbiAgICBpZiAoJHNjb3BlLmRhdGVwaWNrZXJNb2RlID09PSBzZWxmLm1pbk1vZGUpIHtcbiAgICAgIHZhciBkdCA9IG5nTW9kZWxDdHJsLiR2aWV3VmFsdWUgPyBuZXcgRGF0ZShuZ01vZGVsQ3RybC4kdmlld1ZhbHVlKSA6IG5ldyBEYXRlKDAsIDAsIDAsIDAsIDAsIDAsIDApO1xuICAgICAgZHQuc2V0RnVsbFllYXIoZGF0ZS5nZXRGdWxsWWVhcigpLCBkYXRlLmdldE1vbnRoKCksIGRhdGUuZ2V0RGF0ZSgpKTtcbiAgICAgIG5nTW9kZWxDdHJsLiRzZXRWaWV3VmFsdWUoZHQpO1xuICAgICAgbmdNb2RlbEN0cmwuJHJlbmRlcigpO1xuICAgIH0gZWxzZSB7XG4gICAgICBzZWxmLmFjdGl2ZURhdGUgPSBkYXRlO1xuICAgICAgJHNjb3BlLmRhdGVwaWNrZXJNb2RlID0gc2VsZi5tb2Rlc1tzZWxmLm1vZGVzLmluZGV4T2YoJHNjb3BlLmRhdGVwaWNrZXJNb2RlKSAtIDFdO1xuICAgIH1cbiAgfTtcblxuICAkc2NvcGUubW92ZSA9IGZ1bmN0aW9uKGRpcmVjdGlvbikge1xuICAgIHZhciB5ZWFyID0gc2VsZi5hY3RpdmVEYXRlLmdldEZ1bGxZZWFyKCkgKyBkaXJlY3Rpb24gKiAoc2VsZi5zdGVwLnllYXJzIHx8IDApLFxuICAgICAgbW9udGggPSBzZWxmLmFjdGl2ZURhdGUuZ2V0TW9udGgoKSArIGRpcmVjdGlvbiAqIChzZWxmLnN0ZXAubW9udGhzIHx8IDApO1xuICAgIHNlbGYuYWN0aXZlRGF0ZS5zZXRGdWxsWWVhcih5ZWFyLCBtb250aCwgMSk7XG4gICAgc2VsZi5yZWZyZXNoVmlldygpO1xuICB9O1xuXG4gICRzY29wZS50b2dnbGVNb2RlID0gZnVuY3Rpb24oZGlyZWN0aW9uKSB7XG4gICAgZGlyZWN0aW9uID0gZGlyZWN0aW9uIHx8IDE7XG5cbiAgICBpZiAoKCRzY29wZS5kYXRlcGlja2VyTW9kZSA9PT0gc2VsZi5tYXhNb2RlICYmIGRpcmVjdGlvbiA9PT0gMSkgfHwgKCRzY29wZS5kYXRlcGlja2VyTW9kZSA9PT0gc2VsZi5taW5Nb2RlICYmIGRpcmVjdGlvbiA9PT0gLTEpKSB7XG4gICAgICByZXR1cm47XG4gICAgfVxuXG4gICAgJHNjb3BlLmRhdGVwaWNrZXJNb2RlID0gc2VsZi5tb2Rlc1tzZWxmLm1vZGVzLmluZGV4T2YoJHNjb3BlLmRhdGVwaWNrZXJNb2RlKSArIGRpcmVjdGlvbl07XG4gIH07XG5cbiAgLy8gS2V5IGV2ZW50IG1hcHBlclxuICAkc2NvcGUua2V5cyA9IHsgMTM6ICdlbnRlcicsIDMyOiAnc3BhY2UnLCAzMzogJ3BhZ2V1cCcsIDM0OiAncGFnZWRvd24nLCAzNTogJ2VuZCcsIDM2OiAnaG9tZScsIDM3OiAnbGVmdCcsIDM4OiAndXAnLCAzOTogJ3JpZ2h0JywgNDA6ICdkb3duJyB9O1xuXG4gIHZhciBmb2N1c0VsZW1lbnQgPSBmdW5jdGlvbigpIHtcbiAgICBzZWxmLmVsZW1lbnRbMF0uZm9jdXMoKTtcbiAgfTtcblxuICAkc2NvcGUuJG9uKCd1aWI6ZGF0ZXBpY2tlci5mb2N1cycsIGZvY3VzRWxlbWVudCk7XG5cbiAgJHNjb3BlLmtleWRvd24gPSBmdW5jdGlvbihldnQpIHtcbiAgICB2YXIga2V5ID0gJHNjb3BlLmtleXNbZXZ0LndoaWNoXTtcblxuICAgIGlmICgha2V5IHx8IGV2dC5zaGlmdEtleSB8fCBldnQuYWx0S2V5KSB7XG4gICAgICByZXR1cm47XG4gICAgfVxuXG4gICAgZXZ0LnByZXZlbnREZWZhdWx0KCk7XG4gICAgaWYgKCFzZWxmLnNob3J0Y3V0UHJvcGFnYXRpb24pIHtcbiAgICAgIGV2dC5zdG9wUHJvcGFnYXRpb24oKTtcbiAgICB9XG5cbiAgICBpZiAoa2V5ID09PSAnZW50ZXInIHx8IGtleSA9PT0gJ3NwYWNlJykge1xuICAgICAgaWYgKHNlbGYuaXNEaXNhYmxlZChzZWxmLmFjdGl2ZURhdGUpKSB7XG4gICAgICAgIHJldHVybjsgLy8gZG8gbm90aGluZ1xuICAgICAgfVxuICAgICAgJHNjb3BlLnNlbGVjdChzZWxmLmFjdGl2ZURhdGUpO1xuICAgIH0gZWxzZSBpZiAoZXZ0LmN0cmxLZXkgJiYgKGtleSA9PT0gJ3VwJyB8fCBrZXkgPT09ICdkb3duJykpIHtcbiAgICAgICRzY29wZS50b2dnbGVNb2RlKGtleSA9PT0gJ3VwJyA/IDEgOiAtMSk7XG4gICAgfSBlbHNlIHtcbiAgICAgIHNlbGYuaGFuZGxlS2V5RG93bihrZXksIGV2dCk7XG4gICAgICBzZWxmLnJlZnJlc2hWaWV3KCk7XG4gICAgfVxuICB9O1xufV0pXG5cbi5kaXJlY3RpdmUoJ2RhdGVwaWNrZXInLCBbJyRsb2cnLCAnJGRhdGVwaWNrZXJTdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkbG9nLCAkZGF0ZXBpY2tlclN1cHByZXNzV2FybmluZykge1xuICByZXR1cm4ge1xuICAgIHJlcGxhY2U6IHRydWUsXG4gICAgdGVtcGxhdGVVcmw6IGZ1bmN0aW9uKGVsZW1lbnQsIGF0dHJzKSB7XG4gICAgICByZXR1cm4gYXR0cnMudGVtcGxhdGVVcmwgfHwgJ3RlbXBsYXRlL2RhdGVwaWNrZXIvZGF0ZXBpY2tlci5odG1sJztcbiAgICB9LFxuICAgIHNjb3BlOiB7XG4gICAgICBkYXRlcGlja2VyTW9kZTogJz0/JyxcbiAgICAgIGRhdGVEaXNhYmxlZDogJyYnLFxuICAgICAgY3VzdG9tQ2xhc3M6ICcmJyxcbiAgICAgIHNob3J0Y3V0UHJvcGFnYXRpb246ICcmPydcbiAgICB9LFxuICAgIHJlcXVpcmU6IFsnZGF0ZXBpY2tlcicsICdebmdNb2RlbCddLFxuICAgIGNvbnRyb2xsZXI6ICdEYXRlcGlja2VyQ29udHJvbGxlcicsXG4gICAgY29udHJvbGxlckFzOiAnZGF0ZXBpY2tlcicsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHJzLCBjdHJscykge1xuICAgICAgaWYgKCEkZGF0ZXBpY2tlclN1cHByZXNzV2FybmluZykge1xuICAgICAgICAkbG9nLndhcm4oJ2RhdGVwaWNrZXIgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSB1aWItZGF0ZXBpY2tlciBpbnN0ZWFkLicpO1xuICAgICAgfVxuXG4gICAgICB2YXIgZGF0ZXBpY2tlckN0cmwgPSBjdHJsc1swXSwgbmdNb2RlbEN0cmwgPSBjdHJsc1sxXTtcblxuICAgICAgZGF0ZXBpY2tlckN0cmwuaW5pdChuZ01vZGVsQ3RybCk7XG4gICAgfVxuICB9O1xufV0pXG5cbi5kaXJlY3RpdmUoJ2RheXBpY2tlcicsIFsnJGxvZycsICckZGF0ZXBpY2tlclN1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRsb2csICRkYXRlcGlja2VyU3VwcHJlc3NXYXJuaW5nKSB7XG4gIHJldHVybiB7XG4gICAgcmVwbGFjZTogdHJ1ZSxcbiAgICB0ZW1wbGF0ZVVybDogJ3RlbXBsYXRlL2RhdGVwaWNrZXIvZGF5Lmh0bWwnLFxuICAgIHJlcXVpcmU6IFsnXmRhdGVwaWNrZXInLCAnZGF5cGlja2VyJ10sXG4gICAgY29udHJvbGxlcjogJ1VpYkRheXBpY2tlckNvbnRyb2xsZXInLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgY3RybHMpIHtcbiAgICAgIGlmICghJGRhdGVwaWNrZXJTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgJGxvZy53YXJuKCdkYXlwaWNrZXIgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSB1aWItZGF5cGlja2VyIGluc3RlYWQuJyk7XG4gICAgICB9XG5cbiAgICAgIHZhciBkYXRlcGlja2VyQ3RybCA9IGN0cmxzWzBdLFxuICAgICAgICBkYXlwaWNrZXJDdHJsID0gY3RybHNbMV07XG5cbiAgICAgIGRheXBpY2tlckN0cmwuaW5pdChkYXRlcGlja2VyQ3RybCk7XG4gICAgfVxuICB9O1xufV0pXG5cbi5kaXJlY3RpdmUoJ21vbnRocGlja2VyJywgWyckbG9nJywgJyRkYXRlcGlja2VyU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJGxvZywgJGRhdGVwaWNrZXJTdXBwcmVzc1dhcm5pbmcpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHRlbXBsYXRlVXJsOiAndGVtcGxhdGUvZGF0ZXBpY2tlci9tb250aC5odG1sJyxcbiAgICByZXF1aXJlOiBbJ15kYXRlcGlja2VyJywgJ21vbnRocGlja2VyJ10sXG4gICAgY29udHJvbGxlcjogJ1VpYk1vbnRocGlja2VyQ29udHJvbGxlcicsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHJzLCBjdHJscykge1xuICAgICAgaWYgKCEkZGF0ZXBpY2tlclN1cHByZXNzV2FybmluZykge1xuICAgICAgICAkbG9nLndhcm4oJ21vbnRocGlja2VyIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliLW1vbnRocGlja2VyIGluc3RlYWQuJyk7XG4gICAgICB9XG5cbiAgICAgIHZhciBkYXRlcGlja2VyQ3RybCA9IGN0cmxzWzBdLFxuICAgICAgICBtb250aHBpY2tlckN0cmwgPSBjdHJsc1sxXTtcblxuICAgICAgbW9udGhwaWNrZXJDdHJsLmluaXQoZGF0ZXBpY2tlckN0cmwpO1xuICAgIH1cbiAgfTtcbn1dKVxuXG4uZGlyZWN0aXZlKCd5ZWFycGlja2VyJywgWyckbG9nJywgJyRkYXRlcGlja2VyU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJGxvZywgJGRhdGVwaWNrZXJTdXBwcmVzc1dhcm5pbmcpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHRlbXBsYXRlVXJsOiAndGVtcGxhdGUvZGF0ZXBpY2tlci95ZWFyLmh0bWwnLFxuICAgIHJlcXVpcmU6IFsnXmRhdGVwaWNrZXInLCAneWVhcnBpY2tlciddLFxuICAgIGNvbnRyb2xsZXI6ICdVaWJZZWFycGlja2VyQ29udHJvbGxlcicsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHJzLCBjdHJscykge1xuICAgICAgaWYgKCEkZGF0ZXBpY2tlclN1cHByZXNzV2FybmluZykge1xuICAgICAgICAkbG9nLndhcm4oJ3llYXJwaWNrZXIgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSB1aWIteWVhcnBpY2tlciBpbnN0ZWFkLicpO1xuICAgICAgfVxuXG4gICAgICB2YXIgY3RybCA9IGN0cmxzWzBdO1xuICAgICAgYW5ndWxhci5leHRlbmQoY3RybCwgY3RybHNbMV0pO1xuICAgICAgY3RybC55ZWFycGlja2VySW5pdCgpO1xuXG4gICAgICBjdHJsLnJlZnJlc2hWaWV3KCk7XG4gICAgfVxuICB9O1xufV0pXG5cbi5kaXJlY3RpdmUoJ2RhdGVwaWNrZXJQb3B1cCcsIFsnJGxvZycsICckZGF0ZXBpY2tlclN1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRsb2csICRkYXRlcGlja2VyU3VwcHJlc3NXYXJuaW5nKSB7XG4gIHJldHVybiB7XG4gICAgcmVxdWlyZTogWyduZ01vZGVsJywgJ2RhdGVwaWNrZXJQb3B1cCddLFxuICAgIGNvbnRyb2xsZXI6ICdVaWJEYXRlcGlja2VyUG9wdXBDb250cm9sbGVyJyxcbiAgICBzY29wZToge1xuICAgICAgaXNPcGVuOiAnPT8nLFxuICAgICAgY3VycmVudFRleHQ6ICdAJyxcbiAgICAgIGNsZWFyVGV4dDogJ0AnLFxuICAgICAgY2xvc2VUZXh0OiAnQCcsXG4gICAgICBkYXRlRGlzYWJsZWQ6ICcmJyxcbiAgICAgIGN1c3RvbUNsYXNzOiAnJidcbiAgICB9LFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgY3RybHMpIHtcbiAgICAgIGlmICghJGRhdGVwaWNrZXJTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgJGxvZy53YXJuKCdkYXRlcGlja2VyLXBvcHVwIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliLWRhdGVwaWNrZXItcG9wdXAgaW5zdGVhZC4nKTtcbiAgICAgIH1cblxuICAgICAgdmFyIG5nTW9kZWwgPSBjdHJsc1swXSxcbiAgICAgICAgY3RybCA9IGN0cmxzWzFdO1xuXG4gICAgICBjdHJsLmluaXQobmdNb2RlbCk7XG4gICAgfVxuICB9O1xufV0pXG5cbi5kaXJlY3RpdmUoJ2RhdGVwaWNrZXJQb3B1cFdyYXAnLCBbJyRsb2cnLCAnJGRhdGVwaWNrZXJTdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkbG9nLCAkZGF0ZXBpY2tlclN1cHByZXNzV2FybmluZykge1xuICByZXR1cm4ge1xuICAgIHJlcGxhY2U6IHRydWUsXG4gICAgdHJhbnNjbHVkZTogdHJ1ZSxcbiAgICB0ZW1wbGF0ZVVybDogZnVuY3Rpb24oZWxlbWVudCwgYXR0cnMpIHtcbiAgICAgIHJldHVybiBhdHRycy50ZW1wbGF0ZVVybCB8fCAndGVtcGxhdGUvZGF0ZXBpY2tlci9wb3B1cC5odG1sJztcbiAgICB9LFxuICAgIGxpbms6IGZ1bmN0aW9uKCkge1xuICAgICAgaWYgKCEkZGF0ZXBpY2tlclN1cHByZXNzV2FybmluZykge1xuICAgICAgICAkbG9nLndhcm4oJ2RhdGVwaWNrZXItcG9wdXAtd3JhcCBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi1kYXRlcGlja2VyLXBvcHVwLXdyYXAgaW5zdGVhZC4nKTtcbiAgICAgIH1cbiAgICB9XG4gIH07XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKCd1aS5ib290c3RyYXAuZHJvcGRvd24nLCBbJ3VpLmJvb3RzdHJhcC5wb3NpdGlvbiddKVxuXG4uY29uc3RhbnQoJ3VpYkRyb3Bkb3duQ29uZmlnJywge1xuICBvcGVuQ2xhc3M6ICdvcGVuJ1xufSlcblxuLnNlcnZpY2UoJ3VpYkRyb3Bkb3duU2VydmljZScsIFsnJGRvY3VtZW50JywgJyRyb290U2NvcGUnLCBmdW5jdGlvbigkZG9jdW1lbnQsICRyb290U2NvcGUpIHtcbiAgdmFyIG9wZW5TY29wZSA9IG51bGw7XG5cbiAgdGhpcy5vcGVuID0gZnVuY3Rpb24oZHJvcGRvd25TY29wZSkge1xuICAgIGlmICghb3BlblNjb3BlKSB7XG4gICAgICAkZG9jdW1lbnQuYmluZCgnY2xpY2snLCBjbG9zZURyb3Bkb3duKTtcbiAgICAgICRkb2N1bWVudC5iaW5kKCdrZXlkb3duJywga2V5YmluZEZpbHRlcik7XG4gICAgfVxuXG4gICAgaWYgKG9wZW5TY29wZSAmJiBvcGVuU2NvcGUgIT09IGRyb3Bkb3duU2NvcGUpIHtcbiAgICAgIG9wZW5TY29wZS5pc09wZW4gPSBmYWxzZTtcbiAgICB9XG5cbiAgICBvcGVuU2NvcGUgPSBkcm9wZG93blNjb3BlO1xuICB9O1xuXG4gIHRoaXMuY2xvc2UgPSBmdW5jdGlvbihkcm9wZG93blNjb3BlKSB7XG4gICAgaWYgKG9wZW5TY29wZSA9PT0gZHJvcGRvd25TY29wZSkge1xuICAgICAgb3BlblNjb3BlID0gbnVsbDtcbiAgICAgICRkb2N1bWVudC51bmJpbmQoJ2NsaWNrJywgY2xvc2VEcm9wZG93bik7XG4gICAgICAkZG9jdW1lbnQudW5iaW5kKCdrZXlkb3duJywga2V5YmluZEZpbHRlcik7XG4gICAgfVxuICB9O1xuXG4gIHZhciBjbG9zZURyb3Bkb3duID0gZnVuY3Rpb24oZXZ0KSB7XG4gICAgLy8gVGhpcyBtZXRob2QgbWF5IHN0aWxsIGJlIGNhbGxlZCBkdXJpbmcgdGhlIHNhbWUgbW91c2UgZXZlbnQgdGhhdFxuICAgIC8vIHVuYm91bmQgdGhpcyBldmVudCBoYW5kbGVyLiBTbyBjaGVjayBvcGVuU2NvcGUgYmVmb3JlIHByb2NlZWRpbmcuXG4gICAgaWYgKCFvcGVuU2NvcGUpIHsgcmV0dXJuOyB9XG5cbiAgICBpZiAoZXZ0ICYmIG9wZW5TY29wZS5nZXRBdXRvQ2xvc2UoKSA9PT0gJ2Rpc2FibGVkJykgIHsgcmV0dXJuIDsgfVxuXG4gICAgdmFyIHRvZ2dsZUVsZW1lbnQgPSBvcGVuU2NvcGUuZ2V0VG9nZ2xlRWxlbWVudCgpO1xuICAgIGlmIChldnQgJiYgdG9nZ2xlRWxlbWVudCAmJiB0b2dnbGVFbGVtZW50WzBdLmNvbnRhaW5zKGV2dC50YXJnZXQpKSB7XG4gICAgICByZXR1cm47XG4gICAgfVxuXG4gICAgdmFyIGRyb3Bkb3duRWxlbWVudCA9IG9wZW5TY29wZS5nZXREcm9wZG93bkVsZW1lbnQoKTtcbiAgICBpZiAoZXZ0ICYmIG9wZW5TY29wZS5nZXRBdXRvQ2xvc2UoKSA9PT0gJ291dHNpZGVDbGljaycgJiZcbiAgICAgIGRyb3Bkb3duRWxlbWVudCAmJiBkcm9wZG93bkVsZW1lbnRbMF0uY29udGFpbnMoZXZ0LnRhcmdldCkpIHtcbiAgICAgIHJldHVybjtcbiAgICB9XG5cbiAgICBvcGVuU2NvcGUuaXNPcGVuID0gZmFsc2U7XG5cbiAgICBpZiAoISRyb290U2NvcGUuJCRwaGFzZSkge1xuICAgICAgb3BlblNjb3BlLiRhcHBseSgpO1xuICAgIH1cbiAgfTtcblxuICB2YXIga2V5YmluZEZpbHRlciA9IGZ1bmN0aW9uKGV2dCkge1xuICAgIGlmIChldnQud2hpY2ggPT09IDI3KSB7XG4gICAgICBvcGVuU2NvcGUuZm9jdXNUb2dnbGVFbGVtZW50KCk7XG4gICAgICBjbG9zZURyb3Bkb3duKCk7XG4gICAgfSBlbHNlIGlmIChvcGVuU2NvcGUuaXNLZXluYXZFbmFibGVkKCkgJiYgLygzOHw0MCkvLnRlc3QoZXZ0LndoaWNoKSAmJiBvcGVuU2NvcGUuaXNPcGVuKSB7XG4gICAgICBldnQucHJldmVudERlZmF1bHQoKTtcbiAgICAgIGV2dC5zdG9wUHJvcGFnYXRpb24oKTtcbiAgICAgIG9wZW5TY29wZS5mb2N1c0Ryb3Bkb3duRW50cnkoZXZ0LndoaWNoKTtcbiAgICB9XG4gIH07XG59XSlcblxuLmNvbnRyb2xsZXIoJ1VpYkRyb3Bkb3duQ29udHJvbGxlcicsIFsnJHNjb3BlJywgJyRlbGVtZW50JywgJyRhdHRycycsICckcGFyc2UnLCAndWliRHJvcGRvd25Db25maWcnLCAndWliRHJvcGRvd25TZXJ2aWNlJywgJyRhbmltYXRlJywgJyR1aWJQb3NpdGlvbicsICckZG9jdW1lbnQnLCAnJGNvbXBpbGUnLCAnJHRlbXBsYXRlUmVxdWVzdCcsIGZ1bmN0aW9uKCRzY29wZSwgJGVsZW1lbnQsICRhdHRycywgJHBhcnNlLCBkcm9wZG93bkNvbmZpZywgdWliRHJvcGRvd25TZXJ2aWNlLCAkYW5pbWF0ZSwgJHBvc2l0aW9uLCAkZG9jdW1lbnQsICRjb21waWxlLCAkdGVtcGxhdGVSZXF1ZXN0KSB7XG4gIHZhciBzZWxmID0gdGhpcyxcbiAgICBzY29wZSA9ICRzY29wZS4kbmV3KCksIC8vIGNyZWF0ZSBhIGNoaWxkIHNjb3BlIHNvIHdlIGFyZSBub3QgcG9sbHV0aW5nIG9yaWdpbmFsIG9uZVxuICAgIHRlbXBsYXRlU2NvcGUsXG4gICAgb3BlbkNsYXNzID0gZHJvcGRvd25Db25maWcub3BlbkNsYXNzLFxuICAgIGdldElzT3BlbixcbiAgICBzZXRJc09wZW4gPSBhbmd1bGFyLm5vb3AsXG4gICAgdG9nZ2xlSW52b2tlciA9ICRhdHRycy5vblRvZ2dsZSA/ICRwYXJzZSgkYXR0cnMub25Ub2dnbGUpIDogYW5ndWxhci5ub29wLFxuICAgIGFwcGVuZFRvQm9keSA9IGZhbHNlLFxuICAgIGtleW5hdkVuYWJsZWQgPWZhbHNlLFxuICAgIHNlbGVjdGVkT3B0aW9uID0gbnVsbDtcblxuXG4gICRlbGVtZW50LmFkZENsYXNzKCdkcm9wZG93bicpO1xuXG4gIHRoaXMuaW5pdCA9IGZ1bmN0aW9uKCkge1xuICAgIGlmICgkYXR0cnMuaXNPcGVuKSB7XG4gICAgICBnZXRJc09wZW4gPSAkcGFyc2UoJGF0dHJzLmlzT3Blbik7XG4gICAgICBzZXRJc09wZW4gPSBnZXRJc09wZW4uYXNzaWduO1xuXG4gICAgICAkc2NvcGUuJHdhdGNoKGdldElzT3BlbiwgZnVuY3Rpb24odmFsdWUpIHtcbiAgICAgICAgc2NvcGUuaXNPcGVuID0gISF2YWx1ZTtcbiAgICAgIH0pO1xuICAgIH1cblxuICAgIGFwcGVuZFRvQm9keSA9IGFuZ3VsYXIuaXNEZWZpbmVkKCRhdHRycy5kcm9wZG93bkFwcGVuZFRvQm9keSk7XG4gICAga2V5bmF2RW5hYmxlZCA9IGFuZ3VsYXIuaXNEZWZpbmVkKCRhdHRycy51aWJLZXlib2FyZE5hdik7XG5cbiAgICBpZiAoYXBwZW5kVG9Cb2R5ICYmIHNlbGYuZHJvcGRvd25NZW51KSB7XG4gICAgICAkZG9jdW1lbnQuZmluZCgnYm9keScpLmFwcGVuZChzZWxmLmRyb3Bkb3duTWVudSk7XG4gICAgICAkZWxlbWVudC5vbignJGRlc3Ryb3knLCBmdW5jdGlvbiBoYW5kbGVEZXN0cm95RXZlbnQoKSB7XG4gICAgICAgIHNlbGYuZHJvcGRvd25NZW51LnJlbW92ZSgpO1xuICAgICAgfSk7XG4gICAgfVxuICB9O1xuXG4gIHRoaXMudG9nZ2xlID0gZnVuY3Rpb24ob3Blbikge1xuICAgIHJldHVybiBzY29wZS5pc09wZW4gPSBhcmd1bWVudHMubGVuZ3RoID8gISFvcGVuIDogIXNjb3BlLmlzT3BlbjtcbiAgfTtcblxuICAvLyBBbGxvdyBvdGhlciBkaXJlY3RpdmVzIHRvIHdhdGNoIHN0YXR1c1xuICB0aGlzLmlzT3BlbiA9IGZ1bmN0aW9uKCkge1xuICAgIHJldHVybiBzY29wZS5pc09wZW47XG4gIH07XG5cbiAgc2NvcGUuZ2V0VG9nZ2xlRWxlbWVudCA9IGZ1bmN0aW9uKCkge1xuICAgIHJldHVybiBzZWxmLnRvZ2dsZUVsZW1lbnQ7XG4gIH07XG5cbiAgc2NvcGUuZ2V0QXV0b0Nsb3NlID0gZnVuY3Rpb24oKSB7XG4gICAgcmV0dXJuICRhdHRycy5hdXRvQ2xvc2UgfHwgJ2Fsd2F5cyc7IC8vb3IgJ291dHNpZGVDbGljaycgb3IgJ2Rpc2FibGVkJ1xuICB9O1xuXG4gIHNjb3BlLmdldEVsZW1lbnQgPSBmdW5jdGlvbigpIHtcbiAgICByZXR1cm4gJGVsZW1lbnQ7XG4gIH07XG5cbiAgc2NvcGUuaXNLZXluYXZFbmFibGVkID0gZnVuY3Rpb24oKSB7XG4gICAgcmV0dXJuIGtleW5hdkVuYWJsZWQ7XG4gIH07XG5cbiAgc2NvcGUuZm9jdXNEcm9wZG93bkVudHJ5ID0gZnVuY3Rpb24oa2V5Q29kZSkge1xuICAgIHZhciBlbGVtcyA9IHNlbGYuZHJvcGRvd25NZW51ID8gLy9JZiBhcHBlbmQgdG8gYm9keSBpcyB1c2VkLlxuICAgICAgKGFuZ3VsYXIuZWxlbWVudChzZWxmLmRyb3Bkb3duTWVudSkuZmluZCgnYScpKSA6XG4gICAgICAoYW5ndWxhci5lbGVtZW50KCRlbGVtZW50KS5maW5kKCd1bCcpLmVxKDApLmZpbmQoJ2EnKSk7XG5cbiAgICBzd2l0Y2ggKGtleUNvZGUpIHtcbiAgICAgIGNhc2UgKDQwKToge1xuICAgICAgICBpZiAoIWFuZ3VsYXIuaXNOdW1iZXIoc2VsZi5zZWxlY3RlZE9wdGlvbikpIHtcbiAgICAgICAgICBzZWxmLnNlbGVjdGVkT3B0aW9uID0gMDtcbiAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICBzZWxmLnNlbGVjdGVkT3B0aW9uID0gKHNlbGYuc2VsZWN0ZWRPcHRpb24gPT09IGVsZW1zLmxlbmd0aCAtIDEgP1xuICAgICAgICAgICAgc2VsZi5zZWxlY3RlZE9wdGlvbiA6XG4gICAgICAgICAgICBzZWxmLnNlbGVjdGVkT3B0aW9uICsgMSk7XG4gICAgICAgIH1cbiAgICAgICAgYnJlYWs7XG4gICAgICB9XG4gICAgICBjYXNlICgzOCk6IHtcbiAgICAgICAgaWYgKCFhbmd1bGFyLmlzTnVtYmVyKHNlbGYuc2VsZWN0ZWRPcHRpb24pKSB7XG4gICAgICAgICAgc2VsZi5zZWxlY3RlZE9wdGlvbiA9IGVsZW1zLmxlbmd0aCAtIDE7XG4gICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgc2VsZi5zZWxlY3RlZE9wdGlvbiA9IHNlbGYuc2VsZWN0ZWRPcHRpb24gPT09IDAgP1xuICAgICAgICAgICAgMCA6IHNlbGYuc2VsZWN0ZWRPcHRpb24gLSAxO1xuICAgICAgICB9XG4gICAgICAgIGJyZWFrO1xuICAgICAgfVxuICAgIH1cbiAgICBlbGVtc1tzZWxmLnNlbGVjdGVkT3B0aW9uXS5mb2N1cygpO1xuICB9O1xuXG4gIHNjb3BlLmdldERyb3Bkb3duRWxlbWVudCA9IGZ1bmN0aW9uKCkge1xuICAgIHJldHVybiBzZWxmLmRyb3Bkb3duTWVudTtcbiAgfTtcblxuICBzY29wZS5mb2N1c1RvZ2dsZUVsZW1lbnQgPSBmdW5jdGlvbigpIHtcbiAgICBpZiAoc2VsZi50b2dnbGVFbGVtZW50KSB7XG4gICAgICBzZWxmLnRvZ2dsZUVsZW1lbnRbMF0uZm9jdXMoKTtcbiAgICB9XG4gIH07XG5cbiAgc2NvcGUuJHdhdGNoKCdpc09wZW4nLCBmdW5jdGlvbihpc09wZW4sIHdhc09wZW4pIHtcbiAgICBpZiAoYXBwZW5kVG9Cb2R5ICYmIHNlbGYuZHJvcGRvd25NZW51KSB7XG4gICAgICB2YXIgcG9zID0gJHBvc2l0aW9uLnBvc2l0aW9uRWxlbWVudHMoJGVsZW1lbnQsIHNlbGYuZHJvcGRvd25NZW51LCAnYm90dG9tLWxlZnQnLCB0cnVlKTtcbiAgICAgIHZhciBjc3MgPSB7XG4gICAgICAgIHRvcDogcG9zLnRvcCArICdweCcsXG4gICAgICAgIGRpc3BsYXk6IGlzT3BlbiA/ICdibG9jaycgOiAnbm9uZSdcbiAgICAgIH07XG5cbiAgICAgIHZhciByaWdodGFsaWduID0gc2VsZi5kcm9wZG93bk1lbnUuaGFzQ2xhc3MoJ2Ryb3Bkb3duLW1lbnUtcmlnaHQnKTtcbiAgICAgIGlmICghcmlnaHRhbGlnbikge1xuICAgICAgICBjc3MubGVmdCA9IHBvcy5sZWZ0ICsgJ3B4JztcbiAgICAgICAgY3NzLnJpZ2h0ID0gJ2F1dG8nO1xuICAgICAgfSBlbHNlIHtcbiAgICAgICAgY3NzLmxlZnQgPSAnYXV0byc7XG4gICAgICAgIGNzcy5yaWdodCA9ICh3aW5kb3cuaW5uZXJXaWR0aCAtIChwb3MubGVmdCArICRlbGVtZW50LnByb3AoJ29mZnNldFdpZHRoJykpKSArICdweCc7XG4gICAgICB9XG5cbiAgICAgIHNlbGYuZHJvcGRvd25NZW51LmNzcyhjc3MpO1xuICAgIH1cblxuICAgICRhbmltYXRlW2lzT3BlbiA/ICdhZGRDbGFzcycgOiAncmVtb3ZlQ2xhc3MnXSgkZWxlbWVudCwgb3BlbkNsYXNzKS50aGVuKGZ1bmN0aW9uKCkge1xuICAgICAgaWYgKGFuZ3VsYXIuaXNEZWZpbmVkKGlzT3BlbikgJiYgaXNPcGVuICE9PSB3YXNPcGVuKSB7XG4gICAgICAgIHRvZ2dsZUludm9rZXIoJHNjb3BlLCB7IG9wZW46ICEhaXNPcGVuIH0pO1xuICAgICAgfVxuICAgIH0pO1xuXG4gICAgaWYgKGlzT3Blbikge1xuICAgICAgaWYgKHNlbGYuZHJvcGRvd25NZW51VGVtcGxhdGVVcmwpIHtcbiAgICAgICAgJHRlbXBsYXRlUmVxdWVzdChzZWxmLmRyb3Bkb3duTWVudVRlbXBsYXRlVXJsKS50aGVuKGZ1bmN0aW9uKHRwbENvbnRlbnQpIHtcbiAgICAgICAgICB0ZW1wbGF0ZVNjb3BlID0gc2NvcGUuJG5ldygpO1xuICAgICAgICAgICRjb21waWxlKHRwbENvbnRlbnQudHJpbSgpKSh0ZW1wbGF0ZVNjb3BlLCBmdW5jdGlvbihkcm9wZG93bkVsZW1lbnQpIHtcbiAgICAgICAgICAgIHZhciBuZXdFbCA9IGRyb3Bkb3duRWxlbWVudDtcbiAgICAgICAgICAgIHNlbGYuZHJvcGRvd25NZW51LnJlcGxhY2VXaXRoKG5ld0VsKTtcbiAgICAgICAgICAgIHNlbGYuZHJvcGRvd25NZW51ID0gbmV3RWw7XG4gICAgICAgICAgfSk7XG4gICAgICAgIH0pO1xuICAgICAgfVxuXG4gICAgICBzY29wZS5mb2N1c1RvZ2dsZUVsZW1lbnQoKTtcbiAgICAgIHVpYkRyb3Bkb3duU2VydmljZS5vcGVuKHNjb3BlKTtcbiAgICB9IGVsc2Uge1xuICAgICAgaWYgKHNlbGYuZHJvcGRvd25NZW51VGVtcGxhdGVVcmwpIHtcbiAgICAgICAgaWYgKHRlbXBsYXRlU2NvcGUpIHtcbiAgICAgICAgICB0ZW1wbGF0ZVNjb3BlLiRkZXN0cm95KCk7XG4gICAgICAgIH1cbiAgICAgICAgdmFyIG5ld0VsID0gYW5ndWxhci5lbGVtZW50KCc8dWwgY2xhc3M9XCJkcm9wZG93bi1tZW51XCI+PC91bD4nKTtcbiAgICAgICAgc2VsZi5kcm9wZG93bk1lbnUucmVwbGFjZVdpdGgobmV3RWwpO1xuICAgICAgICBzZWxmLmRyb3Bkb3duTWVudSA9IG5ld0VsO1xuICAgICAgfVxuXG4gICAgICB1aWJEcm9wZG93blNlcnZpY2UuY2xvc2Uoc2NvcGUpO1xuICAgICAgc2VsZi5zZWxlY3RlZE9wdGlvbiA9IG51bGw7XG4gICAgfVxuXG4gICAgaWYgKGFuZ3VsYXIuaXNGdW5jdGlvbihzZXRJc09wZW4pKSB7XG4gICAgICBzZXRJc09wZW4oJHNjb3BlLCBpc09wZW4pO1xuICAgIH1cbiAgfSk7XG5cbiAgJHNjb3BlLiRvbignJGxvY2F0aW9uQ2hhbmdlU3VjY2VzcycsIGZ1bmN0aW9uKCkge1xuICAgIGlmIChzY29wZS5nZXRBdXRvQ2xvc2UoKSAhPT0gJ2Rpc2FibGVkJykge1xuICAgICAgc2NvcGUuaXNPcGVuID0gZmFsc2U7XG4gICAgfVxuICB9KTtcblxuICB2YXIgb2ZmRGVzdHJveSA9ICRzY29wZS4kb24oJyRkZXN0cm95JywgZnVuY3Rpb24oKSB7XG4gICAgc2NvcGUuJGRlc3Ryb3koKTtcbiAgfSk7XG4gIHNjb3BlLiRvbignJGRlc3Ryb3knLCBvZmZEZXN0cm95KTtcbn1dKVxuXG4uZGlyZWN0aXZlKCd1aWJEcm9wZG93bicsIGZ1bmN0aW9uKCkge1xuICByZXR1cm4ge1xuICAgIGNvbnRyb2xsZXI6ICdVaWJEcm9wZG93bkNvbnRyb2xsZXInLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgZHJvcGRvd25DdHJsKSB7XG4gICAgICBkcm9wZG93bkN0cmwuaW5pdCgpO1xuICAgIH1cbiAgfTtcbn0pXG5cbi5kaXJlY3RpdmUoJ3VpYkRyb3Bkb3duTWVudScsIGZ1bmN0aW9uKCkge1xuICByZXR1cm4ge1xuICAgIHJlc3RyaWN0OiAnQUMnLFxuICAgIHJlcXVpcmU6ICc/XnVpYkRyb3Bkb3duJyxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMsIGRyb3Bkb3duQ3RybCkge1xuICAgICAgaWYgKCFkcm9wZG93bkN0cmwgfHwgYW5ndWxhci5pc0RlZmluZWQoYXR0cnMuZHJvcGRvd25OZXN0ZWQpKSB7XG4gICAgICAgIHJldHVybjtcbiAgICAgIH1cblxuICAgICAgZWxlbWVudC5hZGRDbGFzcygnZHJvcGRvd24tbWVudScpO1xuXG4gICAgICB2YXIgdHBsVXJsID0gYXR0cnMudGVtcGxhdGVVcmw7XG4gICAgICBpZiAodHBsVXJsKSB7XG4gICAgICAgIGRyb3Bkb3duQ3RybC5kcm9wZG93bk1lbnVUZW1wbGF0ZVVybCA9IHRwbFVybDtcbiAgICAgIH1cblxuICAgICAgaWYgKCFkcm9wZG93bkN0cmwuZHJvcGRvd25NZW51KSB7XG4gICAgICAgIGRyb3Bkb3duQ3RybC5kcm9wZG93bk1lbnUgPSBlbGVtZW50O1xuICAgICAgfVxuICAgIH1cbiAgfTtcbn0pXG5cbi5kaXJlY3RpdmUoJ3VpYktleWJvYXJkTmF2JywgZnVuY3Rpb24oKSB7XG4gIHJldHVybiB7XG4gICAgcmVzdHJpY3Q6ICdBJyxcbiAgICByZXF1aXJlOiAnP151aWJEcm9wZG93bicsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHJzLCBkcm9wZG93bkN0cmwpIHtcbiAgICAgIGVsZW1lbnQuYmluZCgna2V5ZG93bicsIGZ1bmN0aW9uKGUpIHtcbiAgICAgICAgaWYgKFszOCwgNDBdLmluZGV4T2YoZS53aGljaCkgIT09IC0xKSB7XG4gICAgICAgICAgZS5wcmV2ZW50RGVmYXVsdCgpO1xuICAgICAgICAgIGUuc3RvcFByb3BhZ2F0aW9uKCk7XG5cbiAgICAgICAgICB2YXIgZWxlbXMgPSBkcm9wZG93bkN0cmwuZHJvcGRvd25NZW51LmZpbmQoJ2EnKTtcblxuICAgICAgICAgIHN3aXRjaCAoZS53aGljaCkge1xuICAgICAgICAgICAgY2FzZSAoNDApOiB7IC8vIERvd25cbiAgICAgICAgICAgICAgaWYgKCFhbmd1bGFyLmlzTnVtYmVyKGRyb3Bkb3duQ3RybC5zZWxlY3RlZE9wdGlvbikpIHtcbiAgICAgICAgICAgICAgICBkcm9wZG93bkN0cmwuc2VsZWN0ZWRPcHRpb24gPSAwO1xuICAgICAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgICAgIGRyb3Bkb3duQ3RybC5zZWxlY3RlZE9wdGlvbiA9IGRyb3Bkb3duQ3RybC5zZWxlY3RlZE9wdGlvbiA9PT0gZWxlbXMubGVuZ3RoIC0xID9cbiAgICAgICAgICAgICAgICAgIGRyb3Bkb3duQ3RybC5zZWxlY3RlZE9wdGlvbiA6IGRyb3Bkb3duQ3RybC5zZWxlY3RlZE9wdGlvbiArIDE7XG4gICAgICAgICAgICAgIH1cbiAgICAgICAgICAgICAgYnJlYWs7XG4gICAgICAgICAgICB9XG4gICAgICAgICAgICBjYXNlICgzOCk6IHsgLy8gVXBcbiAgICAgICAgICAgICAgaWYgKCFhbmd1bGFyLmlzTnVtYmVyKGRyb3Bkb3duQ3RybC5zZWxlY3RlZE9wdGlvbikpIHtcbiAgICAgICAgICAgICAgICBkcm9wZG93bkN0cmwuc2VsZWN0ZWRPcHRpb24gPSBlbGVtcy5sZW5ndGggLSAxO1xuICAgICAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgICAgIGRyb3Bkb3duQ3RybC5zZWxlY3RlZE9wdGlvbiA9IGRyb3Bkb3duQ3RybC5zZWxlY3RlZE9wdGlvbiA9PT0gMCA/XG4gICAgICAgICAgICAgICAgICAwIDogZHJvcGRvd25DdHJsLnNlbGVjdGVkT3B0aW9uIC0gMTtcbiAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgICBicmVhaztcbiAgICAgICAgICAgIH1cbiAgICAgICAgICB9XG4gICAgICAgICAgZWxlbXNbZHJvcGRvd25DdHJsLnNlbGVjdGVkT3B0aW9uXS5mb2N1cygpO1xuICAgICAgICB9XG4gICAgICB9KTtcbiAgICB9XG4gIH07XG59KVxuXG4uZGlyZWN0aXZlKCd1aWJEcm9wZG93blRvZ2dsZScsIGZ1bmN0aW9uKCkge1xuICByZXR1cm4ge1xuICAgIHJlcXVpcmU6ICc/XnVpYkRyb3Bkb3duJyxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMsIGRyb3Bkb3duQ3RybCkge1xuICAgICAgaWYgKCFkcm9wZG93bkN0cmwpIHtcbiAgICAgICAgcmV0dXJuO1xuICAgICAgfVxuXG4gICAgICBlbGVtZW50LmFkZENsYXNzKCdkcm9wZG93bi10b2dnbGUnKTtcblxuICAgICAgZHJvcGRvd25DdHJsLnRvZ2dsZUVsZW1lbnQgPSBlbGVtZW50O1xuXG4gICAgICB2YXIgdG9nZ2xlRHJvcGRvd24gPSBmdW5jdGlvbihldmVudCkge1xuICAgICAgICBldmVudC5wcmV2ZW50RGVmYXVsdCgpO1xuXG4gICAgICAgIGlmICghZWxlbWVudC5oYXNDbGFzcygnZGlzYWJsZWQnKSAmJiAhYXR0cnMuZGlzYWJsZWQpIHtcbiAgICAgICAgICBzY29wZS4kYXBwbHkoZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICBkcm9wZG93bkN0cmwudG9nZ2xlKCk7XG4gICAgICAgICAgfSk7XG4gICAgICAgIH1cbiAgICAgIH07XG5cbiAgICAgIGVsZW1lbnQuYmluZCgnY2xpY2snLCB0b2dnbGVEcm9wZG93bik7XG5cbiAgICAgIC8vIFdBSS1BUklBXG4gICAgICBlbGVtZW50LmF0dHIoeyAnYXJpYS1oYXNwb3B1cCc6IHRydWUsICdhcmlhLWV4cGFuZGVkJzogZmFsc2UgfSk7XG4gICAgICBzY29wZS4kd2F0Y2goZHJvcGRvd25DdHJsLmlzT3BlbiwgZnVuY3Rpb24oaXNPcGVuKSB7XG4gICAgICAgIGVsZW1lbnQuYXR0cignYXJpYS1leHBhbmRlZCcsICEhaXNPcGVuKTtcbiAgICAgIH0pO1xuXG4gICAgICBzY29wZS4kb24oJyRkZXN0cm95JywgZnVuY3Rpb24oKSB7XG4gICAgICAgIGVsZW1lbnQudW5iaW5kKCdjbGljaycsIHRvZ2dsZURyb3Bkb3duKTtcbiAgICAgIH0pO1xuICAgIH1cbiAgfTtcbn0pO1xuXG4vKiBEZXByZWNhdGVkIGRyb3Bkb3duIGJlbG93ICovXG5cbmFuZ3VsYXIubW9kdWxlKCd1aS5ib290c3RyYXAuZHJvcGRvd24nKVxuXG4udmFsdWUoJyRkcm9wZG93blN1cHByZXNzV2FybmluZycsIGZhbHNlKVxuXG4uc2VydmljZSgnZHJvcGRvd25TZXJ2aWNlJywgWyckbG9nJywgJyRkcm9wZG93blN1cHByZXNzV2FybmluZycsICd1aWJEcm9wZG93blNlcnZpY2UnLCBmdW5jdGlvbigkbG9nLCAkZHJvcGRvd25TdXBwcmVzc1dhcm5pbmcsIHVpYkRyb3Bkb3duU2VydmljZSkge1xuICBpZiAoISRkcm9wZG93blN1cHByZXNzV2FybmluZykge1xuICAgICRsb2cud2FybignZHJvcGRvd25TZXJ2aWNlIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliRHJvcGRvd25TZXJ2aWNlIGluc3RlYWQuJyk7XG4gIH1cblxuICBhbmd1bGFyLmV4dGVuZCh0aGlzLCB1aWJEcm9wZG93blNlcnZpY2UpO1xufV0pXG5cbi5jb250cm9sbGVyKCdEcm9wZG93bkNvbnRyb2xsZXInLCBbJyRzY29wZScsICckZWxlbWVudCcsICckYXR0cnMnLCAnJHBhcnNlJywgJ3VpYkRyb3Bkb3duQ29uZmlnJywgJ3VpYkRyb3Bkb3duU2VydmljZScsICckYW5pbWF0ZScsICckdWliUG9zaXRpb24nLCAnJGRvY3VtZW50JywgJyRjb21waWxlJywgJyR0ZW1wbGF0ZVJlcXVlc3QnLCAnJGxvZycsICckZHJvcGRvd25TdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkc2NvcGUsICRlbGVtZW50LCAkYXR0cnMsICRwYXJzZSwgZHJvcGRvd25Db25maWcsIHVpYkRyb3Bkb3duU2VydmljZSwgJGFuaW1hdGUsICRwb3NpdGlvbiwgJGRvY3VtZW50LCAkY29tcGlsZSwgJHRlbXBsYXRlUmVxdWVzdCwgJGxvZywgJGRyb3Bkb3duU3VwcHJlc3NXYXJuaW5nKSB7XG4gIGlmICghJGRyb3Bkb3duU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgJGxvZy53YXJuKCdEcm9wZG93bkNvbnRyb2xsZXIgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSBVaWJEcm9wZG93bkNvbnRyb2xsZXIgaW5zdGVhZC4nKTtcbiAgfVxuXG4gIHZhciBzZWxmID0gdGhpcyxcbiAgICBzY29wZSA9ICRzY29wZS4kbmV3KCksIC8vIGNyZWF0ZSBhIGNoaWxkIHNjb3BlIHNvIHdlIGFyZSBub3QgcG9sbHV0aW5nIG9yaWdpbmFsIG9uZVxuICAgIHRlbXBsYXRlU2NvcGUsXG4gICAgb3BlbkNsYXNzID0gZHJvcGRvd25Db25maWcub3BlbkNsYXNzLFxuICAgIGdldElzT3BlbixcbiAgICBzZXRJc09wZW4gPSBhbmd1bGFyLm5vb3AsXG4gICAgdG9nZ2xlSW52b2tlciA9ICRhdHRycy5vblRvZ2dsZSA/ICRwYXJzZSgkYXR0cnMub25Ub2dnbGUpIDogYW5ndWxhci5ub29wLFxuICAgIGFwcGVuZFRvQm9keSA9IGZhbHNlLFxuICAgIGtleW5hdkVuYWJsZWQgPWZhbHNlLFxuICAgIHNlbGVjdGVkT3B0aW9uID0gbnVsbDtcblxuXG4gICRlbGVtZW50LmFkZENsYXNzKCdkcm9wZG93bicpO1xuXG4gIHRoaXMuaW5pdCA9IGZ1bmN0aW9uKCkge1xuICAgIGlmICgkYXR0cnMuaXNPcGVuKSB7XG4gICAgICBnZXRJc09wZW4gPSAkcGFyc2UoJGF0dHJzLmlzT3Blbik7XG4gICAgICBzZXRJc09wZW4gPSBnZXRJc09wZW4uYXNzaWduO1xuXG4gICAgICAkc2NvcGUuJHdhdGNoKGdldElzT3BlbiwgZnVuY3Rpb24odmFsdWUpIHtcbiAgICAgICAgc2NvcGUuaXNPcGVuID0gISF2YWx1ZTtcbiAgICAgIH0pO1xuICAgIH1cblxuICAgIGFwcGVuZFRvQm9keSA9IGFuZ3VsYXIuaXNEZWZpbmVkKCRhdHRycy5kcm9wZG93bkFwcGVuZFRvQm9keSk7XG4gICAga2V5bmF2RW5hYmxlZCA9IGFuZ3VsYXIuaXNEZWZpbmVkKCRhdHRycy51aWJLZXlib2FyZE5hdik7XG5cbiAgICBpZiAoYXBwZW5kVG9Cb2R5ICYmIHNlbGYuZHJvcGRvd25NZW51KSB7XG4gICAgICAkZG9jdW1lbnQuZmluZCgnYm9keScpLmFwcGVuZChzZWxmLmRyb3Bkb3duTWVudSk7XG4gICAgICAkZWxlbWVudC5vbignJGRlc3Ryb3knLCBmdW5jdGlvbiBoYW5kbGVEZXN0cm95RXZlbnQoKSB7XG4gICAgICAgIHNlbGYuZHJvcGRvd25NZW51LnJlbW92ZSgpO1xuICAgICAgfSk7XG4gICAgfVxuICB9O1xuXG4gIHRoaXMudG9nZ2xlID0gZnVuY3Rpb24ob3Blbikge1xuICAgIHJldHVybiBzY29wZS5pc09wZW4gPSBhcmd1bWVudHMubGVuZ3RoID8gISFvcGVuIDogIXNjb3BlLmlzT3BlbjtcbiAgfTtcblxuICAvLyBBbGxvdyBvdGhlciBkaXJlY3RpdmVzIHRvIHdhdGNoIHN0YXR1c1xuICB0aGlzLmlzT3BlbiA9IGZ1bmN0aW9uKCkge1xuICAgIHJldHVybiBzY29wZS5pc09wZW47XG4gIH07XG5cbiAgc2NvcGUuZ2V0VG9nZ2xlRWxlbWVudCA9IGZ1bmN0aW9uKCkge1xuICAgIHJldHVybiBzZWxmLnRvZ2dsZUVsZW1lbnQ7XG4gIH07XG5cbiAgc2NvcGUuZ2V0QXV0b0Nsb3NlID0gZnVuY3Rpb24oKSB7XG4gICAgcmV0dXJuICRhdHRycy5hdXRvQ2xvc2UgfHwgJ2Fsd2F5cyc7IC8vb3IgJ291dHNpZGVDbGljaycgb3IgJ2Rpc2FibGVkJ1xuICB9O1xuXG4gIHNjb3BlLmdldEVsZW1lbnQgPSBmdW5jdGlvbigpIHtcbiAgICByZXR1cm4gJGVsZW1lbnQ7XG4gIH07XG5cbiAgc2NvcGUuaXNLZXluYXZFbmFibGVkID0gZnVuY3Rpb24oKSB7XG4gICAgcmV0dXJuIGtleW5hdkVuYWJsZWQ7XG4gIH07XG5cbiAgc2NvcGUuZm9jdXNEcm9wZG93bkVudHJ5ID0gZnVuY3Rpb24oa2V5Q29kZSkge1xuICAgIHZhciBlbGVtcyA9IHNlbGYuZHJvcGRvd25NZW51ID8gLy9JZiBhcHBlbmQgdG8gYm9keSBpcyB1c2VkLlxuICAgICAgKGFuZ3VsYXIuZWxlbWVudChzZWxmLmRyb3Bkb3duTWVudSkuZmluZCgnYScpKSA6XG4gICAgICAoYW5ndWxhci5lbGVtZW50KCRlbGVtZW50KS5maW5kKCd1bCcpLmVxKDApLmZpbmQoJ2EnKSk7XG5cbiAgICBzd2l0Y2ggKGtleUNvZGUpIHtcbiAgICAgIGNhc2UgKDQwKToge1xuICAgICAgICBpZiAoIWFuZ3VsYXIuaXNOdW1iZXIoc2VsZi5zZWxlY3RlZE9wdGlvbikpIHtcbiAgICAgICAgICBzZWxmLnNlbGVjdGVkT3B0aW9uID0gMDtcbiAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICBzZWxmLnNlbGVjdGVkT3B0aW9uID0gKHNlbGYuc2VsZWN0ZWRPcHRpb24gPT09IGVsZW1zLmxlbmd0aCAtMSA/XG4gICAgICAgICAgICBzZWxmLnNlbGVjdGVkT3B0aW9uIDpcbiAgICAgICAgICBzZWxmLnNlbGVjdGVkT3B0aW9uICsgMSk7XG4gICAgICAgIH1cbiAgICAgICAgYnJlYWs7XG4gICAgICB9XG4gICAgICBjYXNlICgzOCk6IHtcbiAgICAgICAgaWYgKCFhbmd1bGFyLmlzTnVtYmVyKHNlbGYuc2VsZWN0ZWRPcHRpb24pKSB7XG4gICAgICAgICAgc2VsZi5zZWxlY3RlZE9wdGlvbiA9IGVsZW1zLmxlbmd0aCAtIDE7XG4gICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgc2VsZi5zZWxlY3RlZE9wdGlvbiA9IHNlbGYuc2VsZWN0ZWRPcHRpb24gPT09IDAgP1xuICAgICAgICAgICAgMCA6IHNlbGYuc2VsZWN0ZWRPcHRpb24gLSAxO1xuICAgICAgICB9XG4gICAgICAgIGJyZWFrO1xuICAgICAgfVxuICAgIH1cbiAgICBlbGVtc1tzZWxmLnNlbGVjdGVkT3B0aW9uXS5mb2N1cygpO1xuICB9O1xuXG4gIHNjb3BlLmdldERyb3Bkb3duRWxlbWVudCA9IGZ1bmN0aW9uKCkge1xuICAgIHJldHVybiBzZWxmLmRyb3Bkb3duTWVudTtcbiAgfTtcblxuICBzY29wZS5mb2N1c1RvZ2dsZUVsZW1lbnQgPSBmdW5jdGlvbigpIHtcbiAgICBpZiAoc2VsZi50b2dnbGVFbGVtZW50KSB7XG4gICAgICBzZWxmLnRvZ2dsZUVsZW1lbnRbMF0uZm9jdXMoKTtcbiAgICB9XG4gIH07XG5cbiAgc2NvcGUuJHdhdGNoKCdpc09wZW4nLCBmdW5jdGlvbihpc09wZW4sIHdhc09wZW4pIHtcbiAgICBpZiAoYXBwZW5kVG9Cb2R5ICYmIHNlbGYuZHJvcGRvd25NZW51KSB7XG4gICAgICB2YXIgcG9zID0gJHBvc2l0aW9uLnBvc2l0aW9uRWxlbWVudHMoJGVsZW1lbnQsIHNlbGYuZHJvcGRvd25NZW51LCAnYm90dG9tLWxlZnQnLCB0cnVlKTtcbiAgICAgIHZhciBjc3MgPSB7XG4gICAgICAgIHRvcDogcG9zLnRvcCArICdweCcsXG4gICAgICAgIGRpc3BsYXk6IGlzT3BlbiA/ICdibG9jaycgOiAnbm9uZSdcbiAgICAgIH07XG5cbiAgICAgIHZhciByaWdodGFsaWduID0gc2VsZi5kcm9wZG93bk1lbnUuaGFzQ2xhc3MoJ2Ryb3Bkb3duLW1lbnUtcmlnaHQnKTtcbiAgICAgIGlmICghcmlnaHRhbGlnbikge1xuICAgICAgICBjc3MubGVmdCA9IHBvcy5sZWZ0ICsgJ3B4JztcbiAgICAgICAgY3NzLnJpZ2h0ID0gJ2F1dG8nO1xuICAgICAgfSBlbHNlIHtcbiAgICAgICAgY3NzLmxlZnQgPSAnYXV0byc7XG4gICAgICAgIGNzcy5yaWdodCA9ICh3aW5kb3cuaW5uZXJXaWR0aCAtIChwb3MubGVmdCArICRlbGVtZW50LnByb3AoJ29mZnNldFdpZHRoJykpKSArICdweCc7XG4gICAgICB9XG5cbiAgICAgIHNlbGYuZHJvcGRvd25NZW51LmNzcyhjc3MpO1xuICAgIH1cblxuICAgICRhbmltYXRlW2lzT3BlbiA/ICdhZGRDbGFzcycgOiAncmVtb3ZlQ2xhc3MnXSgkZWxlbWVudCwgb3BlbkNsYXNzKS50aGVuKGZ1bmN0aW9uKCkge1xuICAgICAgaWYgKGFuZ3VsYXIuaXNEZWZpbmVkKGlzT3BlbikgJiYgaXNPcGVuICE9PSB3YXNPcGVuKSB7XG4gICAgICAgIHRvZ2dsZUludm9rZXIoJHNjb3BlLCB7IG9wZW46ICEhaXNPcGVuIH0pO1xuICAgICAgfVxuICAgIH0pO1xuXG4gICAgaWYgKGlzT3Blbikge1xuICAgICAgaWYgKHNlbGYuZHJvcGRvd25NZW51VGVtcGxhdGVVcmwpIHtcbiAgICAgICAgJHRlbXBsYXRlUmVxdWVzdChzZWxmLmRyb3Bkb3duTWVudVRlbXBsYXRlVXJsKS50aGVuKGZ1bmN0aW9uKHRwbENvbnRlbnQpIHtcbiAgICAgICAgICB0ZW1wbGF0ZVNjb3BlID0gc2NvcGUuJG5ldygpO1xuICAgICAgICAgICRjb21waWxlKHRwbENvbnRlbnQudHJpbSgpKSh0ZW1wbGF0ZVNjb3BlLCBmdW5jdGlvbihkcm9wZG93bkVsZW1lbnQpIHtcbiAgICAgICAgICAgIHZhciBuZXdFbCA9IGRyb3Bkb3duRWxlbWVudDtcbiAgICAgICAgICAgIHNlbGYuZHJvcGRvd25NZW51LnJlcGxhY2VXaXRoKG5ld0VsKTtcbiAgICAgICAgICAgIHNlbGYuZHJvcGRvd25NZW51ID0gbmV3RWw7XG4gICAgICAgICAgfSk7XG4gICAgICAgIH0pO1xuICAgICAgfVxuXG4gICAgICBzY29wZS5mb2N1c1RvZ2dsZUVsZW1lbnQoKTtcbiAgICAgIHVpYkRyb3Bkb3duU2VydmljZS5vcGVuKHNjb3BlKTtcbiAgICB9IGVsc2Uge1xuICAgICAgaWYgKHNlbGYuZHJvcGRvd25NZW51VGVtcGxhdGVVcmwpIHtcbiAgICAgICAgaWYgKHRlbXBsYXRlU2NvcGUpIHtcbiAgICAgICAgICB0ZW1wbGF0ZVNjb3BlLiRkZXN0cm95KCk7XG4gICAgICAgIH1cbiAgICAgICAgdmFyIG5ld0VsID0gYW5ndWxhci5lbGVtZW50KCc8dWwgY2xhc3M9XCJkcm9wZG93bi1tZW51XCI+PC91bD4nKTtcbiAgICAgICAgc2VsZi5kcm9wZG93bk1lbnUucmVwbGFjZVdpdGgobmV3RWwpO1xuICAgICAgICBzZWxmLmRyb3Bkb3duTWVudSA9IG5ld0VsO1xuICAgICAgfVxuXG4gICAgICB1aWJEcm9wZG93blNlcnZpY2UuY2xvc2Uoc2NvcGUpO1xuICAgICAgc2VsZi5zZWxlY3RlZE9wdGlvbiA9IG51bGw7XG4gICAgfVxuXG4gICAgaWYgKGFuZ3VsYXIuaXNGdW5jdGlvbihzZXRJc09wZW4pKSB7XG4gICAgICBzZXRJc09wZW4oJHNjb3BlLCBpc09wZW4pO1xuICAgIH1cbiAgfSk7XG5cbiAgJHNjb3BlLiRvbignJGxvY2F0aW9uQ2hhbmdlU3VjY2VzcycsIGZ1bmN0aW9uKCkge1xuICAgIGlmIChzY29wZS5nZXRBdXRvQ2xvc2UoKSAhPT0gJ2Rpc2FibGVkJykge1xuICAgICAgc2NvcGUuaXNPcGVuID0gZmFsc2U7XG4gICAgfVxuICB9KTtcblxuICB2YXIgb2ZmRGVzdHJveSA9ICRzY29wZS4kb24oJyRkZXN0cm95JywgZnVuY3Rpb24oKSB7XG4gICAgc2NvcGUuJGRlc3Ryb3koKTtcbiAgfSk7XG4gIHNjb3BlLiRvbignJGRlc3Ryb3knLCBvZmZEZXN0cm95KTtcbn1dKVxuXG4uZGlyZWN0aXZlKCdkcm9wZG93bicsIFsnJGxvZycsICckZHJvcGRvd25TdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkbG9nLCAkZHJvcGRvd25TdXBwcmVzc1dhcm5pbmcpIHtcbiAgcmV0dXJuIHtcbiAgICBjb250cm9sbGVyOiAnRHJvcGRvd25Db250cm9sbGVyJyxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMsIGRyb3Bkb3duQ3RybCkge1xuICAgICAgaWYgKCEkZHJvcGRvd25TdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgJGxvZy53YXJuKCdkcm9wZG93biBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi1kcm9wZG93biBpbnN0ZWFkLicpO1xuICAgICAgfVxuXG4gICAgICBkcm9wZG93bkN0cmwuaW5pdCgpO1xuICAgIH1cbiAgfTtcbn1dKVxuXG4uZGlyZWN0aXZlKCdkcm9wZG93bk1lbnUnLCBbJyRsb2cnLCAnJGRyb3Bkb3duU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJGxvZywgJGRyb3Bkb3duU3VwcHJlc3NXYXJuaW5nKSB7XG4gIHJldHVybiB7XG4gICAgcmVzdHJpY3Q6ICdBQycsXG4gICAgcmVxdWlyZTogJz9eZHJvcGRvd24nLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgZHJvcGRvd25DdHJsKSB7XG4gICAgICBpZiAoIWRyb3Bkb3duQ3RybCB8fCBhbmd1bGFyLmlzRGVmaW5lZChhdHRycy5kcm9wZG93bk5lc3RlZCkpIHtcbiAgICAgICAgcmV0dXJuO1xuICAgICAgfVxuXG4gICAgICBpZiAoISRkcm9wZG93blN1cHByZXNzV2FybmluZykge1xuICAgICAgICAkbG9nLndhcm4oJ2Ryb3Bkb3duLW1lbnUgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSB1aWItZHJvcGRvd24tbWVudSBpbnN0ZWFkLicpO1xuICAgICAgfVxuXG4gICAgICBlbGVtZW50LmFkZENsYXNzKCdkcm9wZG93bi1tZW51Jyk7XG5cbiAgICAgIHZhciB0cGxVcmwgPSBhdHRycy50ZW1wbGF0ZVVybDtcbiAgICAgIGlmICh0cGxVcmwpIHtcbiAgICAgICAgZHJvcGRvd25DdHJsLmRyb3Bkb3duTWVudVRlbXBsYXRlVXJsID0gdHBsVXJsO1xuICAgICAgfVxuXG4gICAgICBpZiAoIWRyb3Bkb3duQ3RybC5kcm9wZG93bk1lbnUpIHtcbiAgICAgICAgZHJvcGRvd25DdHJsLmRyb3Bkb3duTWVudSA9IGVsZW1lbnQ7XG4gICAgICB9XG4gICAgfVxuICB9O1xufV0pXG5cbi5kaXJlY3RpdmUoJ2tleWJvYXJkTmF2JywgWyckbG9nJywgJyRkcm9wZG93blN1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRsb2csICRkcm9wZG93blN1cHByZXNzV2FybmluZykge1xuICByZXR1cm4ge1xuICAgIHJlc3RyaWN0OiAnQScsXG4gICAgcmVxdWlyZTogJz9eZHJvcGRvd24nLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgZHJvcGRvd25DdHJsKSB7XG4gICAgICBpZiAoISRkcm9wZG93blN1cHByZXNzV2FybmluZykge1xuICAgICAgICAkbG9nLndhcm4oJ2tleWJvYXJkLW5hdiBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi1rZXlib2FyZC1uYXYgaW5zdGVhZC4nKTtcbiAgICAgIH1cblxuICAgICAgZWxlbWVudC5iaW5kKCdrZXlkb3duJywgZnVuY3Rpb24oZSkge1xuICAgICAgICBpZiAoWzM4LCA0MF0uaW5kZXhPZihlLndoaWNoKSAhPT0gLTEpIHtcbiAgICAgICAgICBlLnByZXZlbnREZWZhdWx0KCk7XG4gICAgICAgICAgZS5zdG9wUHJvcGFnYXRpb24oKTtcblxuICAgICAgICAgIHZhciBlbGVtcyA9IGRyb3Bkb3duQ3RybC5kcm9wZG93bk1lbnUuZmluZCgnYScpO1xuXG4gICAgICAgICAgc3dpdGNoIChlLndoaWNoKSB7XG4gICAgICAgICAgICBjYXNlICg0MCk6IHsgLy8gRG93blxuICAgICAgICAgICAgICBpZiAoIWFuZ3VsYXIuaXNOdW1iZXIoZHJvcGRvd25DdHJsLnNlbGVjdGVkT3B0aW9uKSkge1xuICAgICAgICAgICAgICAgIGRyb3Bkb3duQ3RybC5zZWxlY3RlZE9wdGlvbiA9IDA7XG4gICAgICAgICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgICAgICAgZHJvcGRvd25DdHJsLnNlbGVjdGVkT3B0aW9uID0gZHJvcGRvd25DdHJsLnNlbGVjdGVkT3B0aW9uID09PSBlbGVtcy5sZW5ndGggLTEgP1xuICAgICAgICAgICAgICAgICAgZHJvcGRvd25DdHJsLnNlbGVjdGVkT3B0aW9uIDogZHJvcGRvd25DdHJsLnNlbGVjdGVkT3B0aW9uICsgMTtcbiAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgICBicmVhaztcbiAgICAgICAgICAgIH1cbiAgICAgICAgICAgIGNhc2UgKDM4KTogeyAvLyBVcFxuICAgICAgICAgICAgICBpZiAoIWFuZ3VsYXIuaXNOdW1iZXIoZHJvcGRvd25DdHJsLnNlbGVjdGVkT3B0aW9uKSkge1xuICAgICAgICAgICAgICAgIGRyb3Bkb3duQ3RybC5zZWxlY3RlZE9wdGlvbiA9IGVsZW1zLmxlbmd0aCAtIDE7XG4gICAgICAgICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgICAgICAgZHJvcGRvd25DdHJsLnNlbGVjdGVkT3B0aW9uID0gZHJvcGRvd25DdHJsLnNlbGVjdGVkT3B0aW9uID09PSAwID9cbiAgICAgICAgICAgICAgICAgIDAgOiBkcm9wZG93bkN0cmwuc2VsZWN0ZWRPcHRpb24gLSAxO1xuICAgICAgICAgICAgICB9XG4gICAgICAgICAgICAgIGJyZWFrO1xuICAgICAgICAgICAgfVxuICAgICAgICAgIH1cbiAgICAgICAgICBlbGVtc1tkcm9wZG93bkN0cmwuc2VsZWN0ZWRPcHRpb25dLmZvY3VzKCk7XG4gICAgICAgIH1cbiAgICAgIH0pO1xuICAgIH1cbiAgfTtcbn1dKVxuXG4uZGlyZWN0aXZlKCdkcm9wZG93blRvZ2dsZScsIFsnJGxvZycsICckZHJvcGRvd25TdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkbG9nLCAkZHJvcGRvd25TdXBwcmVzc1dhcm5pbmcpIHtcbiAgcmV0dXJuIHtcbiAgICByZXF1aXJlOiAnP15kcm9wZG93bicsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHJzLCBkcm9wZG93bkN0cmwpIHtcbiAgICAgIGlmICghJGRyb3Bkb3duU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICRsb2cud2FybignZHJvcGRvd24tdG9nZ2xlIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliLWRyb3Bkb3duLXRvZ2dsZSBpbnN0ZWFkLicpO1xuICAgICAgfVxuXG4gICAgICBpZiAoIWRyb3Bkb3duQ3RybCkge1xuICAgICAgICByZXR1cm47XG4gICAgICB9XG5cbiAgICAgIGVsZW1lbnQuYWRkQ2xhc3MoJ2Ryb3Bkb3duLXRvZ2dsZScpO1xuXG4gICAgICBkcm9wZG93bkN0cmwudG9nZ2xlRWxlbWVudCA9IGVsZW1lbnQ7XG5cbiAgICAgIHZhciB0b2dnbGVEcm9wZG93biA9IGZ1bmN0aW9uKGV2ZW50KSB7XG4gICAgICAgIGV2ZW50LnByZXZlbnREZWZhdWx0KCk7XG5cbiAgICAgICAgaWYgKCFlbGVtZW50Lmhhc0NsYXNzKCdkaXNhYmxlZCcpICYmICFhdHRycy5kaXNhYmxlZCkge1xuICAgICAgICAgIHNjb3BlLiRhcHBseShmdW5jdGlvbigpIHtcbiAgICAgICAgICAgIGRyb3Bkb3duQ3RybC50b2dnbGUoKTtcbiAgICAgICAgICB9KTtcbiAgICAgICAgfVxuICAgICAgfTtcblxuICAgICAgZWxlbWVudC5iaW5kKCdjbGljaycsIHRvZ2dsZURyb3Bkb3duKTtcblxuICAgICAgLy8gV0FJLUFSSUFcbiAgICAgIGVsZW1lbnQuYXR0cih7ICdhcmlhLWhhc3BvcHVwJzogdHJ1ZSwgJ2FyaWEtZXhwYW5kZWQnOiBmYWxzZSB9KTtcbiAgICAgIHNjb3BlLiR3YXRjaChkcm9wZG93bkN0cmwuaXNPcGVuLCBmdW5jdGlvbihpc09wZW4pIHtcbiAgICAgICAgZWxlbWVudC5hdHRyKCdhcmlhLWV4cGFuZGVkJywgISFpc09wZW4pO1xuICAgICAgfSk7XG5cbiAgICAgIHNjb3BlLiRvbignJGRlc3Ryb3knLCBmdW5jdGlvbigpIHtcbiAgICAgICAgZWxlbWVudC51bmJpbmQoJ2NsaWNrJywgdG9nZ2xlRHJvcGRvd24pO1xuICAgICAgfSk7XG4gICAgfVxuICB9O1xufV0pO1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLnN0YWNrZWRNYXAnLCBbXSlcbi8qKlxuICogQSBoZWxwZXIsIGludGVybmFsIGRhdGEgc3RydWN0dXJlIHRoYXQgYWN0cyBhcyBhIG1hcCBidXQgYWxzbyBhbGxvd3MgZ2V0dGluZyAvIHJlbW92aW5nXG4gKiBlbGVtZW50cyBpbiB0aGUgTElGTyBvcmRlclxuICovXG4gIC5mYWN0b3J5KCckJHN0YWNrZWRNYXAnLCBmdW5jdGlvbigpIHtcbiAgICByZXR1cm4ge1xuICAgICAgY3JlYXRlTmV3OiBmdW5jdGlvbigpIHtcbiAgICAgICAgdmFyIHN0YWNrID0gW107XG5cbiAgICAgICAgcmV0dXJuIHtcbiAgICAgICAgICBhZGQ6IGZ1bmN0aW9uKGtleSwgdmFsdWUpIHtcbiAgICAgICAgICAgIHN0YWNrLnB1c2goe1xuICAgICAgICAgICAgICBrZXk6IGtleSxcbiAgICAgICAgICAgICAgdmFsdWU6IHZhbHVlXG4gICAgICAgICAgICB9KTtcbiAgICAgICAgICB9LFxuICAgICAgICAgIGdldDogZnVuY3Rpb24oa2V5KSB7XG4gICAgICAgICAgICBmb3IgKHZhciBpID0gMDsgaSA8IHN0YWNrLmxlbmd0aDsgaSsrKSB7XG4gICAgICAgICAgICAgIGlmIChrZXkgPT0gc3RhY2tbaV0ua2V5KSB7XG4gICAgICAgICAgICAgICAgcmV0dXJuIHN0YWNrW2ldO1xuICAgICAgICAgICAgICB9XG4gICAgICAgICAgICB9XG4gICAgICAgICAgfSxcbiAgICAgICAgICBrZXlzOiBmdW5jdGlvbigpIHtcbiAgICAgICAgICAgIHZhciBrZXlzID0gW107XG4gICAgICAgICAgICBmb3IgKHZhciBpID0gMDsgaSA8IHN0YWNrLmxlbmd0aDsgaSsrKSB7XG4gICAgICAgICAgICAgIGtleXMucHVzaChzdGFja1tpXS5rZXkpO1xuICAgICAgICAgICAgfVxuICAgICAgICAgICAgcmV0dXJuIGtleXM7XG4gICAgICAgICAgfSxcbiAgICAgICAgICB0b3A6IGZ1bmN0aW9uKCkge1xuICAgICAgICAgICAgcmV0dXJuIHN0YWNrW3N0YWNrLmxlbmd0aCAtIDFdO1xuICAgICAgICAgIH0sXG4gICAgICAgICAgcmVtb3ZlOiBmdW5jdGlvbihrZXkpIHtcbiAgICAgICAgICAgIHZhciBpZHggPSAtMTtcbiAgICAgICAgICAgIGZvciAodmFyIGkgPSAwOyBpIDwgc3RhY2subGVuZ3RoOyBpKyspIHtcbiAgICAgICAgICAgICAgaWYgKGtleSA9PSBzdGFja1tpXS5rZXkpIHtcbiAgICAgICAgICAgICAgICBpZHggPSBpO1xuICAgICAgICAgICAgICAgIGJyZWFrO1xuICAgICAgICAgICAgICB9XG4gICAgICAgICAgICB9XG4gICAgICAgICAgICByZXR1cm4gc3RhY2suc3BsaWNlKGlkeCwgMSlbMF07XG4gICAgICAgICAgfSxcbiAgICAgICAgICByZW1vdmVUb3A6IGZ1bmN0aW9uKCkge1xuICAgICAgICAgICAgcmV0dXJuIHN0YWNrLnNwbGljZShzdGFjay5sZW5ndGggLSAxLCAxKVswXTtcbiAgICAgICAgICB9LFxuICAgICAgICAgIGxlbmd0aDogZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICByZXR1cm4gc3RhY2subGVuZ3RoO1xuICAgICAgICAgIH1cbiAgICAgICAgfTtcbiAgICAgIH1cbiAgICB9O1xuICB9KTtcbmFuZ3VsYXIubW9kdWxlKCd1aS5ib290c3RyYXAubW9kYWwnLCBbJ3VpLmJvb3RzdHJhcC5zdGFja2VkTWFwJ10pXG4vKipcbiAqIEEgaGVscGVyLCBpbnRlcm5hbCBkYXRhIHN0cnVjdHVyZSB0aGF0IHN0b3JlcyBhbGwgcmVmZXJlbmNlcyBhdHRhY2hlZCB0byBrZXlcbiAqL1xuICAuZmFjdG9yeSgnJCRtdWx0aU1hcCcsIGZ1bmN0aW9uKCkge1xuICAgIHJldHVybiB7XG4gICAgICBjcmVhdGVOZXc6IGZ1bmN0aW9uKCkge1xuICAgICAgICB2YXIgbWFwID0ge307XG5cbiAgICAgICAgcmV0dXJuIHtcbiAgICAgICAgICBlbnRyaWVzOiBmdW5jdGlvbigpIHtcbiAgICAgICAgICAgIHJldHVybiBPYmplY3Qua2V5cyhtYXApLm1hcChmdW5jdGlvbihrZXkpIHtcbiAgICAgICAgICAgICAgcmV0dXJuIHtcbiAgICAgICAgICAgICAgICBrZXk6IGtleSxcbiAgICAgICAgICAgICAgICB2YWx1ZTogbWFwW2tleV1cbiAgICAgICAgICAgICAgfTtcbiAgICAgICAgICAgIH0pO1xuICAgICAgICAgIH0sXG4gICAgICAgICAgZ2V0OiBmdW5jdGlvbihrZXkpIHtcbiAgICAgICAgICAgIHJldHVybiBtYXBba2V5XTtcbiAgICAgICAgICB9LFxuICAgICAgICAgIGhhc0tleTogZnVuY3Rpb24oa2V5KSB7XG4gICAgICAgICAgICByZXR1cm4gISFtYXBba2V5XTtcbiAgICAgICAgICB9LFxuICAgICAgICAgIGtleXM6IGZ1bmN0aW9uKCkge1xuICAgICAgICAgICAgcmV0dXJuIE9iamVjdC5rZXlzKG1hcCk7XG4gICAgICAgICAgfSxcbiAgICAgICAgICBwdXQ6IGZ1bmN0aW9uKGtleSwgdmFsdWUpIHtcbiAgICAgICAgICAgIGlmICghbWFwW2tleV0pIHtcbiAgICAgICAgICAgICAgbWFwW2tleV0gPSBbXTtcbiAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgbWFwW2tleV0ucHVzaCh2YWx1ZSk7XG4gICAgICAgICAgfSxcbiAgICAgICAgICByZW1vdmU6IGZ1bmN0aW9uKGtleSwgdmFsdWUpIHtcbiAgICAgICAgICAgIHZhciB2YWx1ZXMgPSBtYXBba2V5XTtcblxuICAgICAgICAgICAgaWYgKCF2YWx1ZXMpIHtcbiAgICAgICAgICAgICAgcmV0dXJuO1xuICAgICAgICAgICAgfVxuXG4gICAgICAgICAgICB2YXIgaWR4ID0gdmFsdWVzLmluZGV4T2YodmFsdWUpO1xuXG4gICAgICAgICAgICBpZiAoaWR4ICE9PSAtMSkge1xuICAgICAgICAgICAgICB2YWx1ZXMuc3BsaWNlKGlkeCwgMSk7XG4gICAgICAgICAgICB9XG5cbiAgICAgICAgICAgIGlmICghdmFsdWVzLmxlbmd0aCkge1xuICAgICAgICAgICAgICBkZWxldGUgbWFwW2tleV07XG4gICAgICAgICAgICB9XG4gICAgICAgICAgfVxuICAgICAgICB9O1xuICAgICAgfVxuICAgIH07XG4gIH0pXG5cbi8qKlxuICogQSBoZWxwZXIgZGlyZWN0aXZlIGZvciB0aGUgJG1vZGFsIHNlcnZpY2UuIEl0IGNyZWF0ZXMgYSBiYWNrZHJvcCBlbGVtZW50LlxuICovXG4gIC5kaXJlY3RpdmUoJ3VpYk1vZGFsQmFja2Ryb3AnLCBbXG4gICAgICAgICAgICckYW5pbWF0ZScsICckaW5qZWN0b3InLCAnJHVpYk1vZGFsU3RhY2snLFxuICBmdW5jdGlvbigkYW5pbWF0ZSAsICAkaW5qZWN0b3IsICAgJG1vZGFsU3RhY2spIHtcbiAgICB2YXIgJGFuaW1hdGVDc3MgPSBudWxsO1xuXG4gICAgaWYgKCRpbmplY3Rvci5oYXMoJyRhbmltYXRlQ3NzJykpIHtcbiAgICAgICRhbmltYXRlQ3NzID0gJGluamVjdG9yLmdldCgnJGFuaW1hdGVDc3MnKTtcbiAgICB9XG5cbiAgICByZXR1cm4ge1xuICAgICAgcmVwbGFjZTogdHJ1ZSxcbiAgICAgIHRlbXBsYXRlVXJsOiAndGVtcGxhdGUvbW9kYWwvYmFja2Ryb3AuaHRtbCcsXG4gICAgICBjb21waWxlOiBmdW5jdGlvbih0RWxlbWVudCwgdEF0dHJzKSB7XG4gICAgICAgIHRFbGVtZW50LmFkZENsYXNzKHRBdHRycy5iYWNrZHJvcENsYXNzKTtcbiAgICAgICAgcmV0dXJuIGxpbmtGbjtcbiAgICAgIH1cbiAgICB9O1xuXG4gICAgZnVuY3Rpb24gbGlua0ZuKHNjb3BlLCBlbGVtZW50LCBhdHRycykge1xuICAgICAgLy8gVGVtcG9yYXJ5IGZpeCBmb3IgcHJlZml4aW5nXG4gICAgICBlbGVtZW50LmFkZENsYXNzKCdtb2RhbC1iYWNrZHJvcCcpO1xuXG4gICAgICBpZiAoYXR0cnMubW9kYWxJbkNsYXNzKSB7XG4gICAgICAgIGlmICgkYW5pbWF0ZUNzcykge1xuICAgICAgICAgICRhbmltYXRlQ3NzKGVsZW1lbnQsIHtcbiAgICAgICAgICAgIGFkZENsYXNzOiBhdHRycy5tb2RhbEluQ2xhc3NcbiAgICAgICAgICB9KS5zdGFydCgpO1xuICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICRhbmltYXRlLmFkZENsYXNzKGVsZW1lbnQsIGF0dHJzLm1vZGFsSW5DbGFzcyk7XG4gICAgICAgIH1cblxuICAgICAgICBzY29wZS4kb24oJG1vZGFsU3RhY2suTk9XX0NMT1NJTkdfRVZFTlQsIGZ1bmN0aW9uKGUsIHNldElzQXN5bmMpIHtcbiAgICAgICAgICB2YXIgZG9uZSA9IHNldElzQXN5bmMoKTtcbiAgICAgICAgICBpZiAoJGFuaW1hdGVDc3MpIHtcbiAgICAgICAgICAgICRhbmltYXRlQ3NzKGVsZW1lbnQsIHtcbiAgICAgICAgICAgICAgcmVtb3ZlQ2xhc3M6IGF0dHJzLm1vZGFsSW5DbGFzc1xuICAgICAgICAgICAgfSkuc3RhcnQoKS50aGVuKGRvbmUpO1xuICAgICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgICAkYW5pbWF0ZS5yZW1vdmVDbGFzcyhlbGVtZW50LCBhdHRycy5tb2RhbEluQ2xhc3MpLnRoZW4oZG9uZSk7XG4gICAgICAgICAgfVxuICAgICAgICB9KTtcbiAgICAgIH1cbiAgICB9XG4gIH1dKVxuXG4gIC5kaXJlY3RpdmUoJ3VpYk1vZGFsV2luZG93JywgW1xuICAgICAgICAgICAnJHVpYk1vZGFsU3RhY2snLCAnJHEnLCAnJGFuaW1hdGUnLCAnJGluamVjdG9yJyxcbiAgZnVuY3Rpb24oJG1vZGFsU3RhY2sgLCAgJHEgLCAgJGFuaW1hdGUsICAgJGluamVjdG9yKSB7XG4gICAgdmFyICRhbmltYXRlQ3NzID0gbnVsbDtcblxuICAgIGlmICgkaW5qZWN0b3IuaGFzKCckYW5pbWF0ZUNzcycpKSB7XG4gICAgICAkYW5pbWF0ZUNzcyA9ICRpbmplY3Rvci5nZXQoJyRhbmltYXRlQ3NzJyk7XG4gICAgfVxuXG4gICAgcmV0dXJuIHtcbiAgICAgIHNjb3BlOiB7XG4gICAgICAgIGluZGV4OiAnQCdcbiAgICAgIH0sXG4gICAgICByZXBsYWNlOiB0cnVlLFxuICAgICAgdHJhbnNjbHVkZTogdHJ1ZSxcbiAgICAgIHRlbXBsYXRlVXJsOiBmdW5jdGlvbih0RWxlbWVudCwgdEF0dHJzKSB7XG4gICAgICAgIHJldHVybiB0QXR0cnMudGVtcGxhdGVVcmwgfHwgJ3RlbXBsYXRlL21vZGFsL3dpbmRvdy5odG1sJztcbiAgICAgIH0sXG4gICAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMpIHtcbiAgICAgICAgZWxlbWVudC5hZGRDbGFzcyhhdHRycy53aW5kb3dDbGFzcyB8fCAnJyk7XG4gICAgICAgIGVsZW1lbnQuYWRkQ2xhc3MoYXR0cnMud2luZG93VG9wQ2xhc3MgfHwgJycpO1xuICAgICAgICBzY29wZS5zaXplID0gYXR0cnMuc2l6ZTtcblxuICAgICAgICBzY29wZS5jbG9zZSA9IGZ1bmN0aW9uKGV2dCkge1xuICAgICAgICAgIHZhciBtb2RhbCA9ICRtb2RhbFN0YWNrLmdldFRvcCgpO1xuICAgICAgICAgIGlmIChtb2RhbCAmJiBtb2RhbC52YWx1ZS5iYWNrZHJvcCAmJiBtb2RhbC52YWx1ZS5iYWNrZHJvcCAhPT0gJ3N0YXRpYycgJiYgKGV2dC50YXJnZXQgPT09IGV2dC5jdXJyZW50VGFyZ2V0KSkge1xuICAgICAgICAgICAgZXZ0LnByZXZlbnREZWZhdWx0KCk7XG4gICAgICAgICAgICBldnQuc3RvcFByb3BhZ2F0aW9uKCk7XG4gICAgICAgICAgICAkbW9kYWxTdGFjay5kaXNtaXNzKG1vZGFsLmtleSwgJ2JhY2tkcm9wIGNsaWNrJyk7XG4gICAgICAgICAgfVxuICAgICAgICB9O1xuXG4gICAgICAgIC8vIG1vdmVkIGZyb20gdGVtcGxhdGUgdG8gZml4IGlzc3VlICMyMjgwXG4gICAgICAgIGVsZW1lbnQub24oJ2NsaWNrJywgc2NvcGUuY2xvc2UpO1xuXG4gICAgICAgIC8vIFRoaXMgcHJvcGVydHkgaXMgb25seSBhZGRlZCB0byB0aGUgc2NvcGUgZm9yIHRoZSBwdXJwb3NlIG9mIGRldGVjdGluZyB3aGVuIHRoaXMgZGlyZWN0aXZlIGlzIHJlbmRlcmVkLlxuICAgICAgICAvLyBXZSBjYW4gZGV0ZWN0IHRoYXQgYnkgdXNpbmcgdGhpcyBwcm9wZXJ0eSBpbiB0aGUgdGVtcGxhdGUgYXNzb2NpYXRlZCB3aXRoIHRoaXMgZGlyZWN0aXZlIGFuZCB0aGVuIHVzZVxuICAgICAgICAvLyB7QGxpbmsgQXR0cmlidXRlIyRvYnNlcnZlfSBvbiBpdC4gRm9yIG1vcmUgZGV0YWlscyBwbGVhc2Ugc2VlIHtAbGluayBUYWJsZUNvbHVtblJlc2l6ZX0uXG4gICAgICAgIHNjb3BlLiRpc1JlbmRlcmVkID0gdHJ1ZTtcblxuICAgICAgICAvLyBEZWZlcnJlZCBvYmplY3QgdGhhdCB3aWxsIGJlIHJlc29sdmVkIHdoZW4gdGhpcyBtb2RhbCBpcyByZW5kZXIuXG4gICAgICAgIHZhciBtb2RhbFJlbmRlckRlZmVyT2JqID0gJHEuZGVmZXIoKTtcbiAgICAgICAgLy8gT2JzZXJ2ZSBmdW5jdGlvbiB3aWxsIGJlIGNhbGxlZCBvbiBuZXh0IGRpZ2VzdCBjeWNsZSBhZnRlciBjb21waWxhdGlvbiwgZW5zdXJpbmcgdGhhdCB0aGUgRE9NIGlzIHJlYWR5LlxuICAgICAgICAvLyBJbiBvcmRlciB0byB1c2UgdGhpcyB3YXkgb2YgZmluZGluZyB3aGV0aGVyIERPTSBpcyByZWFkeSwgd2UgbmVlZCB0byBvYnNlcnZlIGEgc2NvcGUgcHJvcGVydHkgdXNlZCBpbiBtb2RhbCdzIHRlbXBsYXRlLlxuICAgICAgICBhdHRycy4kb2JzZXJ2ZSgnbW9kYWxSZW5kZXInLCBmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgICAgIGlmICh2YWx1ZSA9PSAndHJ1ZScpIHtcbiAgICAgICAgICAgIG1vZGFsUmVuZGVyRGVmZXJPYmoucmVzb2x2ZSgpO1xuICAgICAgICAgIH1cbiAgICAgICAgfSk7XG5cbiAgICAgICAgbW9kYWxSZW5kZXJEZWZlck9iai5wcm9taXNlLnRoZW4oZnVuY3Rpb24oKSB7XG4gICAgICAgICAgdmFyIGFuaW1hdGlvblByb21pc2UgPSBudWxsO1xuXG4gICAgICAgICAgaWYgKGF0dHJzLm1vZGFsSW5DbGFzcykge1xuICAgICAgICAgICAgaWYgKCRhbmltYXRlQ3NzKSB7XG4gICAgICAgICAgICAgIGFuaW1hdGlvblByb21pc2UgPSAkYW5pbWF0ZUNzcyhlbGVtZW50LCB7XG4gICAgICAgICAgICAgICAgYWRkQ2xhc3M6IGF0dHJzLm1vZGFsSW5DbGFzc1xuICAgICAgICAgICAgICB9KS5zdGFydCgpO1xuICAgICAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICAgICAgYW5pbWF0aW9uUHJvbWlzZSA9ICRhbmltYXRlLmFkZENsYXNzKGVsZW1lbnQsIGF0dHJzLm1vZGFsSW5DbGFzcyk7XG4gICAgICAgICAgICB9XG5cbiAgICAgICAgICAgIHNjb3BlLiRvbigkbW9kYWxTdGFjay5OT1dfQ0xPU0lOR19FVkVOVCwgZnVuY3Rpb24oZSwgc2V0SXNBc3luYykge1xuICAgICAgICAgICAgICB2YXIgZG9uZSA9IHNldElzQXN5bmMoKTtcbiAgICAgICAgICAgICAgaWYgKCRhbmltYXRlQ3NzKSB7XG4gICAgICAgICAgICAgICAgJGFuaW1hdGVDc3MoZWxlbWVudCwge1xuICAgICAgICAgICAgICAgICAgcmVtb3ZlQ2xhc3M6IGF0dHJzLm1vZGFsSW5DbGFzc1xuICAgICAgICAgICAgICAgIH0pLnN0YXJ0KCkudGhlbihkb25lKTtcbiAgICAgICAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICAgICAgICAkYW5pbWF0ZS5yZW1vdmVDbGFzcyhlbGVtZW50LCBhdHRycy5tb2RhbEluQ2xhc3MpLnRoZW4oZG9uZSk7XG4gICAgICAgICAgICAgIH1cbiAgICAgICAgICAgIH0pO1xuICAgICAgICAgIH1cblxuXG4gICAgICAgICAgJHEud2hlbihhbmltYXRpb25Qcm9taXNlKS50aGVuKGZ1bmN0aW9uKCkge1xuICAgICAgICAgICAgdmFyIGlucHV0V2l0aEF1dG9mb2N1cyA9IGVsZW1lbnRbMF0ucXVlcnlTZWxlY3RvcignW2F1dG9mb2N1c10nKTtcbiAgICAgICAgICAgIC8qKlxuICAgICAgICAgICAgICogQXV0by1mb2N1c2luZyBvZiBhIGZyZXNobHktb3BlbmVkIG1vZGFsIGVsZW1lbnQgY2F1c2VzIGFueSBjaGlsZCBlbGVtZW50c1xuICAgICAgICAgICAgICogd2l0aCB0aGUgYXV0b2ZvY3VzIGF0dHJpYnV0ZSB0byBsb3NlIGZvY3VzLiBUaGlzIGlzIGFuIGlzc3VlIG9uIHRvdWNoXG4gICAgICAgICAgICAgKiBiYXNlZCBkZXZpY2VzIHdoaWNoIHdpbGwgc2hvdyBhbmQgdGhlbiBoaWRlIHRoZSBvbnNjcmVlbiBrZXlib2FyZC5cbiAgICAgICAgICAgICAqIEF0dGVtcHRzIHRvIHJlZm9jdXMgdGhlIGF1dG9mb2N1cyBlbGVtZW50IHZpYSBKYXZhU2NyaXB0IHdpbGwgbm90IHJlb3BlblxuICAgICAgICAgICAgICogdGhlIG9uc2NyZWVuIGtleWJvYXJkLiBGaXhlZCBieSB1cGRhdGVkIHRoZSBmb2N1c2luZyBsb2dpYyB0byBvbmx5IGF1dG9mb2N1c1xuICAgICAgICAgICAgICogdGhlIG1vZGFsIGVsZW1lbnQgaWYgdGhlIG1vZGFsIGRvZXMgbm90IGNvbnRhaW4gYW4gYXV0b2ZvY3VzIGVsZW1lbnQuXG4gICAgICAgICAgICAgKi9cbiAgICAgICAgICAgIGlmIChpbnB1dFdpdGhBdXRvZm9jdXMpIHtcbiAgICAgICAgICAgICAgaW5wdXRXaXRoQXV0b2ZvY3VzLmZvY3VzKCk7XG4gICAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgICBlbGVtZW50WzBdLmZvY3VzKCk7XG4gICAgICAgICAgICB9XG4gICAgICAgICAgfSk7XG5cbiAgICAgICAgICAvLyBOb3RpZnkge0BsaW5rICRtb2RhbFN0YWNrfSB0aGF0IG1vZGFsIGlzIHJlbmRlcmVkLlxuICAgICAgICAgIHZhciBtb2RhbCA9ICRtb2RhbFN0YWNrLmdldFRvcCgpO1xuICAgICAgICAgIGlmIChtb2RhbCkge1xuICAgICAgICAgICAgJG1vZGFsU3RhY2subW9kYWxSZW5kZXJlZChtb2RhbC5rZXkpO1xuICAgICAgICAgIH1cbiAgICAgICAgfSk7XG4gICAgICB9XG4gICAgfTtcbiAgfV0pXG5cbiAgLmRpcmVjdGl2ZSgndWliTW9kYWxBbmltYXRpb25DbGFzcycsIGZ1bmN0aW9uKCkge1xuICAgIHJldHVybiB7XG4gICAgICBjb21waWxlOiBmdW5jdGlvbih0RWxlbWVudCwgdEF0dHJzKSB7XG4gICAgICAgIGlmICh0QXR0cnMubW9kYWxBbmltYXRpb24pIHtcbiAgICAgICAgICB0RWxlbWVudC5hZGRDbGFzcyh0QXR0cnMudWliTW9kYWxBbmltYXRpb25DbGFzcyk7XG4gICAgICAgIH1cbiAgICAgIH1cbiAgICB9O1xuICB9KVxuXG4gIC5kaXJlY3RpdmUoJ3VpYk1vZGFsVHJhbnNjbHVkZScsIGZ1bmN0aW9uKCkge1xuICAgIHJldHVybiB7XG4gICAgICBsaW5rOiBmdW5jdGlvbigkc2NvcGUsICRlbGVtZW50LCAkYXR0cnMsIGNvbnRyb2xsZXIsICR0cmFuc2NsdWRlKSB7XG4gICAgICAgICR0cmFuc2NsdWRlKCRzY29wZS4kcGFyZW50LCBmdW5jdGlvbihjbG9uZSkge1xuICAgICAgICAgICRlbGVtZW50LmVtcHR5KCk7XG4gICAgICAgICAgJGVsZW1lbnQuYXBwZW5kKGNsb25lKTtcbiAgICAgICAgfSk7XG4gICAgICB9XG4gICAgfTtcbiAgfSlcblxuICAuZmFjdG9yeSgnJHVpYk1vZGFsU3RhY2snLCBbXG4gICAgICAgICAgICAgJyRhbmltYXRlJywgJyR0aW1lb3V0JywgJyRkb2N1bWVudCcsICckY29tcGlsZScsICckcm9vdFNjb3BlJyxcbiAgICAgICAgICAgICAnJHEnLFxuICAgICAgICAgICAgICckaW5qZWN0b3InLFxuICAgICAgICAgICAgICckJG11bHRpTWFwJyxcbiAgICAgICAgICAgICAnJCRzdGFja2VkTWFwJyxcbiAgICBmdW5jdGlvbigkYW5pbWF0ZSAsICAkdGltZW91dCAsICAkZG9jdW1lbnQgLCAgJGNvbXBpbGUgLCAgJHJvb3RTY29wZSAsXG4gICAgICAgICAgICAgICRxLFxuICAgICAgICAgICAgICAkaW5qZWN0b3IsXG4gICAgICAgICAgICAgICQkbXVsdGlNYXAsXG4gICAgICAgICAgICAgICQkc3RhY2tlZE1hcCkge1xuICAgICAgdmFyICRhbmltYXRlQ3NzID0gbnVsbDtcblxuICAgICAgaWYgKCRpbmplY3Rvci5oYXMoJyRhbmltYXRlQ3NzJykpIHtcbiAgICAgICAgJGFuaW1hdGVDc3MgPSAkaW5qZWN0b3IuZ2V0KCckYW5pbWF0ZUNzcycpO1xuICAgICAgfVxuXG4gICAgICB2YXIgT1BFTkVEX01PREFMX0NMQVNTID0gJ21vZGFsLW9wZW4nO1xuXG4gICAgICB2YXIgYmFja2Ryb3BEb21FbCwgYmFja2Ryb3BTY29wZTtcbiAgICAgIHZhciBvcGVuZWRXaW5kb3dzID0gJCRzdGFja2VkTWFwLmNyZWF0ZU5ldygpO1xuICAgICAgdmFyIG9wZW5lZENsYXNzZXMgPSAkJG11bHRpTWFwLmNyZWF0ZU5ldygpO1xuICAgICAgdmFyICRtb2RhbFN0YWNrID0ge1xuICAgICAgICBOT1dfQ0xPU0lOR19FVkVOVDogJ21vZGFsLnN0YWNrLm5vdy1jbG9zaW5nJ1xuICAgICAgfTtcblxuICAgICAgLy9Nb2RhbCBmb2N1cyBiZWhhdmlvclxuICAgICAgdmFyIGZvY3VzYWJsZUVsZW1lbnRMaXN0O1xuICAgICAgdmFyIGZvY3VzSW5kZXggPSAwO1xuICAgICAgdmFyIHRhYmFiYmxlU2VsZWN0b3IgPSAnYVtocmVmXSwgYXJlYVtocmVmXSwgaW5wdXQ6bm90KFtkaXNhYmxlZF0pLCAnICtcbiAgICAgICAgJ2J1dHRvbjpub3QoW2Rpc2FibGVkXSksc2VsZWN0Om5vdChbZGlzYWJsZWRdKSwgdGV4dGFyZWE6bm90KFtkaXNhYmxlZF0pLCAnICtcbiAgICAgICAgJ2lmcmFtZSwgb2JqZWN0LCBlbWJlZCwgKlt0YWJpbmRleF0sICpbY29udGVudGVkaXRhYmxlPXRydWVdJztcblxuICAgICAgZnVuY3Rpb24gYmFja2Ryb3BJbmRleCgpIHtcbiAgICAgICAgdmFyIHRvcEJhY2tkcm9wSW5kZXggPSAtMTtcbiAgICAgICAgdmFyIG9wZW5lZCA9IG9wZW5lZFdpbmRvd3Mua2V5cygpO1xuICAgICAgICBmb3IgKHZhciBpID0gMDsgaSA8IG9wZW5lZC5sZW5ndGg7IGkrKykge1xuICAgICAgICAgIGlmIChvcGVuZWRXaW5kb3dzLmdldChvcGVuZWRbaV0pLnZhbHVlLmJhY2tkcm9wKSB7XG4gICAgICAgICAgICB0b3BCYWNrZHJvcEluZGV4ID0gaTtcbiAgICAgICAgICB9XG4gICAgICAgIH1cbiAgICAgICAgcmV0dXJuIHRvcEJhY2tkcm9wSW5kZXg7XG4gICAgICB9XG5cbiAgICAgICRyb290U2NvcGUuJHdhdGNoKGJhY2tkcm9wSW5kZXgsIGZ1bmN0aW9uKG5ld0JhY2tkcm9wSW5kZXgpIHtcbiAgICAgICAgaWYgKGJhY2tkcm9wU2NvcGUpIHtcbiAgICAgICAgICBiYWNrZHJvcFNjb3BlLmluZGV4ID0gbmV3QmFja2Ryb3BJbmRleDtcbiAgICAgICAgfVxuICAgICAgfSk7XG5cbiAgICAgIGZ1bmN0aW9uIHJlbW92ZU1vZGFsV2luZG93KG1vZGFsSW5zdGFuY2UsIGVsZW1lbnRUb1JlY2VpdmVGb2N1cykge1xuICAgICAgICB2YXIgYm9keSA9ICRkb2N1bWVudC5maW5kKCdib2R5JykuZXEoMCk7XG4gICAgICAgIHZhciBtb2RhbFdpbmRvdyA9IG9wZW5lZFdpbmRvd3MuZ2V0KG1vZGFsSW5zdGFuY2UpLnZhbHVlO1xuXG4gICAgICAgIC8vY2xlYW4gdXAgdGhlIHN0YWNrXG4gICAgICAgIG9wZW5lZFdpbmRvd3MucmVtb3ZlKG1vZGFsSW5zdGFuY2UpO1xuXG4gICAgICAgIHJlbW92ZUFmdGVyQW5pbWF0ZShtb2RhbFdpbmRvdy5tb2RhbERvbUVsLCBtb2RhbFdpbmRvdy5tb2RhbFNjb3BlLCBmdW5jdGlvbigpIHtcbiAgICAgICAgICB2YXIgbW9kYWxCb2R5Q2xhc3MgPSBtb2RhbFdpbmRvdy5vcGVuZWRDbGFzcyB8fCBPUEVORURfTU9EQUxfQ0xBU1M7XG4gICAgICAgICAgb3BlbmVkQ2xhc3Nlcy5yZW1vdmUobW9kYWxCb2R5Q2xhc3MsIG1vZGFsSW5zdGFuY2UpO1xuICAgICAgICAgIGJvZHkudG9nZ2xlQ2xhc3MobW9kYWxCb2R5Q2xhc3MsIG9wZW5lZENsYXNzZXMuaGFzS2V5KG1vZGFsQm9keUNsYXNzKSk7XG4gICAgICAgICAgdG9nZ2xlVG9wV2luZG93Q2xhc3ModHJ1ZSk7XG4gICAgICAgIH0pO1xuICAgICAgICBjaGVja1JlbW92ZUJhY2tkcm9wKCk7XG5cbiAgICAgICAgLy9tb3ZlIGZvY3VzIHRvIHNwZWNpZmllZCBlbGVtZW50IGlmIGF2YWlsYWJsZSwgb3IgZWxzZSB0byBib2R5XG4gICAgICAgIGlmIChlbGVtZW50VG9SZWNlaXZlRm9jdXMgJiYgZWxlbWVudFRvUmVjZWl2ZUZvY3VzLmZvY3VzKSB7XG4gICAgICAgICAgZWxlbWVudFRvUmVjZWl2ZUZvY3VzLmZvY3VzKCk7XG4gICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgYm9keS5mb2N1cygpO1xuICAgICAgICB9XG4gICAgICB9XG5cbiAgICAgIC8vIEFkZCBvciByZW1vdmUgXCJ3aW5kb3dUb3BDbGFzc1wiIGZyb20gdGhlIHRvcCB3aW5kb3cgaW4gdGhlIHN0YWNrXG4gICAgICBmdW5jdGlvbiB0b2dnbGVUb3BXaW5kb3dDbGFzcyh0b2dnbGVTd2l0Y2gpIHtcbiAgICAgICAgdmFyIG1vZGFsV2luZG93O1xuXG4gICAgICAgIGlmIChvcGVuZWRXaW5kb3dzLmxlbmd0aCgpID4gMCkge1xuICAgICAgICAgIG1vZGFsV2luZG93ID0gb3BlbmVkV2luZG93cy50b3AoKS52YWx1ZTtcbiAgICAgICAgICBtb2RhbFdpbmRvdy5tb2RhbERvbUVsLnRvZ2dsZUNsYXNzKG1vZGFsV2luZG93LndpbmRvd1RvcENsYXNzIHx8ICcnLCB0b2dnbGVTd2l0Y2gpO1xuICAgICAgICB9XG4gICAgICB9XG5cbiAgICAgIGZ1bmN0aW9uIGNoZWNrUmVtb3ZlQmFja2Ryb3AoKSB7XG4gICAgICAgIC8vcmVtb3ZlIGJhY2tkcm9wIGlmIG5vIGxvbmdlciBuZWVkZWRcbiAgICAgICAgaWYgKGJhY2tkcm9wRG9tRWwgJiYgYmFja2Ryb3BJbmRleCgpID09IC0xKSB7XG4gICAgICAgICAgdmFyIGJhY2tkcm9wU2NvcGVSZWYgPSBiYWNrZHJvcFNjb3BlO1xuICAgICAgICAgIHJlbW92ZUFmdGVyQW5pbWF0ZShiYWNrZHJvcERvbUVsLCBiYWNrZHJvcFNjb3BlLCBmdW5jdGlvbigpIHtcbiAgICAgICAgICAgIGJhY2tkcm9wU2NvcGVSZWYgPSBudWxsO1xuICAgICAgICAgIH0pO1xuICAgICAgICAgIGJhY2tkcm9wRG9tRWwgPSB1bmRlZmluZWQ7XG4gICAgICAgICAgYmFja2Ryb3BTY29wZSA9IHVuZGVmaW5lZDtcbiAgICAgICAgfVxuICAgICAgfVxuXG4gICAgICBmdW5jdGlvbiByZW1vdmVBZnRlckFuaW1hdGUoZG9tRWwsIHNjb3BlLCBkb25lKSB7XG4gICAgICAgIHZhciBhc3luY0RlZmVycmVkO1xuICAgICAgICB2YXIgYXN5bmNQcm9taXNlID0gbnVsbDtcbiAgICAgICAgdmFyIHNldElzQXN5bmMgPSBmdW5jdGlvbigpIHtcbiAgICAgICAgICBpZiAoIWFzeW5jRGVmZXJyZWQpIHtcbiAgICAgICAgICAgIGFzeW5jRGVmZXJyZWQgPSAkcS5kZWZlcigpO1xuICAgICAgICAgICAgYXN5bmNQcm9taXNlID0gYXN5bmNEZWZlcnJlZC5wcm9taXNlO1xuICAgICAgICAgIH1cblxuICAgICAgICAgIHJldHVybiBmdW5jdGlvbiBhc3luY0RvbmUoKSB7XG4gICAgICAgICAgICBhc3luY0RlZmVycmVkLnJlc29sdmUoKTtcbiAgICAgICAgICB9O1xuICAgICAgICB9O1xuICAgICAgICBzY29wZS4kYnJvYWRjYXN0KCRtb2RhbFN0YWNrLk5PV19DTE9TSU5HX0VWRU5ULCBzZXRJc0FzeW5jKTtcblxuICAgICAgICAvLyBOb3RlIHRoYXQgaXQncyBpbnRlbnRpb25hbCB0aGF0IGFzeW5jUHJvbWlzZSBtaWdodCBiZSBudWxsLlxuICAgICAgICAvLyBUaGF0J3Mgd2hlbiBzZXRJc0FzeW5jIGhhcyBub3QgYmVlbiBjYWxsZWQgZHVyaW5nIHRoZVxuICAgICAgICAvLyBOT1dfQ0xPU0lOR19FVkVOVCBicm9hZGNhc3QuXG4gICAgICAgIHJldHVybiAkcS53aGVuKGFzeW5jUHJvbWlzZSkudGhlbihhZnRlckFuaW1hdGluZyk7XG5cbiAgICAgICAgZnVuY3Rpb24gYWZ0ZXJBbmltYXRpbmcoKSB7XG4gICAgICAgICAgaWYgKGFmdGVyQW5pbWF0aW5nLmRvbmUpIHtcbiAgICAgICAgICAgIHJldHVybjtcbiAgICAgICAgICB9XG4gICAgICAgICAgYWZ0ZXJBbmltYXRpbmcuZG9uZSA9IHRydWU7XG5cbiAgICAgICAgICBpZiAoJGFuaW1hdGVDc3MpIHtcbiAgICAgICAgICAgICRhbmltYXRlQ3NzKGRvbUVsLCB7XG4gICAgICAgICAgICAgIGV2ZW50OiAnbGVhdmUnXG4gICAgICAgICAgICB9KS5zdGFydCgpLnRoZW4oZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICAgIGRvbUVsLnJlbW92ZSgpO1xuICAgICAgICAgICAgfSk7XG4gICAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICAgICRhbmltYXRlLmxlYXZlKGRvbUVsKTtcbiAgICAgICAgICB9XG4gICAgICAgICAgc2NvcGUuJGRlc3Ryb3koKTtcbiAgICAgICAgICBpZiAoZG9uZSkge1xuICAgICAgICAgICAgZG9uZSgpO1xuICAgICAgICAgIH1cbiAgICAgICAgfVxuICAgICAgfVxuXG4gICAgICAkZG9jdW1lbnQuYmluZCgna2V5ZG93bicsIGZ1bmN0aW9uKGV2dCkge1xuICAgICAgICBpZiAoZXZ0LmlzRGVmYXVsdFByZXZlbnRlZCgpKSB7XG4gICAgICAgICAgcmV0dXJuIGV2dDtcbiAgICAgICAgfVxuXG4gICAgICAgIHZhciBtb2RhbCA9IG9wZW5lZFdpbmRvd3MudG9wKCk7XG4gICAgICAgIGlmIChtb2RhbCAmJiBtb2RhbC52YWx1ZS5rZXlib2FyZCkge1xuICAgICAgICAgIHN3aXRjaCAoZXZ0LndoaWNoKSB7XG4gICAgICAgICAgICBjYXNlIDI3OiB7XG4gICAgICAgICAgICAgIGV2dC5wcmV2ZW50RGVmYXVsdCgpO1xuICAgICAgICAgICAgICAkcm9vdFNjb3BlLiRhcHBseShmdW5jdGlvbigpIHtcbiAgICAgICAgICAgICAgICAkbW9kYWxTdGFjay5kaXNtaXNzKG1vZGFsLmtleSwgJ2VzY2FwZSBrZXkgcHJlc3MnKTtcbiAgICAgICAgICAgICAgfSk7XG4gICAgICAgICAgICAgIGJyZWFrO1xuICAgICAgICAgICAgfVxuICAgICAgICAgICAgY2FzZSA5OiB7XG4gICAgICAgICAgICAgICRtb2RhbFN0YWNrLmxvYWRGb2N1c0VsZW1lbnRMaXN0KG1vZGFsKTtcbiAgICAgICAgICAgICAgdmFyIGZvY3VzQ2hhbmdlZCA9IGZhbHNlO1xuICAgICAgICAgICAgICBpZiAoZXZ0LnNoaWZ0S2V5KSB7XG4gICAgICAgICAgICAgICAgaWYgKCRtb2RhbFN0YWNrLmlzRm9jdXNJbkZpcnN0SXRlbShldnQpKSB7XG4gICAgICAgICAgICAgICAgICBmb2N1c0NoYW5nZWQgPSAkbW9kYWxTdGFjay5mb2N1c0xhc3RGb2N1c2FibGVFbGVtZW50KCk7XG4gICAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgICAgIGlmICgkbW9kYWxTdGFjay5pc0ZvY3VzSW5MYXN0SXRlbShldnQpKSB7XG4gICAgICAgICAgICAgICAgICBmb2N1c0NoYW5nZWQgPSAkbW9kYWxTdGFjay5mb2N1c0ZpcnN0Rm9jdXNhYmxlRWxlbWVudCgpO1xuICAgICAgICAgICAgICAgIH1cbiAgICAgICAgICAgICAgfVxuXG4gICAgICAgICAgICAgIGlmIChmb2N1c0NoYW5nZWQpIHtcbiAgICAgICAgICAgICAgICBldnQucHJldmVudERlZmF1bHQoKTtcbiAgICAgICAgICAgICAgICBldnQuc3RvcFByb3BhZ2F0aW9uKCk7XG4gICAgICAgICAgICAgIH1cbiAgICAgICAgICAgICAgYnJlYWs7XG4gICAgICAgICAgICB9XG4gICAgICAgICAgfVxuICAgICAgICB9XG4gICAgICB9KTtcblxuICAgICAgJG1vZGFsU3RhY2sub3BlbiA9IGZ1bmN0aW9uKG1vZGFsSW5zdGFuY2UsIG1vZGFsKSB7XG4gICAgICAgIHZhciBtb2RhbE9wZW5lciA9ICRkb2N1bWVudFswXS5hY3RpdmVFbGVtZW50LFxuICAgICAgICAgIG1vZGFsQm9keUNsYXNzID0gbW9kYWwub3BlbmVkQ2xhc3MgfHwgT1BFTkVEX01PREFMX0NMQVNTO1xuXG4gICAgICAgIHRvZ2dsZVRvcFdpbmRvd0NsYXNzKGZhbHNlKTtcblxuICAgICAgICBvcGVuZWRXaW5kb3dzLmFkZChtb2RhbEluc3RhbmNlLCB7XG4gICAgICAgICAgZGVmZXJyZWQ6IG1vZGFsLmRlZmVycmVkLFxuICAgICAgICAgIHJlbmRlckRlZmVycmVkOiBtb2RhbC5yZW5kZXJEZWZlcnJlZCxcbiAgICAgICAgICBtb2RhbFNjb3BlOiBtb2RhbC5zY29wZSxcbiAgICAgICAgICBiYWNrZHJvcDogbW9kYWwuYmFja2Ryb3AsXG4gICAgICAgICAga2V5Ym9hcmQ6IG1vZGFsLmtleWJvYXJkLFxuICAgICAgICAgIG9wZW5lZENsYXNzOiBtb2RhbC5vcGVuZWRDbGFzcyxcbiAgICAgICAgICB3aW5kb3dUb3BDbGFzczogbW9kYWwud2luZG93VG9wQ2xhc3NcbiAgICAgICAgfSk7XG5cbiAgICAgICAgb3BlbmVkQ2xhc3Nlcy5wdXQobW9kYWxCb2R5Q2xhc3MsIG1vZGFsSW5zdGFuY2UpO1xuXG4gICAgICAgIHZhciBib2R5ID0gJGRvY3VtZW50LmZpbmQoJ2JvZHknKS5lcSgwKSxcbiAgICAgICAgICAgIGN1cnJCYWNrZHJvcEluZGV4ID0gYmFja2Ryb3BJbmRleCgpO1xuXG4gICAgICAgIGlmIChjdXJyQmFja2Ryb3BJbmRleCA+PSAwICYmICFiYWNrZHJvcERvbUVsKSB7XG4gICAgICAgICAgYmFja2Ryb3BTY29wZSA9ICRyb290U2NvcGUuJG5ldyh0cnVlKTtcbiAgICAgICAgICBiYWNrZHJvcFNjb3BlLmluZGV4ID0gY3VyckJhY2tkcm9wSW5kZXg7XG4gICAgICAgICAgdmFyIGFuZ3VsYXJCYWNrZ3JvdW5kRG9tRWwgPSBhbmd1bGFyLmVsZW1lbnQoJzxkaXYgdWliLW1vZGFsLWJhY2tkcm9wPVwibW9kYWwtYmFja2Ryb3BcIj48L2Rpdj4nKTtcbiAgICAgICAgICBhbmd1bGFyQmFja2dyb3VuZERvbUVsLmF0dHIoJ2JhY2tkcm9wLWNsYXNzJywgbW9kYWwuYmFja2Ryb3BDbGFzcyk7XG4gICAgICAgICAgaWYgKG1vZGFsLmFuaW1hdGlvbikge1xuICAgICAgICAgICAgYW5ndWxhckJhY2tncm91bmREb21FbC5hdHRyKCdtb2RhbC1hbmltYXRpb24nLCAndHJ1ZScpO1xuICAgICAgICAgIH1cbiAgICAgICAgICBiYWNrZHJvcERvbUVsID0gJGNvbXBpbGUoYW5ndWxhckJhY2tncm91bmREb21FbCkoYmFja2Ryb3BTY29wZSk7XG4gICAgICAgICAgYm9keS5hcHBlbmQoYmFja2Ryb3BEb21FbCk7XG4gICAgICAgIH1cblxuICAgICAgICB2YXIgYW5ndWxhckRvbUVsID0gYW5ndWxhci5lbGVtZW50KCc8ZGl2IHVpYi1tb2RhbC13aW5kb3c9XCJtb2RhbC13aW5kb3dcIj48L2Rpdj4nKTtcbiAgICAgICAgYW5ndWxhckRvbUVsLmF0dHIoe1xuICAgICAgICAgICd0ZW1wbGF0ZS11cmwnOiBtb2RhbC53aW5kb3dUZW1wbGF0ZVVybCxcbiAgICAgICAgICAnd2luZG93LWNsYXNzJzogbW9kYWwud2luZG93Q2xhc3MsXG4gICAgICAgICAgJ3dpbmRvdy10b3AtY2xhc3MnOiBtb2RhbC53aW5kb3dUb3BDbGFzcyxcbiAgICAgICAgICAnc2l6ZSc6IG1vZGFsLnNpemUsXG4gICAgICAgICAgJ2luZGV4Jzogb3BlbmVkV2luZG93cy5sZW5ndGgoKSAtIDEsXG4gICAgICAgICAgJ2FuaW1hdGUnOiAnYW5pbWF0ZSdcbiAgICAgICAgfSkuaHRtbChtb2RhbC5jb250ZW50KTtcbiAgICAgICAgaWYgKG1vZGFsLmFuaW1hdGlvbikge1xuICAgICAgICAgIGFuZ3VsYXJEb21FbC5hdHRyKCdtb2RhbC1hbmltYXRpb24nLCAndHJ1ZScpO1xuICAgICAgICB9XG5cbiAgICAgICAgdmFyIG1vZGFsRG9tRWwgPSAkY29tcGlsZShhbmd1bGFyRG9tRWwpKG1vZGFsLnNjb3BlKTtcbiAgICAgICAgb3BlbmVkV2luZG93cy50b3AoKS52YWx1ZS5tb2RhbERvbUVsID0gbW9kYWxEb21FbDtcbiAgICAgICAgb3BlbmVkV2luZG93cy50b3AoKS52YWx1ZS5tb2RhbE9wZW5lciA9IG1vZGFsT3BlbmVyO1xuICAgICAgICBib2R5LmFwcGVuZChtb2RhbERvbUVsKTtcbiAgICAgICAgYm9keS5hZGRDbGFzcyhtb2RhbEJvZHlDbGFzcyk7XG5cbiAgICAgICAgJG1vZGFsU3RhY2suY2xlYXJGb2N1c0xpc3RDYWNoZSgpO1xuICAgICAgfTtcblxuICAgICAgZnVuY3Rpb24gYnJvYWRjYXN0Q2xvc2luZyhtb2RhbFdpbmRvdywgcmVzdWx0T3JSZWFzb24sIGNsb3NpbmcpIHtcbiAgICAgICAgcmV0dXJuICFtb2RhbFdpbmRvdy52YWx1ZS5tb2RhbFNjb3BlLiRicm9hZGNhc3QoJ21vZGFsLmNsb3NpbmcnLCByZXN1bHRPclJlYXNvbiwgY2xvc2luZykuZGVmYXVsdFByZXZlbnRlZDtcbiAgICAgIH1cblxuICAgICAgJG1vZGFsU3RhY2suY2xvc2UgPSBmdW5jdGlvbihtb2RhbEluc3RhbmNlLCByZXN1bHQpIHtcbiAgICAgICAgdmFyIG1vZGFsV2luZG93ID0gb3BlbmVkV2luZG93cy5nZXQobW9kYWxJbnN0YW5jZSk7XG4gICAgICAgIGlmIChtb2RhbFdpbmRvdyAmJiBicm9hZGNhc3RDbG9zaW5nKG1vZGFsV2luZG93LCByZXN1bHQsIHRydWUpKSB7XG4gICAgICAgICAgbW9kYWxXaW5kb3cudmFsdWUubW9kYWxTY29wZS4kJHVpYkRlc3RydWN0aW9uU2NoZWR1bGVkID0gdHJ1ZTtcbiAgICAgICAgICBtb2RhbFdpbmRvdy52YWx1ZS5kZWZlcnJlZC5yZXNvbHZlKHJlc3VsdCk7XG4gICAgICAgICAgcmVtb3ZlTW9kYWxXaW5kb3cobW9kYWxJbnN0YW5jZSwgbW9kYWxXaW5kb3cudmFsdWUubW9kYWxPcGVuZXIpO1xuICAgICAgICAgIHJldHVybiB0cnVlO1xuICAgICAgICB9XG4gICAgICAgIHJldHVybiAhbW9kYWxXaW5kb3c7XG4gICAgICB9O1xuXG4gICAgICAkbW9kYWxTdGFjay5kaXNtaXNzID0gZnVuY3Rpb24obW9kYWxJbnN0YW5jZSwgcmVhc29uKSB7XG4gICAgICAgIHZhciBtb2RhbFdpbmRvdyA9IG9wZW5lZFdpbmRvd3MuZ2V0KG1vZGFsSW5zdGFuY2UpO1xuICAgICAgICBpZiAobW9kYWxXaW5kb3cgJiYgYnJvYWRjYXN0Q2xvc2luZyhtb2RhbFdpbmRvdywgcmVhc29uLCBmYWxzZSkpIHtcbiAgICAgICAgICBtb2RhbFdpbmRvdy52YWx1ZS5tb2RhbFNjb3BlLiQkdWliRGVzdHJ1Y3Rpb25TY2hlZHVsZWQgPSB0cnVlO1xuICAgICAgICAgIG1vZGFsV2luZG93LnZhbHVlLmRlZmVycmVkLnJlamVjdChyZWFzb24pO1xuICAgICAgICAgIHJlbW92ZU1vZGFsV2luZG93KG1vZGFsSW5zdGFuY2UsIG1vZGFsV2luZG93LnZhbHVlLm1vZGFsT3BlbmVyKTtcbiAgICAgICAgICByZXR1cm4gdHJ1ZTtcbiAgICAgICAgfVxuICAgICAgICByZXR1cm4gIW1vZGFsV2luZG93O1xuICAgICAgfTtcblxuICAgICAgJG1vZGFsU3RhY2suZGlzbWlzc0FsbCA9IGZ1bmN0aW9uKHJlYXNvbikge1xuICAgICAgICB2YXIgdG9wTW9kYWwgPSB0aGlzLmdldFRvcCgpO1xuICAgICAgICB3aGlsZSAodG9wTW9kYWwgJiYgdGhpcy5kaXNtaXNzKHRvcE1vZGFsLmtleSwgcmVhc29uKSkge1xuICAgICAgICAgIHRvcE1vZGFsID0gdGhpcy5nZXRUb3AoKTtcbiAgICAgICAgfVxuICAgICAgfTtcblxuICAgICAgJG1vZGFsU3RhY2suZ2V0VG9wID0gZnVuY3Rpb24oKSB7XG4gICAgICAgIHJldHVybiBvcGVuZWRXaW5kb3dzLnRvcCgpO1xuICAgICAgfTtcblxuICAgICAgJG1vZGFsU3RhY2subW9kYWxSZW5kZXJlZCA9IGZ1bmN0aW9uKG1vZGFsSW5zdGFuY2UpIHtcbiAgICAgICAgdmFyIG1vZGFsV2luZG93ID0gb3BlbmVkV2luZG93cy5nZXQobW9kYWxJbnN0YW5jZSk7XG4gICAgICAgIGlmIChtb2RhbFdpbmRvdykge1xuICAgICAgICAgIG1vZGFsV2luZG93LnZhbHVlLnJlbmRlckRlZmVycmVkLnJlc29sdmUoKTtcbiAgICAgICAgfVxuICAgICAgfTtcblxuICAgICAgJG1vZGFsU3RhY2suZm9jdXNGaXJzdEZvY3VzYWJsZUVsZW1lbnQgPSBmdW5jdGlvbigpIHtcbiAgICAgICAgaWYgKGZvY3VzYWJsZUVsZW1lbnRMaXN0Lmxlbmd0aCA+IDApIHtcbiAgICAgICAgICBmb2N1c2FibGVFbGVtZW50TGlzdFswXS5mb2N1cygpO1xuICAgICAgICAgIHJldHVybiB0cnVlO1xuICAgICAgICB9XG4gICAgICAgIHJldHVybiBmYWxzZTtcbiAgICAgIH07XG4gICAgICAkbW9kYWxTdGFjay5mb2N1c0xhc3RGb2N1c2FibGVFbGVtZW50ID0gZnVuY3Rpb24oKSB7XG4gICAgICAgIGlmIChmb2N1c2FibGVFbGVtZW50TGlzdC5sZW5ndGggPiAwKSB7XG4gICAgICAgICAgZm9jdXNhYmxlRWxlbWVudExpc3RbZm9jdXNhYmxlRWxlbWVudExpc3QubGVuZ3RoIC0gMV0uZm9jdXMoKTtcbiAgICAgICAgICByZXR1cm4gdHJ1ZTtcbiAgICAgICAgfVxuICAgICAgICByZXR1cm4gZmFsc2U7XG4gICAgICB9O1xuXG4gICAgICAkbW9kYWxTdGFjay5pc0ZvY3VzSW5GaXJzdEl0ZW0gPSBmdW5jdGlvbihldnQpIHtcbiAgICAgICAgaWYgKGZvY3VzYWJsZUVsZW1lbnRMaXN0Lmxlbmd0aCA+IDApIHtcbiAgICAgICAgICByZXR1cm4gKGV2dC50YXJnZXQgfHwgZXZ0LnNyY0VsZW1lbnQpID09IGZvY3VzYWJsZUVsZW1lbnRMaXN0WzBdO1xuICAgICAgICB9XG4gICAgICAgIHJldHVybiBmYWxzZTtcbiAgICAgIH07XG5cbiAgICAgICRtb2RhbFN0YWNrLmlzRm9jdXNJbkxhc3RJdGVtID0gZnVuY3Rpb24oZXZ0KSB7XG4gICAgICAgIGlmIChmb2N1c2FibGVFbGVtZW50TGlzdC5sZW5ndGggPiAwKSB7XG4gICAgICAgICAgcmV0dXJuIChldnQudGFyZ2V0IHx8IGV2dC5zcmNFbGVtZW50KSA9PSBmb2N1c2FibGVFbGVtZW50TGlzdFtmb2N1c2FibGVFbGVtZW50TGlzdC5sZW5ndGggLSAxXTtcbiAgICAgICAgfVxuICAgICAgICByZXR1cm4gZmFsc2U7XG4gICAgICB9O1xuXG4gICAgICAkbW9kYWxTdGFjay5jbGVhckZvY3VzTGlzdENhY2hlID0gZnVuY3Rpb24oKSB7XG4gICAgICAgIGZvY3VzYWJsZUVsZW1lbnRMaXN0ID0gW107XG4gICAgICAgIGZvY3VzSW5kZXggPSAwO1xuICAgICAgfTtcblxuICAgICAgJG1vZGFsU3RhY2subG9hZEZvY3VzRWxlbWVudExpc3QgPSBmdW5jdGlvbihtb2RhbFdpbmRvdykge1xuICAgICAgICBpZiAoZm9jdXNhYmxlRWxlbWVudExpc3QgPT09IHVuZGVmaW5lZCB8fCAhZm9jdXNhYmxlRWxlbWVudExpc3QubGVuZ3RoKSB7XG4gICAgICAgICAgaWYgKG1vZGFsV2luZG93KSB7XG4gICAgICAgICAgICB2YXIgbW9kYWxEb21FMSA9IG1vZGFsV2luZG93LnZhbHVlLm1vZGFsRG9tRWw7XG4gICAgICAgICAgICBpZiAobW9kYWxEb21FMSAmJiBtb2RhbERvbUUxLmxlbmd0aCkge1xuICAgICAgICAgICAgICBmb2N1c2FibGVFbGVtZW50TGlzdCA9IG1vZGFsRG9tRTFbMF0ucXVlcnlTZWxlY3RvckFsbCh0YWJhYmJsZVNlbGVjdG9yKTtcbiAgICAgICAgICAgIH1cbiAgICAgICAgICB9XG4gICAgICAgIH1cbiAgICAgIH07XG5cbiAgICAgIHJldHVybiAkbW9kYWxTdGFjaztcbiAgICB9XSlcblxuICAucHJvdmlkZXIoJyR1aWJNb2RhbCcsIGZ1bmN0aW9uKCkge1xuICAgIHZhciAkbW9kYWxQcm92aWRlciA9IHtcbiAgICAgIG9wdGlvbnM6IHtcbiAgICAgICAgYW5pbWF0aW9uOiB0cnVlLFxuICAgICAgICBiYWNrZHJvcDogdHJ1ZSwgLy9jYW4gYWxzbyBiZSBmYWxzZSBvciAnc3RhdGljJ1xuICAgICAgICBrZXlib2FyZDogdHJ1ZVxuICAgICAgfSxcbiAgICAgICRnZXQ6IFsnJGluamVjdG9yJywgJyRyb290U2NvcGUnLCAnJHEnLCAnJHRlbXBsYXRlUmVxdWVzdCcsICckY29udHJvbGxlcicsICckdWliTW9kYWxTdGFjaycsICckbW9kYWxTdXBwcmVzc1dhcm5pbmcnLCAnJGxvZycsXG4gICAgICAgIGZ1bmN0aW9uICgkaW5qZWN0b3IsICRyb290U2NvcGUsICRxLCAkdGVtcGxhdGVSZXF1ZXN0LCAkY29udHJvbGxlciwgJG1vZGFsU3RhY2ssICRtb2RhbFN1cHByZXNzV2FybmluZywgJGxvZykge1xuICAgICAgICAgIHZhciAkbW9kYWwgPSB7fTtcblxuICAgICAgICAgIGZ1bmN0aW9uIGdldFRlbXBsYXRlUHJvbWlzZShvcHRpb25zKSB7XG4gICAgICAgICAgICByZXR1cm4gb3B0aW9ucy50ZW1wbGF0ZSA/ICRxLndoZW4ob3B0aW9ucy50ZW1wbGF0ZSkgOlxuICAgICAgICAgICAgICAkdGVtcGxhdGVSZXF1ZXN0KGFuZ3VsYXIuaXNGdW5jdGlvbihvcHRpb25zLnRlbXBsYXRlVXJsKSA/IChvcHRpb25zLnRlbXBsYXRlVXJsKSgpIDogb3B0aW9ucy50ZW1wbGF0ZVVybCk7XG4gICAgICAgICAgfVxuXG4gICAgICAgICAgZnVuY3Rpb24gZ2V0UmVzb2x2ZVByb21pc2VzKHJlc29sdmVzKSB7XG4gICAgICAgICAgICB2YXIgcHJvbWlzZXNBcnIgPSBbXTtcbiAgICAgICAgICAgIGFuZ3VsYXIuZm9yRWFjaChyZXNvbHZlcywgZnVuY3Rpb24odmFsdWUpIHtcbiAgICAgICAgICAgICAgaWYgKGFuZ3VsYXIuaXNGdW5jdGlvbih2YWx1ZSkgfHwgYW5ndWxhci5pc0FycmF5KHZhbHVlKSkge1xuICAgICAgICAgICAgICAgIHByb21pc2VzQXJyLnB1c2goJHEud2hlbigkaW5qZWN0b3IuaW52b2tlKHZhbHVlKSkpO1xuICAgICAgICAgICAgICB9IGVsc2UgaWYgKGFuZ3VsYXIuaXNTdHJpbmcodmFsdWUpKSB7XG4gICAgICAgICAgICAgICAgcHJvbWlzZXNBcnIucHVzaCgkcS53aGVuKCRpbmplY3Rvci5nZXQodmFsdWUpKSk7XG4gICAgICAgICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgICAgICAgcHJvbWlzZXNBcnIucHVzaCgkcS53aGVuKHZhbHVlKSk7XG4gICAgICAgICAgICAgIH1cbiAgICAgICAgICAgIH0pO1xuICAgICAgICAgICAgcmV0dXJuIHByb21pc2VzQXJyO1xuICAgICAgICAgIH1cblxuICAgICAgICAgIHZhciBwcm9taXNlQ2hhaW4gPSBudWxsO1xuICAgICAgICAgICRtb2RhbC5nZXRQcm9taXNlQ2hhaW4gPSBmdW5jdGlvbigpIHtcbiAgICAgICAgICAgIHJldHVybiBwcm9taXNlQ2hhaW47XG4gICAgICAgICAgfTtcblxuICAgICAgICAgICRtb2RhbC5vcGVuID0gZnVuY3Rpb24obW9kYWxPcHRpb25zKSB7XG4gICAgICAgICAgICB2YXIgbW9kYWxSZXN1bHREZWZlcnJlZCA9ICRxLmRlZmVyKCk7XG4gICAgICAgICAgICB2YXIgbW9kYWxPcGVuZWREZWZlcnJlZCA9ICRxLmRlZmVyKCk7XG4gICAgICAgICAgICB2YXIgbW9kYWxSZW5kZXJEZWZlcnJlZCA9ICRxLmRlZmVyKCk7XG5cbiAgICAgICAgICAgIC8vcHJlcGFyZSBhbiBpbnN0YW5jZSBvZiBhIG1vZGFsIHRvIGJlIGluamVjdGVkIGludG8gY29udHJvbGxlcnMgYW5kIHJldHVybmVkIHRvIGEgY2FsbGVyXG4gICAgICAgICAgICB2YXIgbW9kYWxJbnN0YW5jZSA9IHtcbiAgICAgICAgICAgICAgcmVzdWx0OiBtb2RhbFJlc3VsdERlZmVycmVkLnByb21pc2UsXG4gICAgICAgICAgICAgIG9wZW5lZDogbW9kYWxPcGVuZWREZWZlcnJlZC5wcm9taXNlLFxuICAgICAgICAgICAgICByZW5kZXJlZDogbW9kYWxSZW5kZXJEZWZlcnJlZC5wcm9taXNlLFxuICAgICAgICAgICAgICBjbG9zZTogZnVuY3Rpb24gKHJlc3VsdCkge1xuICAgICAgICAgICAgICAgIHJldHVybiAkbW9kYWxTdGFjay5jbG9zZShtb2RhbEluc3RhbmNlLCByZXN1bHQpO1xuICAgICAgICAgICAgICB9LFxuICAgICAgICAgICAgICBkaXNtaXNzOiBmdW5jdGlvbiAocmVhc29uKSB7XG4gICAgICAgICAgICAgICAgcmV0dXJuICRtb2RhbFN0YWNrLmRpc21pc3MobW9kYWxJbnN0YW5jZSwgcmVhc29uKTtcbiAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgfTtcblxuICAgICAgICAgICAgLy9tZXJnZSBhbmQgY2xlYW4gdXAgb3B0aW9uc1xuICAgICAgICAgICAgbW9kYWxPcHRpb25zID0gYW5ndWxhci5leHRlbmQoe30sICRtb2RhbFByb3ZpZGVyLm9wdGlvbnMsIG1vZGFsT3B0aW9ucyk7XG4gICAgICAgICAgICBtb2RhbE9wdGlvbnMucmVzb2x2ZSA9IG1vZGFsT3B0aW9ucy5yZXNvbHZlIHx8IHt9O1xuXG4gICAgICAgICAgICAvL3ZlcmlmeSBvcHRpb25zXG4gICAgICAgICAgICBpZiAoIW1vZGFsT3B0aW9ucy50ZW1wbGF0ZSAmJiAhbW9kYWxPcHRpb25zLnRlbXBsYXRlVXJsKSB7XG4gICAgICAgICAgICAgIHRocm93IG5ldyBFcnJvcignT25lIG9mIHRlbXBsYXRlIG9yIHRlbXBsYXRlVXJsIG9wdGlvbnMgaXMgcmVxdWlyZWQuJyk7XG4gICAgICAgICAgICB9XG5cbiAgICAgICAgICAgIHZhciB0ZW1wbGF0ZUFuZFJlc29sdmVQcm9taXNlID1cbiAgICAgICAgICAgICAgJHEuYWxsKFtnZXRUZW1wbGF0ZVByb21pc2UobW9kYWxPcHRpb25zKV0uY29uY2F0KGdldFJlc29sdmVQcm9taXNlcyhtb2RhbE9wdGlvbnMucmVzb2x2ZSkpKTtcblxuICAgICAgICAgICAgZnVuY3Rpb24gcmVzb2x2ZVdpdGhUZW1wbGF0ZSgpIHtcbiAgICAgICAgICAgICAgcmV0dXJuIHRlbXBsYXRlQW5kUmVzb2x2ZVByb21pc2U7XG4gICAgICAgICAgICB9XG5cbiAgICAgICAgICAgIC8vIFdhaXQgZm9yIHRoZSByZXNvbHV0aW9uIG9mIHRoZSBleGlzdGluZyBwcm9taXNlIGNoYWluLlxuICAgICAgICAgICAgLy8gVGhlbiBzd2l0Y2ggdG8gb3VyIG93biBjb21iaW5lZCBwcm9taXNlIGRlcGVuZGVuY3kgKHJlZ2FyZGxlc3Mgb2YgaG93IHRoZSBwcmV2aW91cyBtb2RhbCBmYXJlZCkuXG4gICAgICAgICAgICAvLyBUaGVuIGFkZCB0byAkbW9kYWxTdGFjayBhbmQgcmVzb2x2ZSBvcGVuZWQuXG4gICAgICAgICAgICAvLyBGaW5hbGx5IGNsZWFuIHVwIHRoZSBjaGFpbiB2YXJpYWJsZSBpZiBubyBzdWJzZXF1ZW50IG1vZGFsIGhhcyBvdmVyd3JpdHRlbiBpdC5cbiAgICAgICAgICAgIHZhciBzYW1lUHJvbWlzZTtcbiAgICAgICAgICAgIHNhbWVQcm9taXNlID0gcHJvbWlzZUNoYWluID0gJHEuYWxsKFtwcm9taXNlQ2hhaW5dKVxuICAgICAgICAgICAgICAudGhlbihyZXNvbHZlV2l0aFRlbXBsYXRlLCByZXNvbHZlV2l0aFRlbXBsYXRlKVxuICAgICAgICAgICAgICAudGhlbihmdW5jdGlvbiByZXNvbHZlU3VjY2Vzcyh0cGxBbmRWYXJzKSB7XG5cbiAgICAgICAgICAgICAgICB2YXIgbW9kYWxTY29wZSA9IChtb2RhbE9wdGlvbnMuc2NvcGUgfHwgJHJvb3RTY29wZSkuJG5ldygpO1xuICAgICAgICAgICAgICAgIG1vZGFsU2NvcGUuJGNsb3NlID0gbW9kYWxJbnN0YW5jZS5jbG9zZTtcbiAgICAgICAgICAgICAgICBtb2RhbFNjb3BlLiRkaXNtaXNzID0gbW9kYWxJbnN0YW5jZS5kaXNtaXNzO1xuXG4gICAgICAgICAgICAgICAgbW9kYWxTY29wZS4kb24oJyRkZXN0cm95JywgZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICAgICAgICBpZiAoIW1vZGFsU2NvcGUuJCR1aWJEZXN0cnVjdGlvblNjaGVkdWxlZCkge1xuICAgICAgICAgICAgICAgICAgICBtb2RhbFNjb3BlLiRkaXNtaXNzKCckdWliVW5zY2hlZHVsZWREZXN0cnVjdGlvbicpO1xuICAgICAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgICAgIH0pO1xuXG4gICAgICAgICAgICAgICAgdmFyIGN0cmxJbnN0YW5jZSwgY3RybExvY2FscyA9IHt9O1xuICAgICAgICAgICAgICAgIHZhciByZXNvbHZlSXRlciA9IDE7XG5cbiAgICAgICAgICAgICAgICAvL2NvbnRyb2xsZXJzXG4gICAgICAgICAgICAgICAgaWYgKG1vZGFsT3B0aW9ucy5jb250cm9sbGVyKSB7XG4gICAgICAgICAgICAgICAgICBjdHJsTG9jYWxzLiRzY29wZSA9IG1vZGFsU2NvcGU7XG4gICAgICAgICAgICAgICAgICBjdHJsTG9jYWxzLiR1aWJNb2RhbEluc3RhbmNlID0gbW9kYWxJbnN0YW5jZTtcbiAgICAgICAgICAgICAgICAgIE9iamVjdC5kZWZpbmVQcm9wZXJ0eShjdHJsTG9jYWxzLCAnJG1vZGFsSW5zdGFuY2UnLCB7XG4gICAgICAgICAgICAgICAgICAgIGdldDogZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICAgICAgICAgICAgaWYgKCEkbW9kYWxTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgICAgICAgICAgICAgICAgICRsb2cud2FybignJG1vZGFsSW5zdGFuY2UgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSAkdWliTW9kYWxJbnN0YW5jZSBpbnN0ZWFkLicpO1xuICAgICAgICAgICAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgICAgICAgICAgIHJldHVybiBtb2RhbEluc3RhbmNlO1xuICAgICAgICAgICAgICAgICAgICB9XG4gICAgICAgICAgICAgICAgICB9KTtcbiAgICAgICAgICAgICAgICAgIGFuZ3VsYXIuZm9yRWFjaChtb2RhbE9wdGlvbnMucmVzb2x2ZSwgZnVuY3Rpb24odmFsdWUsIGtleSkge1xuICAgICAgICAgICAgICAgICAgICBjdHJsTG9jYWxzW2tleV0gPSB0cGxBbmRWYXJzW3Jlc29sdmVJdGVyKytdO1xuICAgICAgICAgICAgICAgICAgfSk7XG5cbiAgICAgICAgICAgICAgICAgIGN0cmxJbnN0YW5jZSA9ICRjb250cm9sbGVyKG1vZGFsT3B0aW9ucy5jb250cm9sbGVyLCBjdHJsTG9jYWxzKTtcbiAgICAgICAgICAgICAgICAgIGlmIChtb2RhbE9wdGlvbnMuY29udHJvbGxlckFzKSB7XG4gICAgICAgICAgICAgICAgICAgIGlmIChtb2RhbE9wdGlvbnMuYmluZFRvQ29udHJvbGxlcikge1xuICAgICAgICAgICAgICAgICAgICAgIGFuZ3VsYXIuZXh0ZW5kKGN0cmxJbnN0YW5jZSwgbW9kYWxTY29wZSk7XG4gICAgICAgICAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgICAgICAgICBtb2RhbFNjb3BlW21vZGFsT3B0aW9ucy5jb250cm9sbGVyQXNdID0gY3RybEluc3RhbmNlO1xuICAgICAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgICAgICRtb2RhbFN0YWNrLm9wZW4obW9kYWxJbnN0YW5jZSwge1xuICAgICAgICAgICAgICAgICAgc2NvcGU6IG1vZGFsU2NvcGUsXG4gICAgICAgICAgICAgICAgICBkZWZlcnJlZDogbW9kYWxSZXN1bHREZWZlcnJlZCxcbiAgICAgICAgICAgICAgICAgIHJlbmRlckRlZmVycmVkOiBtb2RhbFJlbmRlckRlZmVycmVkLFxuICAgICAgICAgICAgICAgICAgY29udGVudDogdHBsQW5kVmFyc1swXSxcbiAgICAgICAgICAgICAgICAgIGFuaW1hdGlvbjogbW9kYWxPcHRpb25zLmFuaW1hdGlvbixcbiAgICAgICAgICAgICAgICAgIGJhY2tkcm9wOiBtb2RhbE9wdGlvbnMuYmFja2Ryb3AsXG4gICAgICAgICAgICAgICAgICBrZXlib2FyZDogbW9kYWxPcHRpb25zLmtleWJvYXJkLFxuICAgICAgICAgICAgICAgICAgYmFja2Ryb3BDbGFzczogbW9kYWxPcHRpb25zLmJhY2tkcm9wQ2xhc3MsXG4gICAgICAgICAgICAgICAgICB3aW5kb3dUb3BDbGFzczogbW9kYWxPcHRpb25zLndpbmRvd1RvcENsYXNzLFxuICAgICAgICAgICAgICAgICAgd2luZG93Q2xhc3M6IG1vZGFsT3B0aW9ucy53aW5kb3dDbGFzcyxcbiAgICAgICAgICAgICAgICAgIHdpbmRvd1RlbXBsYXRlVXJsOiBtb2RhbE9wdGlvbnMud2luZG93VGVtcGxhdGVVcmwsXG4gICAgICAgICAgICAgICAgICBzaXplOiBtb2RhbE9wdGlvbnMuc2l6ZSxcbiAgICAgICAgICAgICAgICAgIG9wZW5lZENsYXNzOiBtb2RhbE9wdGlvbnMub3BlbmVkQ2xhc3NcbiAgICAgICAgICAgICAgICB9KTtcbiAgICAgICAgICAgICAgICBtb2RhbE9wZW5lZERlZmVycmVkLnJlc29sdmUodHJ1ZSk7XG5cbiAgICAgICAgICAgIH0sIGZ1bmN0aW9uIHJlc29sdmVFcnJvcihyZWFzb24pIHtcbiAgICAgICAgICAgICAgbW9kYWxPcGVuZWREZWZlcnJlZC5yZWplY3QocmVhc29uKTtcbiAgICAgICAgICAgICAgbW9kYWxSZXN1bHREZWZlcnJlZC5yZWplY3QocmVhc29uKTtcbiAgICAgICAgICAgIH0pXG4gICAgICAgICAgICAuZmluYWxseShmdW5jdGlvbigpIHtcbiAgICAgICAgICAgICAgaWYgKHByb21pc2VDaGFpbiA9PT0gc2FtZVByb21pc2UpIHtcbiAgICAgICAgICAgICAgICBwcm9taXNlQ2hhaW4gPSBudWxsO1xuICAgICAgICAgICAgICB9XG4gICAgICAgICAgICB9KTtcblxuICAgICAgICAgICAgcmV0dXJuIG1vZGFsSW5zdGFuY2U7XG4gICAgICAgICAgfTtcblxuICAgICAgICAgIHJldHVybiAkbW9kYWw7XG4gICAgICAgIH1cbiAgICAgIF1cbiAgICB9O1xuXG4gICAgcmV0dXJuICRtb2RhbFByb3ZpZGVyO1xuICB9KTtcblxuLyogZGVwcmVjYXRlZCBtb2RhbCBiZWxvdyAqL1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLm1vZGFsJylcblxuICAudmFsdWUoJyRtb2RhbFN1cHByZXNzV2FybmluZycsIGZhbHNlKVxuXG4gIC8qKlxuICAgKiBBIGhlbHBlciBkaXJlY3RpdmUgZm9yIHRoZSAkbW9kYWwgc2VydmljZS4gSXQgY3JlYXRlcyBhIGJhY2tkcm9wIGVsZW1lbnQuXG4gICAqL1xuICAuZGlyZWN0aXZlKCdtb2RhbEJhY2tkcm9wJywgW1xuICAgICckYW5pbWF0ZScsICckaW5qZWN0b3InLCAnJG1vZGFsU3RhY2snLCAnJGxvZycsICckbW9kYWxTdXBwcmVzc1dhcm5pbmcnLFxuICAgIGZ1bmN0aW9uKCRhbmltYXRlICwgICRpbmplY3RvciwgICAkbW9kYWxTdGFjaywgJGxvZywgJG1vZGFsU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICB2YXIgJGFuaW1hdGVDc3MgPSBudWxsO1xuXG4gICAgICBpZiAoJGluamVjdG9yLmhhcygnJGFuaW1hdGVDc3MnKSkge1xuICAgICAgICAkYW5pbWF0ZUNzcyA9ICRpbmplY3Rvci5nZXQoJyRhbmltYXRlQ3NzJyk7XG4gICAgICB9XG5cbiAgICAgIHJldHVybiB7XG4gICAgICAgIHJlcGxhY2U6IHRydWUsXG4gICAgICAgIHRlbXBsYXRlVXJsOiAndGVtcGxhdGUvbW9kYWwvYmFja2Ryb3AuaHRtbCcsXG4gICAgICAgIGNvbXBpbGU6IGZ1bmN0aW9uKHRFbGVtZW50LCB0QXR0cnMpIHtcbiAgICAgICAgICB0RWxlbWVudC5hZGRDbGFzcyh0QXR0cnMuYmFja2Ryb3BDbGFzcyk7XG4gICAgICAgICAgcmV0dXJuIGxpbmtGbjtcbiAgICAgICAgfVxuICAgICAgfTtcblxuICAgICAgZnVuY3Rpb24gbGlua0ZuKHNjb3BlLCBlbGVtZW50LCBhdHRycykge1xuICAgICAgICBpZiAoISRtb2RhbFN1cHByZXNzV2FybmluZykge1xuICAgICAgICAgICRsb2cud2FybignbW9kYWwtYmFja2Ryb3AgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSB1aWItbW9kYWwtYmFja2Ryb3AgaW5zdGVhZC4nKTtcbiAgICAgICAgfVxuICAgICAgICBlbGVtZW50LmFkZENsYXNzKCdtb2RhbC1iYWNrZHJvcCcpO1xuXG4gICAgICAgIGlmIChhdHRycy5tb2RhbEluQ2xhc3MpIHtcbiAgICAgICAgICBpZiAoJGFuaW1hdGVDc3MpIHtcbiAgICAgICAgICAgICRhbmltYXRlQ3NzKGVsZW1lbnQsIHtcbiAgICAgICAgICAgICAgYWRkQ2xhc3M6IGF0dHJzLm1vZGFsSW5DbGFzc1xuICAgICAgICAgICAgfSkuc3RhcnQoKTtcbiAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgJGFuaW1hdGUuYWRkQ2xhc3MoZWxlbWVudCwgYXR0cnMubW9kYWxJbkNsYXNzKTtcbiAgICAgICAgICB9XG5cbiAgICAgICAgICBzY29wZS4kb24oJG1vZGFsU3RhY2suTk9XX0NMT1NJTkdfRVZFTlQsIGZ1bmN0aW9uKGUsIHNldElzQXN5bmMpIHtcbiAgICAgICAgICAgIHZhciBkb25lID0gc2V0SXNBc3luYygpO1xuICAgICAgICAgICAgaWYgKCRhbmltYXRlQ3NzKSB7XG4gICAgICAgICAgICAgICRhbmltYXRlQ3NzKGVsZW1lbnQsIHtcbiAgICAgICAgICAgICAgICByZW1vdmVDbGFzczogYXR0cnMubW9kYWxJbkNsYXNzXG4gICAgICAgICAgICAgIH0pLnN0YXJ0KCkudGhlbihkb25lKTtcbiAgICAgICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgICAgICRhbmltYXRlLnJlbW92ZUNsYXNzKGVsZW1lbnQsIGF0dHJzLm1vZGFsSW5DbGFzcykudGhlbihkb25lKTtcbiAgICAgICAgICAgIH1cbiAgICAgICAgICB9KTtcbiAgICAgICAgfVxuICAgICAgfVxuICAgIH1dKVxuXG4gIC5kaXJlY3RpdmUoJ21vZGFsV2luZG93JywgW1xuICAgICckbW9kYWxTdGFjaycsICckcScsICckYW5pbWF0ZScsICckaW5qZWN0b3InLCAnJGxvZycsICckbW9kYWxTdXBwcmVzc1dhcm5pbmcnLFxuICAgIGZ1bmN0aW9uKCRtb2RhbFN0YWNrICwgICRxICwgICRhbmltYXRlLCAgICRpbmplY3RvciwgJGxvZywgJG1vZGFsU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICB2YXIgJGFuaW1hdGVDc3MgPSBudWxsO1xuXG4gICAgICBpZiAoJGluamVjdG9yLmhhcygnJGFuaW1hdGVDc3MnKSkge1xuICAgICAgICAkYW5pbWF0ZUNzcyA9ICRpbmplY3Rvci5nZXQoJyRhbmltYXRlQ3NzJyk7XG4gICAgICB9XG5cbiAgICAgIHJldHVybiB7XG4gICAgICAgIHNjb3BlOiB7XG4gICAgICAgICAgaW5kZXg6ICdAJ1xuICAgICAgICB9LFxuICAgICAgICByZXBsYWNlOiB0cnVlLFxuICAgICAgICB0cmFuc2NsdWRlOiB0cnVlLFxuICAgICAgICB0ZW1wbGF0ZVVybDogZnVuY3Rpb24odEVsZW1lbnQsIHRBdHRycykge1xuICAgICAgICAgIHJldHVybiB0QXR0cnMudGVtcGxhdGVVcmwgfHwgJ3RlbXBsYXRlL21vZGFsL3dpbmRvdy5odG1sJztcbiAgICAgICAgfSxcbiAgICAgICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHJzKSB7XG4gICAgICAgICAgaWYgKCEkbW9kYWxTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgICAgICRsb2cud2FybignbW9kYWwtd2luZG93IGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliLW1vZGFsLXdpbmRvdyBpbnN0ZWFkLicpO1xuICAgICAgICAgIH1cbiAgICAgICAgICBlbGVtZW50LmFkZENsYXNzKGF0dHJzLndpbmRvd0NsYXNzIHx8ICcnKTtcbiAgICAgICAgICBlbGVtZW50LmFkZENsYXNzKGF0dHJzLndpbmRvd1RvcENsYXNzIHx8ICcnKTtcbiAgICAgICAgICBzY29wZS5zaXplID0gYXR0cnMuc2l6ZTtcblxuICAgICAgICAgIHNjb3BlLmNsb3NlID0gZnVuY3Rpb24oZXZ0KSB7XG4gICAgICAgICAgICB2YXIgbW9kYWwgPSAkbW9kYWxTdGFjay5nZXRUb3AoKTtcbiAgICAgICAgICAgIGlmIChtb2RhbCAmJiBtb2RhbC52YWx1ZS5iYWNrZHJvcCAmJiBtb2RhbC52YWx1ZS5iYWNrZHJvcCAhPT0gJ3N0YXRpYycgJiYgKGV2dC50YXJnZXQgPT09IGV2dC5jdXJyZW50VGFyZ2V0KSkge1xuICAgICAgICAgICAgICBldnQucHJldmVudERlZmF1bHQoKTtcbiAgICAgICAgICAgICAgZXZ0LnN0b3BQcm9wYWdhdGlvbigpO1xuICAgICAgICAgICAgICAkbW9kYWxTdGFjay5kaXNtaXNzKG1vZGFsLmtleSwgJ2JhY2tkcm9wIGNsaWNrJyk7XG4gICAgICAgICAgICB9XG4gICAgICAgICAgfTtcblxuICAgICAgICAgIC8vIG1vdmVkIGZyb20gdGVtcGxhdGUgdG8gZml4IGlzc3VlICMyMjgwXG4gICAgICAgICAgZWxlbWVudC5vbignY2xpY2snLCBzY29wZS5jbG9zZSk7XG5cbiAgICAgICAgICAvLyBUaGlzIHByb3BlcnR5IGlzIG9ubHkgYWRkZWQgdG8gdGhlIHNjb3BlIGZvciB0aGUgcHVycG9zZSBvZiBkZXRlY3Rpbmcgd2hlbiB0aGlzIGRpcmVjdGl2ZSBpcyByZW5kZXJlZC5cbiAgICAgICAgICAvLyBXZSBjYW4gZGV0ZWN0IHRoYXQgYnkgdXNpbmcgdGhpcyBwcm9wZXJ0eSBpbiB0aGUgdGVtcGxhdGUgYXNzb2NpYXRlZCB3aXRoIHRoaXMgZGlyZWN0aXZlIGFuZCB0aGVuIHVzZVxuICAgICAgICAgIC8vIHtAbGluayBBdHRyaWJ1dGUjJG9ic2VydmV9IG9uIGl0LiBGb3IgbW9yZSBkZXRhaWxzIHBsZWFzZSBzZWUge0BsaW5rIFRhYmxlQ29sdW1uUmVzaXplfS5cbiAgICAgICAgICBzY29wZS4kaXNSZW5kZXJlZCA9IHRydWU7XG5cbiAgICAgICAgICAvLyBEZWZlcnJlZCBvYmplY3QgdGhhdCB3aWxsIGJlIHJlc29sdmVkIHdoZW4gdGhpcyBtb2RhbCBpcyByZW5kZXIuXG4gICAgICAgICAgdmFyIG1vZGFsUmVuZGVyRGVmZXJPYmogPSAkcS5kZWZlcigpO1xuICAgICAgICAgIC8vIE9ic2VydmUgZnVuY3Rpb24gd2lsbCBiZSBjYWxsZWQgb24gbmV4dCBkaWdlc3QgY3ljbGUgYWZ0ZXIgY29tcGlsYXRpb24sIGVuc3VyaW5nIHRoYXQgdGhlIERPTSBpcyByZWFkeS5cbiAgICAgICAgICAvLyBJbiBvcmRlciB0byB1c2UgdGhpcyB3YXkgb2YgZmluZGluZyB3aGV0aGVyIERPTSBpcyByZWFkeSwgd2UgbmVlZCB0byBvYnNlcnZlIGEgc2NvcGUgcHJvcGVydHkgdXNlZCBpbiBtb2RhbCdzIHRlbXBsYXRlLlxuICAgICAgICAgIGF0dHJzLiRvYnNlcnZlKCdtb2RhbFJlbmRlcicsIGZ1bmN0aW9uKHZhbHVlKSB7XG4gICAgICAgICAgICBpZiAodmFsdWUgPT0gJ3RydWUnKSB7XG4gICAgICAgICAgICAgIG1vZGFsUmVuZGVyRGVmZXJPYmoucmVzb2x2ZSgpO1xuICAgICAgICAgICAgfVxuICAgICAgICAgIH0pO1xuXG4gICAgICAgICAgbW9kYWxSZW5kZXJEZWZlck9iai5wcm9taXNlLnRoZW4oZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICB2YXIgYW5pbWF0aW9uUHJvbWlzZSA9IG51bGw7XG5cbiAgICAgICAgICAgIGlmIChhdHRycy5tb2RhbEluQ2xhc3MpIHtcbiAgICAgICAgICAgICAgaWYgKCRhbmltYXRlQ3NzKSB7XG4gICAgICAgICAgICAgICAgYW5pbWF0aW9uUHJvbWlzZSA9ICRhbmltYXRlQ3NzKGVsZW1lbnQsIHtcbiAgICAgICAgICAgICAgICAgIGFkZENsYXNzOiBhdHRycy5tb2RhbEluQ2xhc3NcbiAgICAgICAgICAgICAgICB9KS5zdGFydCgpO1xuICAgICAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgICAgIGFuaW1hdGlvblByb21pc2UgPSAkYW5pbWF0ZS5hZGRDbGFzcyhlbGVtZW50LCBhdHRycy5tb2RhbEluQ2xhc3MpO1xuICAgICAgICAgICAgICB9XG5cbiAgICAgICAgICAgICAgc2NvcGUuJG9uKCRtb2RhbFN0YWNrLk5PV19DTE9TSU5HX0VWRU5ULCBmdW5jdGlvbihlLCBzZXRJc0FzeW5jKSB7XG4gICAgICAgICAgICAgICAgdmFyIGRvbmUgPSBzZXRJc0FzeW5jKCk7XG4gICAgICAgICAgICAgICAgaWYgKCRhbmltYXRlQ3NzKSB7XG4gICAgICAgICAgICAgICAgICAkYW5pbWF0ZUNzcyhlbGVtZW50LCB7XG4gICAgICAgICAgICAgICAgICAgIHJlbW92ZUNsYXNzOiBhdHRycy5tb2RhbEluQ2xhc3NcbiAgICAgICAgICAgICAgICAgIH0pLnN0YXJ0KCkudGhlbihkb25lKTtcbiAgICAgICAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgICAgICAgJGFuaW1hdGUucmVtb3ZlQ2xhc3MoZWxlbWVudCwgYXR0cnMubW9kYWxJbkNsYXNzKS50aGVuKGRvbmUpO1xuICAgICAgICAgICAgICAgIH1cbiAgICAgICAgICAgICAgfSk7XG4gICAgICAgICAgICB9XG5cblxuICAgICAgICAgICAgJHEud2hlbihhbmltYXRpb25Qcm9taXNlKS50aGVuKGZ1bmN0aW9uKCkge1xuICAgICAgICAgICAgICB2YXIgaW5wdXRXaXRoQXV0b2ZvY3VzID0gZWxlbWVudFswXS5xdWVyeVNlbGVjdG9yKCdbYXV0b2ZvY3VzXScpO1xuICAgICAgICAgICAgICAvKipcbiAgICAgICAgICAgICAgICogQXV0by1mb2N1c2luZyBvZiBhIGZyZXNobHktb3BlbmVkIG1vZGFsIGVsZW1lbnQgY2F1c2VzIGFueSBjaGlsZCBlbGVtZW50c1xuICAgICAgICAgICAgICAgKiB3aXRoIHRoZSBhdXRvZm9jdXMgYXR0cmlidXRlIHRvIGxvc2UgZm9jdXMuIFRoaXMgaXMgYW4gaXNzdWUgb24gdG91Y2hcbiAgICAgICAgICAgICAgICogYmFzZWQgZGV2aWNlcyB3aGljaCB3aWxsIHNob3cgYW5kIHRoZW4gaGlkZSB0aGUgb25zY3JlZW4ga2V5Ym9hcmQuXG4gICAgICAgICAgICAgICAqIEF0dGVtcHRzIHRvIHJlZm9jdXMgdGhlIGF1dG9mb2N1cyBlbGVtZW50IHZpYSBKYXZhU2NyaXB0IHdpbGwgbm90IHJlb3BlblxuICAgICAgICAgICAgICAgKiB0aGUgb25zY3JlZW4ga2V5Ym9hcmQuIEZpeGVkIGJ5IHVwZGF0ZWQgdGhlIGZvY3VzaW5nIGxvZ2ljIHRvIG9ubHkgYXV0b2ZvY3VzXG4gICAgICAgICAgICAgICAqIHRoZSBtb2RhbCBlbGVtZW50IGlmIHRoZSBtb2RhbCBkb2VzIG5vdCBjb250YWluIGFuIGF1dG9mb2N1cyBlbGVtZW50LlxuICAgICAgICAgICAgICAgKi9cbiAgICAgICAgICAgICAgaWYgKGlucHV0V2l0aEF1dG9mb2N1cykge1xuICAgICAgICAgICAgICAgIGlucHV0V2l0aEF1dG9mb2N1cy5mb2N1cygpO1xuICAgICAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgICAgIGVsZW1lbnRbMF0uZm9jdXMoKTtcbiAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgfSk7XG5cbiAgICAgICAgICAgIC8vIE5vdGlmeSB7QGxpbmsgJG1vZGFsU3RhY2t9IHRoYXQgbW9kYWwgaXMgcmVuZGVyZWQuXG4gICAgICAgICAgICB2YXIgbW9kYWwgPSAkbW9kYWxTdGFjay5nZXRUb3AoKTtcbiAgICAgICAgICAgIGlmIChtb2RhbCkge1xuICAgICAgICAgICAgICAkbW9kYWxTdGFjay5tb2RhbFJlbmRlcmVkKG1vZGFsLmtleSk7XG4gICAgICAgICAgICB9XG4gICAgICAgICAgfSk7XG4gICAgICAgIH1cbiAgICAgIH07XG4gICAgfV0pXG5cbiAgLmRpcmVjdGl2ZSgnbW9kYWxBbmltYXRpb25DbGFzcycsIFtcbiAgICAnJGxvZycsICckbW9kYWxTdXBwcmVzc1dhcm5pbmcnLFxuICAgIGZ1bmN0aW9uICgkbG9nLCAkbW9kYWxTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgIHJldHVybiB7XG4gICAgICAgIGNvbXBpbGU6IGZ1bmN0aW9uKHRFbGVtZW50LCB0QXR0cnMpIHtcbiAgICAgICAgICBpZiAoISRtb2RhbFN1cHByZXNzV2FybmluZykge1xuICAgICAgICAgICAgJGxvZy53YXJuKCdtb2RhbC1hbmltYXRpb24tY2xhc3MgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSB1aWItbW9kYWwtYW5pbWF0aW9uLWNsYXNzIGluc3RlYWQuJyk7XG4gICAgICAgICAgfVxuICAgICAgICAgIGlmICh0QXR0cnMubW9kYWxBbmltYXRpb24pIHtcbiAgICAgICAgICAgIHRFbGVtZW50LmFkZENsYXNzKHRBdHRycy5tb2RhbEFuaW1hdGlvbkNsYXNzKTtcbiAgICAgICAgICB9XG4gICAgICAgIH1cbiAgICAgIH07XG4gICAgfV0pXG5cbiAgLmRpcmVjdGl2ZSgnbW9kYWxUcmFuc2NsdWRlJywgW1xuICAgICckbG9nJywgJyRtb2RhbFN1cHByZXNzV2FybmluZycsXG4gICAgZnVuY3Rpb24gKCRsb2csICRtb2RhbFN1cHByZXNzV2FybmluZykge1xuICAgIHJldHVybiB7XG4gICAgICBsaW5rOiBmdW5jdGlvbigkc2NvcGUsICRlbGVtZW50LCAkYXR0cnMsIGNvbnRyb2xsZXIsICR0cmFuc2NsdWRlKSB7XG4gICAgICAgIGlmICghJG1vZGFsU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICAgJGxvZy53YXJuKCdtb2RhbC10cmFuc2NsdWRlIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliLW1vZGFsLXRyYW5zY2x1ZGUgaW5zdGVhZC4nKTtcbiAgICAgICAgfVxuICAgICAgICAkdHJhbnNjbHVkZSgkc2NvcGUuJHBhcmVudCwgZnVuY3Rpb24oY2xvbmUpIHtcbiAgICAgICAgICAkZWxlbWVudC5lbXB0eSgpO1xuICAgICAgICAgICRlbGVtZW50LmFwcGVuZChjbG9uZSk7XG4gICAgICAgIH0pO1xuICAgICAgfVxuICAgIH07XG4gIH1dKVxuXG4gIC5zZXJ2aWNlKCckbW9kYWxTdGFjaycsIFtcbiAgICAnJGFuaW1hdGUnLCAnJHRpbWVvdXQnLCAnJGRvY3VtZW50JywgJyRjb21waWxlJywgJyRyb290U2NvcGUnLFxuICAgICckcScsXG4gICAgJyRpbmplY3RvcicsXG4gICAgJyQkbXVsdGlNYXAnLFxuICAgICckJHN0YWNrZWRNYXAnLFxuICAgICckdWliTW9kYWxTdGFjaycsXG4gICAgJyRsb2cnLFxuICAgICckbW9kYWxTdXBwcmVzc1dhcm5pbmcnLFxuICAgIGZ1bmN0aW9uKCRhbmltYXRlICwgICR0aW1lb3V0ICwgICRkb2N1bWVudCAsICAkY29tcGlsZSAsICAkcm9vdFNjb3BlICxcbiAgICAgICAgICAgICAkcSxcbiAgICAgICAgICAgICAkaW5qZWN0b3IsXG4gICAgICAgICAgICAgJCRtdWx0aU1hcCxcbiAgICAgICAgICAgICAkJHN0YWNrZWRNYXAsXG4gICAgICAgICAgICAgJHVpYk1vZGFsU3RhY2ssXG4gICAgICAgICAgICAgJGxvZyxcbiAgICAgICAgICAgICAkbW9kYWxTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgIGlmICghJG1vZGFsU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICRsb2cud2FybignJG1vZGFsU3RhY2sgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSAkdWliTW9kYWxTdGFjayBpbnN0ZWFkLicpO1xuICAgICAgfVxuXG4gICAgICBhbmd1bGFyLmV4dGVuZCh0aGlzLCAkdWliTW9kYWxTdGFjayk7XG4gICAgfV0pXG5cbiAgLnByb3ZpZGVyKCckbW9kYWwnLCBbJyR1aWJNb2RhbFByb3ZpZGVyJywgZnVuY3Rpb24oJHVpYk1vZGFsUHJvdmlkZXIpIHtcbiAgICBhbmd1bGFyLmV4dGVuZCh0aGlzLCAkdWliTW9kYWxQcm92aWRlcik7XG5cbiAgICB0aGlzLiRnZXQgPSBbJyRpbmplY3RvcicsICckbG9nJywgJyRtb2RhbFN1cHByZXNzV2FybmluZycsXG4gICAgICBmdW5jdGlvbiAoJGluamVjdG9yLCAkbG9nLCAkbW9kYWxTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgaWYgKCEkbW9kYWxTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgICAkbG9nLndhcm4oJyRtb2RhbCBpcyBub3cgZGVwcmVjYXRlZC4gVXNlICR1aWJNb2RhbCBpbnN0ZWFkLicpO1xuICAgICAgICB9XG5cbiAgICAgICAgcmV0dXJuICRpbmplY3Rvci5pbnZva2UoJHVpYk1vZGFsUHJvdmlkZXIuJGdldCk7XG4gICAgICB9XTtcbiAgfV0pO1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLnBhZ2luYXRpb24nLCBbXSlcbi5jb250cm9sbGVyKCdVaWJQYWdpbmF0aW9uQ29udHJvbGxlcicsIFsnJHNjb3BlJywgJyRhdHRycycsICckcGFyc2UnLCBmdW5jdGlvbigkc2NvcGUsICRhdHRycywgJHBhcnNlKSB7XG4gIHZhciBzZWxmID0gdGhpcyxcbiAgICAgIG5nTW9kZWxDdHJsID0geyAkc2V0Vmlld1ZhbHVlOiBhbmd1bGFyLm5vb3AgfSwgLy8gbnVsbE1vZGVsQ3RybFxuICAgICAgc2V0TnVtUGFnZXMgPSAkYXR0cnMubnVtUGFnZXMgPyAkcGFyc2UoJGF0dHJzLm51bVBhZ2VzKS5hc3NpZ24gOiBhbmd1bGFyLm5vb3A7XG5cbiAgdGhpcy5pbml0ID0gZnVuY3Rpb24obmdNb2RlbEN0cmxfLCBjb25maWcpIHtcbiAgICBuZ01vZGVsQ3RybCA9IG5nTW9kZWxDdHJsXztcbiAgICB0aGlzLmNvbmZpZyA9IGNvbmZpZztcblxuICAgIG5nTW9kZWxDdHJsLiRyZW5kZXIgPSBmdW5jdGlvbigpIHtcbiAgICAgIHNlbGYucmVuZGVyKCk7XG4gICAgfTtcblxuICAgIGlmICgkYXR0cnMuaXRlbXNQZXJQYWdlKSB7XG4gICAgICAkc2NvcGUuJHBhcmVudC4kd2F0Y2goJHBhcnNlKCRhdHRycy5pdGVtc1BlclBhZ2UpLCBmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgICBzZWxmLml0ZW1zUGVyUGFnZSA9IHBhcnNlSW50KHZhbHVlLCAxMCk7XG4gICAgICAgICRzY29wZS50b3RhbFBhZ2VzID0gc2VsZi5jYWxjdWxhdGVUb3RhbFBhZ2VzKCk7XG4gICAgICB9KTtcbiAgICB9IGVsc2Uge1xuICAgICAgdGhpcy5pdGVtc1BlclBhZ2UgPSBjb25maWcuaXRlbXNQZXJQYWdlO1xuICAgIH1cblxuICAgICRzY29wZS4kd2F0Y2goJ3RvdGFsSXRlbXMnLCBmdW5jdGlvbigpIHtcbiAgICAgICRzY29wZS50b3RhbFBhZ2VzID0gc2VsZi5jYWxjdWxhdGVUb3RhbFBhZ2VzKCk7XG4gICAgfSk7XG5cbiAgICAkc2NvcGUuJHdhdGNoKCd0b3RhbFBhZ2VzJywgZnVuY3Rpb24odmFsdWUpIHtcbiAgICAgIHNldE51bVBhZ2VzKCRzY29wZS4kcGFyZW50LCB2YWx1ZSk7IC8vIFJlYWRvbmx5IHZhcmlhYmxlXG5cbiAgICAgIGlmICggJHNjb3BlLnBhZ2UgPiB2YWx1ZSApIHtcbiAgICAgICAgJHNjb3BlLnNlbGVjdFBhZ2UodmFsdWUpO1xuICAgICAgfSBlbHNlIHtcbiAgICAgICAgbmdNb2RlbEN0cmwuJHJlbmRlcigpO1xuICAgICAgfVxuICAgIH0pO1xuICB9O1xuXG4gIHRoaXMuY2FsY3VsYXRlVG90YWxQYWdlcyA9IGZ1bmN0aW9uKCkge1xuICAgIHZhciB0b3RhbFBhZ2VzID0gdGhpcy5pdGVtc1BlclBhZ2UgPCAxID8gMSA6IE1hdGguY2VpbCgkc2NvcGUudG90YWxJdGVtcyAvIHRoaXMuaXRlbXNQZXJQYWdlKTtcbiAgICByZXR1cm4gTWF0aC5tYXgodG90YWxQYWdlcyB8fCAwLCAxKTtcbiAgfTtcblxuICB0aGlzLnJlbmRlciA9IGZ1bmN0aW9uKCkge1xuICAgICRzY29wZS5wYWdlID0gcGFyc2VJbnQobmdNb2RlbEN0cmwuJHZpZXdWYWx1ZSwgMTApIHx8IDE7XG4gIH07XG5cbiAgJHNjb3BlLnNlbGVjdFBhZ2UgPSBmdW5jdGlvbihwYWdlLCBldnQpIHtcbiAgICBpZiAoZXZ0KSB7XG4gICAgICBldnQucHJldmVudERlZmF1bHQoKTtcbiAgICB9XG5cbiAgICB2YXIgY2xpY2tBbGxvd2VkID0gISRzY29wZS5uZ0Rpc2FibGVkIHx8ICFldnQ7XG4gICAgaWYgKGNsaWNrQWxsb3dlZCAmJiAkc2NvcGUucGFnZSAhPT0gcGFnZSAmJiBwYWdlID4gMCAmJiBwYWdlIDw9ICRzY29wZS50b3RhbFBhZ2VzKSB7XG4gICAgICBpZiAoZXZ0ICYmIGV2dC50YXJnZXQpIHtcbiAgICAgICAgZXZ0LnRhcmdldC5ibHVyKCk7XG4gICAgICB9XG4gICAgICBuZ01vZGVsQ3RybC4kc2V0Vmlld1ZhbHVlKHBhZ2UpO1xuICAgICAgbmdNb2RlbEN0cmwuJHJlbmRlcigpO1xuICAgIH1cbiAgfTtcblxuICAkc2NvcGUuZ2V0VGV4dCA9IGZ1bmN0aW9uKGtleSkge1xuICAgIHJldHVybiAkc2NvcGVba2V5ICsgJ1RleHQnXSB8fCBzZWxmLmNvbmZpZ1trZXkgKyAnVGV4dCddO1xuICB9O1xuXG4gICRzY29wZS5ub1ByZXZpb3VzID0gZnVuY3Rpb24oKSB7XG4gICAgcmV0dXJuICRzY29wZS5wYWdlID09PSAxO1xuICB9O1xuXG4gICRzY29wZS5ub05leHQgPSBmdW5jdGlvbigpIHtcbiAgICByZXR1cm4gJHNjb3BlLnBhZ2UgPT09ICRzY29wZS50b3RhbFBhZ2VzO1xuICB9O1xufV0pXG5cbi5jb25zdGFudCgndWliUGFnaW5hdGlvbkNvbmZpZycsIHtcbiAgaXRlbXNQZXJQYWdlOiAxMCxcbiAgYm91bmRhcnlMaW5rczogZmFsc2UsXG4gIGRpcmVjdGlvbkxpbmtzOiB0cnVlLFxuICBmaXJzdFRleHQ6ICdGaXJzdCcsXG4gIHByZXZpb3VzVGV4dDogJ1ByZXZpb3VzJyxcbiAgbmV4dFRleHQ6ICdOZXh0JyxcbiAgbGFzdFRleHQ6ICdMYXN0JyxcbiAgcm90YXRlOiB0cnVlXG59KVxuXG4uZGlyZWN0aXZlKCd1aWJQYWdpbmF0aW9uJywgWyckcGFyc2UnLCAndWliUGFnaW5hdGlvbkNvbmZpZycsIGZ1bmN0aW9uKCRwYXJzZSwgcGFnaW5hdGlvbkNvbmZpZykge1xuICByZXR1cm4ge1xuICAgIHJlc3RyaWN0OiAnRUEnLFxuICAgIHNjb3BlOiB7XG4gICAgICB0b3RhbEl0ZW1zOiAnPScsXG4gICAgICBmaXJzdFRleHQ6ICdAJyxcbiAgICAgIHByZXZpb3VzVGV4dDogJ0AnLFxuICAgICAgbmV4dFRleHQ6ICdAJyxcbiAgICAgIGxhc3RUZXh0OiAnQCcsXG4gICAgICBuZ0Rpc2FibGVkOic9J1xuICAgIH0sXG4gICAgcmVxdWlyZTogWyd1aWJQYWdpbmF0aW9uJywgJz9uZ01vZGVsJ10sXG4gICAgY29udHJvbGxlcjogJ1VpYlBhZ2luYXRpb25Db250cm9sbGVyJyxcbiAgICBjb250cm9sbGVyQXM6ICdwYWdpbmF0aW9uJyxcbiAgICB0ZW1wbGF0ZVVybDogZnVuY3Rpb24oZWxlbWVudCwgYXR0cnMpIHtcbiAgICAgIHJldHVybiBhdHRycy50ZW1wbGF0ZVVybCB8fCAndGVtcGxhdGUvcGFnaW5hdGlvbi9wYWdpbmF0aW9uLmh0bWwnO1xuICAgIH0sXG4gICAgcmVwbGFjZTogdHJ1ZSxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMsIGN0cmxzKSB7XG4gICAgICB2YXIgcGFnaW5hdGlvbkN0cmwgPSBjdHJsc1swXSwgbmdNb2RlbEN0cmwgPSBjdHJsc1sxXTtcblxuICAgICAgaWYgKCFuZ01vZGVsQ3RybCkge1xuICAgICAgICAgcmV0dXJuOyAvLyBkbyBub3RoaW5nIGlmIG5vIG5nLW1vZGVsXG4gICAgICB9XG5cbiAgICAgIC8vIFNldHVwIGNvbmZpZ3VyYXRpb24gcGFyYW1ldGVyc1xuICAgICAgdmFyIG1heFNpemUgPSBhbmd1bGFyLmlzRGVmaW5lZChhdHRycy5tYXhTaXplKSA/IHNjb3BlLiRwYXJlbnQuJGV2YWwoYXR0cnMubWF4U2l6ZSkgOiBwYWdpbmF0aW9uQ29uZmlnLm1heFNpemUsXG4gICAgICAgICAgcm90YXRlID0gYW5ndWxhci5pc0RlZmluZWQoYXR0cnMucm90YXRlKSA/IHNjb3BlLiRwYXJlbnQuJGV2YWwoYXR0cnMucm90YXRlKSA6IHBhZ2luYXRpb25Db25maWcucm90YXRlO1xuICAgICAgc2NvcGUuYm91bmRhcnlMaW5rcyA9IGFuZ3VsYXIuaXNEZWZpbmVkKGF0dHJzLmJvdW5kYXJ5TGlua3MpID8gc2NvcGUuJHBhcmVudC4kZXZhbChhdHRycy5ib3VuZGFyeUxpbmtzKSA6IHBhZ2luYXRpb25Db25maWcuYm91bmRhcnlMaW5rcztcbiAgICAgIHNjb3BlLmRpcmVjdGlvbkxpbmtzID0gYW5ndWxhci5pc0RlZmluZWQoYXR0cnMuZGlyZWN0aW9uTGlua3MpID8gc2NvcGUuJHBhcmVudC4kZXZhbChhdHRycy5kaXJlY3Rpb25MaW5rcykgOiBwYWdpbmF0aW9uQ29uZmlnLmRpcmVjdGlvbkxpbmtzO1xuXG4gICAgICBwYWdpbmF0aW9uQ3RybC5pbml0KG5nTW9kZWxDdHJsLCBwYWdpbmF0aW9uQ29uZmlnKTtcblxuICAgICAgaWYgKGF0dHJzLm1heFNpemUpIHtcbiAgICAgICAgc2NvcGUuJHBhcmVudC4kd2F0Y2goJHBhcnNlKGF0dHJzLm1heFNpemUpLCBmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgICAgIG1heFNpemUgPSBwYXJzZUludCh2YWx1ZSwgMTApO1xuICAgICAgICAgIHBhZ2luYXRpb25DdHJsLnJlbmRlcigpO1xuICAgICAgICB9KTtcbiAgICAgIH1cblxuICAgICAgLy8gQ3JlYXRlIHBhZ2Ugb2JqZWN0IHVzZWQgaW4gdGVtcGxhdGVcbiAgICAgIGZ1bmN0aW9uIG1ha2VQYWdlKG51bWJlciwgdGV4dCwgaXNBY3RpdmUpIHtcbiAgICAgICAgcmV0dXJuIHtcbiAgICAgICAgICBudW1iZXI6IG51bWJlcixcbiAgICAgICAgICB0ZXh0OiB0ZXh0LFxuICAgICAgICAgIGFjdGl2ZTogaXNBY3RpdmVcbiAgICAgICAgfTtcbiAgICAgIH1cblxuICAgICAgZnVuY3Rpb24gZ2V0UGFnZXMoY3VycmVudFBhZ2UsIHRvdGFsUGFnZXMpIHtcbiAgICAgICAgdmFyIHBhZ2VzID0gW107XG5cbiAgICAgICAgLy8gRGVmYXVsdCBwYWdlIGxpbWl0c1xuICAgICAgICB2YXIgc3RhcnRQYWdlID0gMSwgZW5kUGFnZSA9IHRvdGFsUGFnZXM7XG4gICAgICAgIHZhciBpc01heFNpemVkID0gYW5ndWxhci5pc0RlZmluZWQobWF4U2l6ZSkgJiYgbWF4U2l6ZSA8IHRvdGFsUGFnZXM7XG5cbiAgICAgICAgLy8gcmVjb21wdXRlIGlmIG1heFNpemVcbiAgICAgICAgaWYgKGlzTWF4U2l6ZWQpIHtcbiAgICAgICAgICBpZiAocm90YXRlKSB7XG4gICAgICAgICAgICAvLyBDdXJyZW50IHBhZ2UgaXMgZGlzcGxheWVkIGluIHRoZSBtaWRkbGUgb2YgdGhlIHZpc2libGUgb25lc1xuICAgICAgICAgICAgc3RhcnRQYWdlID0gTWF0aC5tYXgoY3VycmVudFBhZ2UgLSBNYXRoLmZsb29yKG1heFNpemUvMiksIDEpO1xuICAgICAgICAgICAgZW5kUGFnZSAgID0gc3RhcnRQYWdlICsgbWF4U2l6ZSAtIDE7XG5cbiAgICAgICAgICAgIC8vIEFkanVzdCBpZiBsaW1pdCBpcyBleGNlZWRlZFxuICAgICAgICAgICAgaWYgKGVuZFBhZ2UgPiB0b3RhbFBhZ2VzKSB7XG4gICAgICAgICAgICAgIGVuZFBhZ2UgICA9IHRvdGFsUGFnZXM7XG4gICAgICAgICAgICAgIHN0YXJ0UGFnZSA9IGVuZFBhZ2UgLSBtYXhTaXplICsgMTtcbiAgICAgICAgICAgIH1cbiAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgLy8gVmlzaWJsZSBwYWdlcyBhcmUgcGFnaW5hdGVkIHdpdGggbWF4U2l6ZVxuICAgICAgICAgICAgc3RhcnRQYWdlID0gKChNYXRoLmNlaWwoY3VycmVudFBhZ2UgLyBtYXhTaXplKSAtIDEpICogbWF4U2l6ZSkgKyAxO1xuXG4gICAgICAgICAgICAvLyBBZGp1c3QgbGFzdCBwYWdlIGlmIGxpbWl0IGlzIGV4Y2VlZGVkXG4gICAgICAgICAgICBlbmRQYWdlID0gTWF0aC5taW4oc3RhcnRQYWdlICsgbWF4U2l6ZSAtIDEsIHRvdGFsUGFnZXMpO1xuICAgICAgICAgIH1cbiAgICAgICAgfVxuXG4gICAgICAgIC8vIEFkZCBwYWdlIG51bWJlciBsaW5rc1xuICAgICAgICBmb3IgKHZhciBudW1iZXIgPSBzdGFydFBhZ2U7IG51bWJlciA8PSBlbmRQYWdlOyBudW1iZXIrKykge1xuICAgICAgICAgIHZhciBwYWdlID0gbWFrZVBhZ2UobnVtYmVyLCBudW1iZXIsIG51bWJlciA9PT0gY3VycmVudFBhZ2UpO1xuICAgICAgICAgIHBhZ2VzLnB1c2gocGFnZSk7XG4gICAgICAgIH1cblxuICAgICAgICAvLyBBZGQgbGlua3MgdG8gbW92ZSBiZXR3ZWVuIHBhZ2Ugc2V0c1xuICAgICAgICBpZiAoaXNNYXhTaXplZCAmJiAhIHJvdGF0ZSkge1xuICAgICAgICAgIGlmIChzdGFydFBhZ2UgPiAxKSB7XG4gICAgICAgICAgICB2YXIgcHJldmlvdXNQYWdlU2V0ID0gbWFrZVBhZ2Uoc3RhcnRQYWdlIC0gMSwgJy4uLicsIGZhbHNlKTtcbiAgICAgICAgICAgIHBhZ2VzLnVuc2hpZnQocHJldmlvdXNQYWdlU2V0KTtcbiAgICAgICAgICB9XG5cbiAgICAgICAgICBpZiAoZW5kUGFnZSA8IHRvdGFsUGFnZXMpIHtcbiAgICAgICAgICAgIHZhciBuZXh0UGFnZVNldCA9IG1ha2VQYWdlKGVuZFBhZ2UgKyAxLCAnLi4uJywgZmFsc2UpO1xuICAgICAgICAgICAgcGFnZXMucHVzaChuZXh0UGFnZVNldCk7XG4gICAgICAgICAgfVxuICAgICAgICB9XG5cbiAgICAgICAgcmV0dXJuIHBhZ2VzO1xuICAgICAgfVxuXG4gICAgICB2YXIgb3JpZ2luYWxSZW5kZXIgPSBwYWdpbmF0aW9uQ3RybC5yZW5kZXI7XG4gICAgICBwYWdpbmF0aW9uQ3RybC5yZW5kZXIgPSBmdW5jdGlvbigpIHtcbiAgICAgICAgb3JpZ2luYWxSZW5kZXIoKTtcbiAgICAgICAgaWYgKHNjb3BlLnBhZ2UgPiAwICYmIHNjb3BlLnBhZ2UgPD0gc2NvcGUudG90YWxQYWdlcykge1xuICAgICAgICAgIHNjb3BlLnBhZ2VzID0gZ2V0UGFnZXMoc2NvcGUucGFnZSwgc2NvcGUudG90YWxQYWdlcyk7XG4gICAgICAgIH1cbiAgICAgIH07XG4gICAgfVxuICB9O1xufV0pXG5cbi5jb25zdGFudCgndWliUGFnZXJDb25maWcnLCB7XG4gIGl0ZW1zUGVyUGFnZTogMTAsXG4gIHByZXZpb3VzVGV4dDogJ8KrIFByZXZpb3VzJyxcbiAgbmV4dFRleHQ6ICdOZXh0IMK7JyxcbiAgYWxpZ246IHRydWVcbn0pXG5cbi5kaXJlY3RpdmUoJ3VpYlBhZ2VyJywgWyd1aWJQYWdlckNvbmZpZycsIGZ1bmN0aW9uKHBhZ2VyQ29uZmlnKSB7XG4gIHJldHVybiB7XG4gICAgcmVzdHJpY3Q6ICdFQScsXG4gICAgc2NvcGU6IHtcbiAgICAgIHRvdGFsSXRlbXM6ICc9JyxcbiAgICAgIHByZXZpb3VzVGV4dDogJ0AnLFxuICAgICAgbmV4dFRleHQ6ICdAJyxcbiAgICAgIG5nRGlzYWJsZWQ6ICc9J1xuICAgIH0sXG4gICAgcmVxdWlyZTogWyd1aWJQYWdlcicsICc/bmdNb2RlbCddLFxuICAgIGNvbnRyb2xsZXI6ICdVaWJQYWdpbmF0aW9uQ29udHJvbGxlcicsXG4gICAgY29udHJvbGxlckFzOiAncGFnaW5hdGlvbicsXG4gICAgdGVtcGxhdGVVcmw6IGZ1bmN0aW9uKGVsZW1lbnQsIGF0dHJzKSB7XG4gICAgICByZXR1cm4gYXR0cnMudGVtcGxhdGVVcmwgfHwgJ3RlbXBsYXRlL3BhZ2luYXRpb24vcGFnZXIuaHRtbCc7XG4gICAgfSxcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgY3RybHMpIHtcbiAgICAgIHZhciBwYWdpbmF0aW9uQ3RybCA9IGN0cmxzWzBdLCBuZ01vZGVsQ3RybCA9IGN0cmxzWzFdO1xuXG4gICAgICBpZiAoIW5nTW9kZWxDdHJsKSB7XG4gICAgICAgICByZXR1cm47IC8vIGRvIG5vdGhpbmcgaWYgbm8gbmctbW9kZWxcbiAgICAgIH1cblxuICAgICAgc2NvcGUuYWxpZ24gPSBhbmd1bGFyLmlzRGVmaW5lZChhdHRycy5hbGlnbikgPyBzY29wZS4kcGFyZW50LiRldmFsKGF0dHJzLmFsaWduKSA6IHBhZ2VyQ29uZmlnLmFsaWduO1xuICAgICAgcGFnaW5hdGlvbkN0cmwuaW5pdChuZ01vZGVsQ3RybCwgcGFnZXJDb25maWcpO1xuICAgIH1cbiAgfTtcbn1dKTtcblxuLyogRGVwcmVjYXRlZCBQYWdpbmF0aW9uIEJlbG93ICovXG5cbmFuZ3VsYXIubW9kdWxlKCd1aS5ib290c3RyYXAucGFnaW5hdGlvbicpXG4udmFsdWUoJyRwYWdpbmF0aW9uU3VwcHJlc3NXYXJuaW5nJywgZmFsc2UpXG4uY29udHJvbGxlcignUGFnaW5hdGlvbkNvbnRyb2xsZXInLCBbJyRzY29wZScsICckYXR0cnMnLCAnJHBhcnNlJywgJyRsb2cnLCAnJHBhZ2luYXRpb25TdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkc2NvcGUsICRhdHRycywgJHBhcnNlLCAkbG9nLCAkcGFnaW5hdGlvblN1cHByZXNzV2FybmluZykge1xuICBpZiAoISRwYWdpbmF0aW9uU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgJGxvZy53YXJuKCdQYWdpbmF0aW9uQ29udHJvbGxlciBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIFVpYlBhZ2luYXRpb25Db250cm9sbGVyIGluc3RlYWQuJyk7XG4gIH1cblxuICB2YXIgc2VsZiA9IHRoaXMsXG4gICAgbmdNb2RlbEN0cmwgPSB7ICRzZXRWaWV3VmFsdWU6IGFuZ3VsYXIubm9vcCB9LCAvLyBudWxsTW9kZWxDdHJsXG4gICAgc2V0TnVtUGFnZXMgPSAkYXR0cnMubnVtUGFnZXMgPyAkcGFyc2UoJGF0dHJzLm51bVBhZ2VzKS5hc3NpZ24gOiBhbmd1bGFyLm5vb3A7XG5cbiAgdGhpcy5pbml0ID0gZnVuY3Rpb24obmdNb2RlbEN0cmxfLCBjb25maWcpIHtcbiAgICBuZ01vZGVsQ3RybCA9IG5nTW9kZWxDdHJsXztcbiAgICB0aGlzLmNvbmZpZyA9IGNvbmZpZztcblxuICAgIG5nTW9kZWxDdHJsLiRyZW5kZXIgPSBmdW5jdGlvbigpIHtcbiAgICAgIHNlbGYucmVuZGVyKCk7XG4gICAgfTtcblxuICAgIGlmICgkYXR0cnMuaXRlbXNQZXJQYWdlKSB7XG4gICAgICAkc2NvcGUuJHBhcmVudC4kd2F0Y2goJHBhcnNlKCRhdHRycy5pdGVtc1BlclBhZ2UpLCBmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgICBzZWxmLml0ZW1zUGVyUGFnZSA9IHBhcnNlSW50KHZhbHVlLCAxMCk7XG4gICAgICAgICRzY29wZS50b3RhbFBhZ2VzID0gc2VsZi5jYWxjdWxhdGVUb3RhbFBhZ2VzKCk7XG4gICAgICB9KTtcbiAgICB9IGVsc2Uge1xuICAgICAgdGhpcy5pdGVtc1BlclBhZ2UgPSBjb25maWcuaXRlbXNQZXJQYWdlO1xuICAgIH1cblxuICAgICRzY29wZS4kd2F0Y2goJ3RvdGFsSXRlbXMnLCBmdW5jdGlvbigpIHtcbiAgICAgICRzY29wZS50b3RhbFBhZ2VzID0gc2VsZi5jYWxjdWxhdGVUb3RhbFBhZ2VzKCk7XG4gICAgfSk7XG5cbiAgICAkc2NvcGUuJHdhdGNoKCd0b3RhbFBhZ2VzJywgZnVuY3Rpb24odmFsdWUpIHtcbiAgICAgIHNldE51bVBhZ2VzKCRzY29wZS4kcGFyZW50LCB2YWx1ZSk7IC8vIFJlYWRvbmx5IHZhcmlhYmxlXG5cbiAgICAgIGlmICggJHNjb3BlLnBhZ2UgPiB2YWx1ZSApIHtcbiAgICAgICAgJHNjb3BlLnNlbGVjdFBhZ2UodmFsdWUpO1xuICAgICAgfSBlbHNlIHtcbiAgICAgICAgbmdNb2RlbEN0cmwuJHJlbmRlcigpO1xuICAgICAgfVxuICAgIH0pO1xuICB9O1xuXG4gIHRoaXMuY2FsY3VsYXRlVG90YWxQYWdlcyA9IGZ1bmN0aW9uKCkge1xuICAgIHZhciB0b3RhbFBhZ2VzID0gdGhpcy5pdGVtc1BlclBhZ2UgPCAxID8gMSA6IE1hdGguY2VpbCgkc2NvcGUudG90YWxJdGVtcyAvIHRoaXMuaXRlbXNQZXJQYWdlKTtcbiAgICByZXR1cm4gTWF0aC5tYXgodG90YWxQYWdlcyB8fCAwLCAxKTtcbiAgfTtcblxuICB0aGlzLnJlbmRlciA9IGZ1bmN0aW9uKCkge1xuICAgICRzY29wZS5wYWdlID0gcGFyc2VJbnQobmdNb2RlbEN0cmwuJHZpZXdWYWx1ZSwgMTApIHx8IDE7XG4gIH07XG5cbiAgJHNjb3BlLnNlbGVjdFBhZ2UgPSBmdW5jdGlvbihwYWdlLCBldnQpIHtcbiAgICBpZiAoZXZ0KSB7XG4gICAgICBldnQucHJldmVudERlZmF1bHQoKTtcbiAgICB9XG5cbiAgICB2YXIgY2xpY2tBbGxvd2VkID0gISRzY29wZS5uZ0Rpc2FibGVkIHx8ICFldnQ7XG4gICAgaWYgKGNsaWNrQWxsb3dlZCAmJiAkc2NvcGUucGFnZSAhPT0gcGFnZSAmJiBwYWdlID4gMCAmJiBwYWdlIDw9ICRzY29wZS50b3RhbFBhZ2VzKSB7XG4gICAgICBpZiAoZXZ0ICYmIGV2dC50YXJnZXQpIHtcbiAgICAgICAgZXZ0LnRhcmdldC5ibHVyKCk7XG4gICAgICB9XG4gICAgICBuZ01vZGVsQ3RybC4kc2V0Vmlld1ZhbHVlKHBhZ2UpO1xuICAgICAgbmdNb2RlbEN0cmwuJHJlbmRlcigpO1xuICAgIH1cbiAgfTtcblxuICAkc2NvcGUuZ2V0VGV4dCA9IGZ1bmN0aW9uKGtleSkge1xuICAgIHJldHVybiAkc2NvcGVba2V5ICsgJ1RleHQnXSB8fCBzZWxmLmNvbmZpZ1trZXkgKyAnVGV4dCddO1xuICB9O1xuXG4gICRzY29wZS5ub1ByZXZpb3VzID0gZnVuY3Rpb24oKSB7XG4gICAgcmV0dXJuICRzY29wZS5wYWdlID09PSAxO1xuICB9O1xuXG4gICRzY29wZS5ub05leHQgPSBmdW5jdGlvbigpIHtcbiAgICByZXR1cm4gJHNjb3BlLnBhZ2UgPT09ICRzY29wZS50b3RhbFBhZ2VzO1xuICB9O1xufV0pXG4uZGlyZWN0aXZlKCdwYWdpbmF0aW9uJywgWyckcGFyc2UnLCAndWliUGFnaW5hdGlvbkNvbmZpZycsICckbG9nJywgJyRwYWdpbmF0aW9uU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJHBhcnNlLCBwYWdpbmF0aW9uQ29uZmlnLCAkbG9nLCAkcGFnaW5hdGlvblN1cHByZXNzV2FybmluZykge1xuICByZXR1cm4ge1xuICAgIHJlc3RyaWN0OiAnRUEnLFxuICAgIHNjb3BlOiB7XG4gICAgICB0b3RhbEl0ZW1zOiAnPScsXG4gICAgICBmaXJzdFRleHQ6ICdAJyxcbiAgICAgIHByZXZpb3VzVGV4dDogJ0AnLFxuICAgICAgbmV4dFRleHQ6ICdAJyxcbiAgICAgIGxhc3RUZXh0OiAnQCcsXG4gICAgICBuZ0Rpc2FibGVkOic9J1xuICAgIH0sXG4gICAgcmVxdWlyZTogWydwYWdpbmF0aW9uJywgJz9uZ01vZGVsJ10sXG4gICAgY29udHJvbGxlcjogJ1BhZ2luYXRpb25Db250cm9sbGVyJyxcbiAgICBjb250cm9sbGVyQXM6ICdwYWdpbmF0aW9uJyxcbiAgICB0ZW1wbGF0ZVVybDogZnVuY3Rpb24oZWxlbWVudCwgYXR0cnMpIHtcbiAgICAgIHJldHVybiBhdHRycy50ZW1wbGF0ZVVybCB8fCAndGVtcGxhdGUvcGFnaW5hdGlvbi9wYWdpbmF0aW9uLmh0bWwnO1xuICAgIH0sXG4gICAgcmVwbGFjZTogdHJ1ZSxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMsIGN0cmxzKSB7XG4gICAgICBpZiAoISRwYWdpbmF0aW9uU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICRsb2cud2FybigncGFnaW5hdGlvbiBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi1wYWdpbmF0aW9uIGluc3RlYWQuJyk7XG4gICAgICB9XG4gICAgICB2YXIgcGFnaW5hdGlvbkN0cmwgPSBjdHJsc1swXSwgbmdNb2RlbEN0cmwgPSBjdHJsc1sxXTtcblxuICAgICAgaWYgKCFuZ01vZGVsQ3RybCkge1xuICAgICAgICAgcmV0dXJuOyAvLyBkbyBub3RoaW5nIGlmIG5vIG5nLW1vZGVsXG4gICAgICB9XG5cbiAgICAgIC8vIFNldHVwIGNvbmZpZ3VyYXRpb24gcGFyYW1ldGVyc1xuICAgICAgdmFyIG1heFNpemUgPSBhbmd1bGFyLmlzRGVmaW5lZChhdHRycy5tYXhTaXplKSA/IHNjb3BlLiRwYXJlbnQuJGV2YWwoYXR0cnMubWF4U2l6ZSkgOiBwYWdpbmF0aW9uQ29uZmlnLm1heFNpemUsXG4gICAgICAgICAgcm90YXRlID0gYW5ndWxhci5pc0RlZmluZWQoYXR0cnMucm90YXRlKSA/IHNjb3BlLiRwYXJlbnQuJGV2YWwoYXR0cnMucm90YXRlKSA6IHBhZ2luYXRpb25Db25maWcucm90YXRlO1xuICAgICAgc2NvcGUuYm91bmRhcnlMaW5rcyA9IGFuZ3VsYXIuaXNEZWZpbmVkKGF0dHJzLmJvdW5kYXJ5TGlua3MpID8gc2NvcGUuJHBhcmVudC4kZXZhbChhdHRycy5ib3VuZGFyeUxpbmtzKSA6IHBhZ2luYXRpb25Db25maWcuYm91bmRhcnlMaW5rcztcbiAgICAgIHNjb3BlLmRpcmVjdGlvbkxpbmtzID0gYW5ndWxhci5pc0RlZmluZWQoYXR0cnMuZGlyZWN0aW9uTGlua3MpID8gc2NvcGUuJHBhcmVudC4kZXZhbChhdHRycy5kaXJlY3Rpb25MaW5rcykgOiBwYWdpbmF0aW9uQ29uZmlnLmRpcmVjdGlvbkxpbmtzO1xuXG4gICAgICBwYWdpbmF0aW9uQ3RybC5pbml0KG5nTW9kZWxDdHJsLCBwYWdpbmF0aW9uQ29uZmlnKTtcblxuICAgICAgaWYgKGF0dHJzLm1heFNpemUpIHtcbiAgICAgICAgc2NvcGUuJHBhcmVudC4kd2F0Y2goJHBhcnNlKGF0dHJzLm1heFNpemUpLCBmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgICAgIG1heFNpemUgPSBwYXJzZUludCh2YWx1ZSwgMTApO1xuICAgICAgICAgIHBhZ2luYXRpb25DdHJsLnJlbmRlcigpO1xuICAgICAgICB9KTtcbiAgICAgIH1cblxuICAgICAgLy8gQ3JlYXRlIHBhZ2Ugb2JqZWN0IHVzZWQgaW4gdGVtcGxhdGVcbiAgICAgIGZ1bmN0aW9uIG1ha2VQYWdlKG51bWJlciwgdGV4dCwgaXNBY3RpdmUpIHtcbiAgICAgICAgcmV0dXJuIHtcbiAgICAgICAgICBudW1iZXI6IG51bWJlcixcbiAgICAgICAgICB0ZXh0OiB0ZXh0LFxuICAgICAgICAgIGFjdGl2ZTogaXNBY3RpdmVcbiAgICAgICAgfTtcbiAgICAgIH1cblxuICAgICAgZnVuY3Rpb24gZ2V0UGFnZXMoY3VycmVudFBhZ2UsIHRvdGFsUGFnZXMpIHtcbiAgICAgICAgdmFyIHBhZ2VzID0gW107XG5cbiAgICAgICAgLy8gRGVmYXVsdCBwYWdlIGxpbWl0c1xuICAgICAgICB2YXIgc3RhcnRQYWdlID0gMSwgZW5kUGFnZSA9IHRvdGFsUGFnZXM7XG4gICAgICAgIHZhciBpc01heFNpemVkID0gYW5ndWxhci5pc0RlZmluZWQobWF4U2l6ZSkgJiYgbWF4U2l6ZSA8IHRvdGFsUGFnZXM7XG5cbiAgICAgICAgLy8gcmVjb21wdXRlIGlmIG1heFNpemVcbiAgICAgICAgaWYgKGlzTWF4U2l6ZWQpIHtcbiAgICAgICAgICBpZiAocm90YXRlKSB7XG4gICAgICAgICAgICAvLyBDdXJyZW50IHBhZ2UgaXMgZGlzcGxheWVkIGluIHRoZSBtaWRkbGUgb2YgdGhlIHZpc2libGUgb25lc1xuICAgICAgICAgICAgc3RhcnRQYWdlID0gTWF0aC5tYXgoY3VycmVudFBhZ2UgLSBNYXRoLmZsb29yKG1heFNpemUvMiksIDEpO1xuICAgICAgICAgICAgZW5kUGFnZSAgID0gc3RhcnRQYWdlICsgbWF4U2l6ZSAtIDE7XG5cbiAgICAgICAgICAgIC8vIEFkanVzdCBpZiBsaW1pdCBpcyBleGNlZWRlZFxuICAgICAgICAgICAgaWYgKGVuZFBhZ2UgPiB0b3RhbFBhZ2VzKSB7XG4gICAgICAgICAgICAgIGVuZFBhZ2UgICA9IHRvdGFsUGFnZXM7XG4gICAgICAgICAgICAgIHN0YXJ0UGFnZSA9IGVuZFBhZ2UgLSBtYXhTaXplICsgMTtcbiAgICAgICAgICAgIH1cbiAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgLy8gVmlzaWJsZSBwYWdlcyBhcmUgcGFnaW5hdGVkIHdpdGggbWF4U2l6ZVxuICAgICAgICAgICAgc3RhcnRQYWdlID0gKChNYXRoLmNlaWwoY3VycmVudFBhZ2UgLyBtYXhTaXplKSAtIDEpICogbWF4U2l6ZSkgKyAxO1xuXG4gICAgICAgICAgICAvLyBBZGp1c3QgbGFzdCBwYWdlIGlmIGxpbWl0IGlzIGV4Y2VlZGVkXG4gICAgICAgICAgICBlbmRQYWdlID0gTWF0aC5taW4oc3RhcnRQYWdlICsgbWF4U2l6ZSAtIDEsIHRvdGFsUGFnZXMpO1xuICAgICAgICAgIH1cbiAgICAgICAgfVxuXG4gICAgICAgIC8vIEFkZCBwYWdlIG51bWJlciBsaW5rc1xuICAgICAgICBmb3IgKHZhciBudW1iZXIgPSBzdGFydFBhZ2U7IG51bWJlciA8PSBlbmRQYWdlOyBudW1iZXIrKykge1xuICAgICAgICAgIHZhciBwYWdlID0gbWFrZVBhZ2UobnVtYmVyLCBudW1iZXIsIG51bWJlciA9PT0gY3VycmVudFBhZ2UpO1xuICAgICAgICAgIHBhZ2VzLnB1c2gocGFnZSk7XG4gICAgICAgIH1cblxuICAgICAgICAvLyBBZGQgbGlua3MgdG8gbW92ZSBiZXR3ZWVuIHBhZ2Ugc2V0c1xuICAgICAgICBpZiAoaXNNYXhTaXplZCAmJiAhIHJvdGF0ZSkge1xuICAgICAgICAgIGlmIChzdGFydFBhZ2UgPiAxKSB7XG4gICAgICAgICAgICB2YXIgcHJldmlvdXNQYWdlU2V0ID0gbWFrZVBhZ2Uoc3RhcnRQYWdlIC0gMSwgJy4uLicsIGZhbHNlKTtcbiAgICAgICAgICAgIHBhZ2VzLnVuc2hpZnQocHJldmlvdXNQYWdlU2V0KTtcbiAgICAgICAgICB9XG5cbiAgICAgICAgICBpZiAoZW5kUGFnZSA8IHRvdGFsUGFnZXMpIHtcbiAgICAgICAgICAgIHZhciBuZXh0UGFnZVNldCA9IG1ha2VQYWdlKGVuZFBhZ2UgKyAxLCAnLi4uJywgZmFsc2UpO1xuICAgICAgICAgICAgcGFnZXMucHVzaChuZXh0UGFnZVNldCk7XG4gICAgICAgICAgfVxuICAgICAgICB9XG5cbiAgICAgICAgcmV0dXJuIHBhZ2VzO1xuICAgICAgfVxuXG4gICAgICB2YXIgb3JpZ2luYWxSZW5kZXIgPSBwYWdpbmF0aW9uQ3RybC5yZW5kZXI7XG4gICAgICBwYWdpbmF0aW9uQ3RybC5yZW5kZXIgPSBmdW5jdGlvbigpIHtcbiAgICAgICAgb3JpZ2luYWxSZW5kZXIoKTtcbiAgICAgICAgaWYgKHNjb3BlLnBhZ2UgPiAwICYmIHNjb3BlLnBhZ2UgPD0gc2NvcGUudG90YWxQYWdlcykge1xuICAgICAgICAgIHNjb3BlLnBhZ2VzID0gZ2V0UGFnZXMoc2NvcGUucGFnZSwgc2NvcGUudG90YWxQYWdlcyk7XG4gICAgICAgIH1cbiAgICAgIH07XG4gICAgfVxuICB9O1xufV0pXG5cbi5kaXJlY3RpdmUoJ3BhZ2VyJywgWyd1aWJQYWdlckNvbmZpZycsICckbG9nJywgJyRwYWdpbmF0aW9uU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24ocGFnZXJDb25maWcsICRsb2csICRwYWdpbmF0aW9uU3VwcHJlc3NXYXJuaW5nKSB7XG4gIHJldHVybiB7XG4gICAgcmVzdHJpY3Q6ICdFQScsXG4gICAgc2NvcGU6IHtcbiAgICAgIHRvdGFsSXRlbXM6ICc9JyxcbiAgICAgIHByZXZpb3VzVGV4dDogJ0AnLFxuICAgICAgbmV4dFRleHQ6ICdAJyxcbiAgICAgIG5nRGlzYWJsZWQ6ICc9J1xuICAgIH0sXG4gICAgcmVxdWlyZTogWydwYWdlcicsICc/bmdNb2RlbCddLFxuICAgIGNvbnRyb2xsZXI6ICdQYWdpbmF0aW9uQ29udHJvbGxlcicsXG4gICAgY29udHJvbGxlckFzOiAncGFnaW5hdGlvbicsXG4gICAgdGVtcGxhdGVVcmw6IGZ1bmN0aW9uKGVsZW1lbnQsIGF0dHJzKSB7XG4gICAgICByZXR1cm4gYXR0cnMudGVtcGxhdGVVcmwgfHwgJ3RlbXBsYXRlL3BhZ2luYXRpb24vcGFnZXIuaHRtbCc7XG4gICAgfSxcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgY3RybHMpIHtcbiAgICAgIGlmICghJHBhZ2luYXRpb25TdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgJGxvZy53YXJuKCdwYWdlciBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi1wYWdlciBpbnN0ZWFkLicpO1xuICAgICAgfVxuICAgICAgdmFyIHBhZ2luYXRpb25DdHJsID0gY3RybHNbMF0sIG5nTW9kZWxDdHJsID0gY3RybHNbMV07XG5cbiAgICAgIGlmICghbmdNb2RlbEN0cmwpIHtcbiAgICAgICAgIHJldHVybjsgLy8gZG8gbm90aGluZyBpZiBubyBuZy1tb2RlbFxuICAgICAgfVxuXG4gICAgICBzY29wZS5hbGlnbiA9IGFuZ3VsYXIuaXNEZWZpbmVkKGF0dHJzLmFsaWduKSA/IHNjb3BlLiRwYXJlbnQuJGV2YWwoYXR0cnMuYWxpZ24pIDogcGFnZXJDb25maWcuYWxpZ247XG4gICAgICBwYWdpbmF0aW9uQ3RybC5pbml0KG5nTW9kZWxDdHJsLCBwYWdlckNvbmZpZyk7XG4gICAgfVxuICB9O1xufV0pO1xuXG4vKipcbiAqIFRoZSBmb2xsb3dpbmcgZmVhdHVyZXMgYXJlIHN0aWxsIG91dHN0YW5kaW5nOiBhbmltYXRpb24gYXMgYVxuICogZnVuY3Rpb24sIHBsYWNlbWVudCBhcyBhIGZ1bmN0aW9uLCBpbnNpZGUsIHN1cHBvcnQgZm9yIG1vcmUgdHJpZ2dlcnMgdGhhblxuICoganVzdCBtb3VzZSBlbnRlci9sZWF2ZSwgaHRtbCB0b29sdGlwcywgYW5kIHNlbGVjdG9yIGRlbGVnYXRpb24uXG4gKi9cbmFuZ3VsYXIubW9kdWxlKCd1aS5ib290c3RyYXAudG9vbHRpcCcsIFsndWkuYm9vdHN0cmFwLnBvc2l0aW9uJywgJ3VpLmJvb3RzdHJhcC5zdGFja2VkTWFwJ10pXG5cbi8qKlxuICogVGhlICR0b29sdGlwIHNlcnZpY2UgY3JlYXRlcyB0b29sdGlwLSBhbmQgcG9wb3Zlci1saWtlIGRpcmVjdGl2ZXMgYXMgd2VsbCBhc1xuICogaG91c2VzIGdsb2JhbCBvcHRpb25zIGZvciB0aGVtLlxuICovXG4ucHJvdmlkZXIoJyR1aWJUb29sdGlwJywgZnVuY3Rpb24oKSB7XG4gIC8vIFRoZSBkZWZhdWx0IG9wdGlvbnMgdG9vbHRpcCBhbmQgcG9wb3Zlci5cbiAgdmFyIGRlZmF1bHRPcHRpb25zID0ge1xuICAgIHBsYWNlbWVudDogJ3RvcCcsXG4gICAgYW5pbWF0aW9uOiB0cnVlLFxuICAgIHBvcHVwRGVsYXk6IDAsXG4gICAgcG9wdXBDbG9zZURlbGF5OiAwLFxuICAgIHVzZUNvbnRlbnRFeHA6IGZhbHNlXG4gIH07XG5cbiAgLy8gRGVmYXVsdCBoaWRlIHRyaWdnZXJzIGZvciBlYWNoIHNob3cgdHJpZ2dlclxuICB2YXIgdHJpZ2dlck1hcCA9IHtcbiAgICAnbW91c2VlbnRlcic6ICdtb3VzZWxlYXZlJyxcbiAgICAnY2xpY2snOiAnY2xpY2snLFxuICAgICdmb2N1cyc6ICdibHVyJyxcbiAgICAnbm9uZSc6ICcnXG4gIH07XG5cbiAgLy8gVGhlIG9wdGlvbnMgc3BlY2lmaWVkIHRvIHRoZSBwcm92aWRlciBnbG9iYWxseS5cbiAgdmFyIGdsb2JhbE9wdGlvbnMgPSB7fTtcblxuICAvKipcbiAgICogYG9wdGlvbnMoe30pYCBhbGxvd3MgZ2xvYmFsIGNvbmZpZ3VyYXRpb24gb2YgYWxsIHRvb2x0aXBzIGluIHRoZVxuICAgKiBhcHBsaWNhdGlvbi5cbiAgICpcbiAgICogICB2YXIgYXBwID0gYW5ndWxhci5tb2R1bGUoICdBcHAnLCBbJ3VpLmJvb3RzdHJhcC50b29sdGlwJ10sIGZ1bmN0aW9uKCAkdG9vbHRpcFByb3ZpZGVyICkge1xuICAgKiAgICAgLy8gcGxhY2UgdG9vbHRpcHMgbGVmdCBpbnN0ZWFkIG9mIHRvcCBieSBkZWZhdWx0XG4gICAqICAgICAkdG9vbHRpcFByb3ZpZGVyLm9wdGlvbnMoIHsgcGxhY2VtZW50OiAnbGVmdCcgfSApO1xuICAgKiAgIH0pO1xuICAgKi9cblx0dGhpcy5vcHRpb25zID0gZnVuY3Rpb24odmFsdWUpIHtcblx0XHRhbmd1bGFyLmV4dGVuZChnbG9iYWxPcHRpb25zLCB2YWx1ZSk7XG5cdH07XG5cbiAgLyoqXG4gICAqIFRoaXMgYWxsb3dzIHlvdSB0byBleHRlbmQgdGhlIHNldCBvZiB0cmlnZ2VyIG1hcHBpbmdzIGF2YWlsYWJsZS4gRS5nLjpcbiAgICpcbiAgICogICAkdG9vbHRpcFByb3ZpZGVyLnNldFRyaWdnZXJzKCAnb3BlblRyaWdnZXInOiAnY2xvc2VUcmlnZ2VyJyApO1xuICAgKi9cbiAgdGhpcy5zZXRUcmlnZ2VycyA9IGZ1bmN0aW9uIHNldFRyaWdnZXJzKHRyaWdnZXJzKSB7XG4gICAgYW5ndWxhci5leHRlbmQodHJpZ2dlck1hcCwgdHJpZ2dlcnMpO1xuICB9O1xuXG4gIC8qKlxuICAgKiBUaGlzIGlzIGEgaGVscGVyIGZ1bmN0aW9uIGZvciB0cmFuc2xhdGluZyBjYW1lbC1jYXNlIHRvIHNuYWtlLWNhc2UuXG4gICAqL1xuICBmdW5jdGlvbiBzbmFrZV9jYXNlKG5hbWUpIHtcbiAgICB2YXIgcmVnZXhwID0gL1tBLVpdL2c7XG4gICAgdmFyIHNlcGFyYXRvciA9ICctJztcbiAgICByZXR1cm4gbmFtZS5yZXBsYWNlKHJlZ2V4cCwgZnVuY3Rpb24obGV0dGVyLCBwb3MpIHtcbiAgICAgIHJldHVybiAocG9zID8gc2VwYXJhdG9yIDogJycpICsgbGV0dGVyLnRvTG93ZXJDYXNlKCk7XG4gICAgfSk7XG4gIH1cblxuICAvKipcbiAgICogUmV0dXJucyB0aGUgYWN0dWFsIGluc3RhbmNlIG9mIHRoZSAkdG9vbHRpcCBzZXJ2aWNlLlxuICAgKiBUT0RPIHN1cHBvcnQgbXVsdGlwbGUgdHJpZ2dlcnNcbiAgICovXG4gIHRoaXMuJGdldCA9IFsnJHdpbmRvdycsICckY29tcGlsZScsICckdGltZW91dCcsICckZG9jdW1lbnQnLCAnJHVpYlBvc2l0aW9uJywgJyRpbnRlcnBvbGF0ZScsICckcm9vdFNjb3BlJywgJyRwYXJzZScsICckJHN0YWNrZWRNYXAnLCBmdW5jdGlvbigkd2luZG93LCAkY29tcGlsZSwgJHRpbWVvdXQsICRkb2N1bWVudCwgJHBvc2l0aW9uLCAkaW50ZXJwb2xhdGUsICRyb290U2NvcGUsICRwYXJzZSwgJCRzdGFja2VkTWFwKSB7XG4gICAgdmFyIG9wZW5lZFRvb2x0aXBzID0gJCRzdGFja2VkTWFwLmNyZWF0ZU5ldygpO1xuICAgICRkb2N1bWVudC5vbigna2V5cHJlc3MnLCBmdW5jdGlvbihlKSB7XG4gICAgICBpZiAoZS53aGljaCA9PT0gMjcpIHtcbiAgICAgICAgdmFyIGxhc3QgPSBvcGVuZWRUb29sdGlwcy50b3AoKTtcbiAgICAgICAgaWYgKGxhc3QpIHtcbiAgICAgICAgICBsYXN0LnZhbHVlLmNsb3NlKCk7XG4gICAgICAgICAgb3BlbmVkVG9vbHRpcHMucmVtb3ZlVG9wKCk7XG4gICAgICAgICAgbGFzdCA9IG51bGw7XG4gICAgICAgIH1cbiAgICAgIH1cbiAgICB9KTtcblxuICAgIHJldHVybiBmdW5jdGlvbiAkdG9vbHRpcCh0dFR5cGUsIHByZWZpeCwgZGVmYXVsdFRyaWdnZXJTaG93LCBvcHRpb25zKSB7XG4gICAgICBvcHRpb25zID0gYW5ndWxhci5leHRlbmQoe30sIGRlZmF1bHRPcHRpb25zLCBnbG9iYWxPcHRpb25zLCBvcHRpb25zKTtcblxuICAgICAgLyoqXG4gICAgICAgKiBSZXR1cm5zIGFuIG9iamVjdCBvZiBzaG93IGFuZCBoaWRlIHRyaWdnZXJzLlxuICAgICAgICpcbiAgICAgICAqIElmIGEgdHJpZ2dlciBpcyBzdXBwbGllZCxcbiAgICAgICAqIGl0IGlzIHVzZWQgdG8gc2hvdyB0aGUgdG9vbHRpcDsgb3RoZXJ3aXNlLCBpdCB3aWxsIHVzZSB0aGUgYHRyaWdnZXJgXG4gICAgICAgKiBvcHRpb24gcGFzc2VkIHRvIHRoZSBgJHRvb2x0aXBQcm92aWRlci5vcHRpb25zYCBtZXRob2Q7IGVsc2UgaXQgd2lsbFxuICAgICAgICogZGVmYXVsdCB0byB0aGUgdHJpZ2dlciBzdXBwbGllZCB0byB0aGlzIGRpcmVjdGl2ZSBmYWN0b3J5LlxuICAgICAgICpcbiAgICAgICAqIFRoZSBoaWRlIHRyaWdnZXIgaXMgYmFzZWQgb24gdGhlIHNob3cgdHJpZ2dlci4gSWYgdGhlIGB0cmlnZ2VyYCBvcHRpb25cbiAgICAgICAqIHdhcyBwYXNzZWQgdG8gdGhlIGAkdG9vbHRpcFByb3ZpZGVyLm9wdGlvbnNgIG1ldGhvZCwgaXQgd2lsbCB1c2UgdGhlXG4gICAgICAgKiBtYXBwZWQgdHJpZ2dlciBmcm9tIGB0cmlnZ2VyTWFwYCBvciB0aGUgcGFzc2VkIHRyaWdnZXIgaWYgdGhlIG1hcCBpc1xuICAgICAgICogdW5kZWZpbmVkOyBvdGhlcndpc2UsIGl0IHVzZXMgdGhlIGB0cmlnZ2VyTWFwYCB2YWx1ZSBvZiB0aGUgc2hvd1xuICAgICAgICogdHJpZ2dlcjsgZWxzZSBpdCB3aWxsIGp1c3QgdXNlIHRoZSBzaG93IHRyaWdnZXIuXG4gICAgICAgKi9cbiAgICAgIGZ1bmN0aW9uIGdldFRyaWdnZXJzKHRyaWdnZXIpIHtcbiAgICAgICAgdmFyIHNob3cgPSAodHJpZ2dlciB8fCBvcHRpb25zLnRyaWdnZXIgfHwgZGVmYXVsdFRyaWdnZXJTaG93KS5zcGxpdCgnICcpO1xuICAgICAgICB2YXIgaGlkZSA9IHNob3cubWFwKGZ1bmN0aW9uKHRyaWdnZXIpIHtcbiAgICAgICAgICByZXR1cm4gdHJpZ2dlck1hcFt0cmlnZ2VyXSB8fCB0cmlnZ2VyO1xuICAgICAgICB9KTtcbiAgICAgICAgcmV0dXJuIHtcbiAgICAgICAgICBzaG93OiBzaG93LFxuICAgICAgICAgIGhpZGU6IGhpZGVcbiAgICAgICAgfTtcbiAgICAgIH1cblxuICAgICAgdmFyIGRpcmVjdGl2ZU5hbWUgPSBzbmFrZV9jYXNlKHR0VHlwZSk7XG5cbiAgICAgIHZhciBzdGFydFN5bSA9ICRpbnRlcnBvbGF0ZS5zdGFydFN5bWJvbCgpO1xuICAgICAgdmFyIGVuZFN5bSA9ICRpbnRlcnBvbGF0ZS5lbmRTeW1ib2woKTtcbiAgICAgIHZhciB0ZW1wbGF0ZSA9XG4gICAgICAgICc8ZGl2ICcrIGRpcmVjdGl2ZU5hbWUgKyAnLXBvcHVwICcrXG4gICAgICAgICAgJ3RpdGxlPVwiJyArIHN0YXJ0U3ltICsgJ3RpdGxlJyArIGVuZFN5bSArICdcIiAnK1xuICAgICAgICAgIChvcHRpb25zLnVzZUNvbnRlbnRFeHAgP1xuICAgICAgICAgICAgJ2NvbnRlbnQtZXhwPVwiY29udGVudEV4cCgpXCIgJyA6XG4gICAgICAgICAgICAnY29udGVudD1cIicgKyBzdGFydFN5bSArICdjb250ZW50JyArIGVuZFN5bSArICdcIiAnKSArXG4gICAgICAgICAgJ3BsYWNlbWVudD1cIicgKyBzdGFydFN5bSArICdwbGFjZW1lbnQnICsgZW5kU3ltICsgJ1wiICcrXG4gICAgICAgICAgJ3BvcHVwLWNsYXNzPVwiJyArIHN0YXJ0U3ltICsgJ3BvcHVwQ2xhc3MnICsgZW5kU3ltICsgJ1wiICcrXG4gICAgICAgICAgJ2FuaW1hdGlvbj1cImFuaW1hdGlvblwiICcgK1xuICAgICAgICAgICdpcy1vcGVuPVwiaXNPcGVuXCInICtcbiAgICAgICAgICAnb3JpZ2luLXNjb3BlPVwib3JpZ1Njb3BlXCIgJyArXG4gICAgICAgICAgJ3N0eWxlPVwidmlzaWJpbGl0eTogaGlkZGVuOyBkaXNwbGF5OiBibG9jazsgdG9wOiAtOTk5OXB4OyBsZWZ0OiAtOTk5OXB4O1wiJyArXG4gICAgICAgICAgJz4nICtcbiAgICAgICAgJzwvZGl2Pic7XG5cbiAgICAgIHJldHVybiB7XG4gICAgICAgIGNvbXBpbGU6IGZ1bmN0aW9uKHRFbGVtLCB0QXR0cnMpIHtcbiAgICAgICAgICB2YXIgdG9vbHRpcExpbmtlciA9ICRjb21waWxlKHRlbXBsYXRlKTtcblxuICAgICAgICAgIHJldHVybiBmdW5jdGlvbiBsaW5rKHNjb3BlLCBlbGVtZW50LCBhdHRycywgdG9vbHRpcEN0cmwpIHtcbiAgICAgICAgICAgIHZhciB0b29sdGlwO1xuICAgICAgICAgICAgdmFyIHRvb2x0aXBMaW5rZWRTY29wZTtcbiAgICAgICAgICAgIHZhciB0cmFuc2l0aW9uVGltZW91dDtcbiAgICAgICAgICAgIHZhciBzaG93VGltZW91dDtcbiAgICAgICAgICAgIHZhciBoaWRlVGltZW91dDtcbiAgICAgICAgICAgIHZhciBwb3NpdGlvblRpbWVvdXQ7XG4gICAgICAgICAgICB2YXIgYXBwZW5kVG9Cb2R5ID0gYW5ndWxhci5pc0RlZmluZWQob3B0aW9ucy5hcHBlbmRUb0JvZHkpID8gb3B0aW9ucy5hcHBlbmRUb0JvZHkgOiBmYWxzZTtcbiAgICAgICAgICAgIHZhciB0cmlnZ2VycyA9IGdldFRyaWdnZXJzKHVuZGVmaW5lZCk7XG4gICAgICAgICAgICB2YXIgaGFzRW5hYmxlRXhwID0gYW5ndWxhci5pc0RlZmluZWQoYXR0cnNbcHJlZml4ICsgJ0VuYWJsZSddKTtcbiAgICAgICAgICAgIHZhciB0dFNjb3BlID0gc2NvcGUuJG5ldyh0cnVlKTtcbiAgICAgICAgICAgIHZhciByZXBvc2l0aW9uU2NoZWR1bGVkID0gZmFsc2U7XG4gICAgICAgICAgICB2YXIgaXNPcGVuUGFyc2UgPSBhbmd1bGFyLmlzRGVmaW5lZChhdHRyc1twcmVmaXggKyAnSXNPcGVuJ10pID8gJHBhcnNlKGF0dHJzW3ByZWZpeCArICdJc09wZW4nXSkgOiBmYWxzZTtcbiAgICAgICAgICAgIHZhciBjb250ZW50UGFyc2UgPSBvcHRpb25zLnVzZUNvbnRlbnRFeHAgPyAkcGFyc2UoYXR0cnNbdHRUeXBlXSkgOiBmYWxzZTtcbiAgICAgICAgICAgIHZhciBvYnNlcnZlcnMgPSBbXTtcblxuICAgICAgICAgICAgdmFyIHBvc2l0aW9uVG9vbHRpcCA9IGZ1bmN0aW9uKCkge1xuICAgICAgICAgICAgICAvLyBjaGVjayBpZiB0b29sdGlwIGV4aXN0cyBhbmQgaXMgbm90IGVtcHR5XG4gICAgICAgICAgICAgIGlmICghdG9vbHRpcCB8fCAhdG9vbHRpcC5odG1sKCkpIHsgcmV0dXJuOyB9XG5cbiAgICAgICAgICAgICAgaWYgKCFwb3NpdGlvblRpbWVvdXQpIHtcbiAgICAgICAgICAgICAgICBwb3NpdGlvblRpbWVvdXQgPSAkdGltZW91dChmdW5jdGlvbigpIHtcbiAgICAgICAgICAgICAgICAgIC8vIFJlc2V0IHRoZSBwb3NpdGlvbmluZy5cbiAgICAgICAgICAgICAgICAgIHRvb2x0aXAuY3NzKHsgdG9wOiAwLCBsZWZ0OiAwIH0pO1xuXG4gICAgICAgICAgICAgICAgICAvLyBOb3cgc2V0IHRoZSBjYWxjdWxhdGVkIHBvc2l0aW9uaW5nLlxuICAgICAgICAgICAgICAgICAgdmFyIHR0Q3NzID0gJHBvc2l0aW9uLnBvc2l0aW9uRWxlbWVudHMoZWxlbWVudCwgdG9vbHRpcCwgdHRTY29wZS5wbGFjZW1lbnQsIGFwcGVuZFRvQm9keSk7XG4gICAgICAgICAgICAgICAgICB0dENzcy50b3AgKz0gJ3B4JztcbiAgICAgICAgICAgICAgICAgIHR0Q3NzLmxlZnQgKz0gJ3B4JztcbiAgICAgICAgICAgICAgICAgIHR0Q3NzLnZpc2liaWxpdHkgPSAndmlzaWJsZSc7XG4gICAgICAgICAgICAgICAgICB0b29sdGlwLmNzcyh0dENzcyk7XG5cbiAgICAgICAgICAgICAgICAgIHBvc2l0aW9uVGltZW91dCA9IG51bGw7XG4gICAgICAgICAgICAgICAgfSwgMCwgZmFsc2UpO1xuICAgICAgICAgICAgICB9XG4gICAgICAgICAgICB9O1xuXG4gICAgICAgICAgICAvLyBTZXQgdXAgdGhlIGNvcnJlY3Qgc2NvcGUgdG8gYWxsb3cgdHJhbnNjbHVzaW9uIGxhdGVyXG4gICAgICAgICAgICB0dFNjb3BlLm9yaWdTY29wZSA9IHNjb3BlO1xuXG4gICAgICAgICAgICAvLyBCeSBkZWZhdWx0LCB0aGUgdG9vbHRpcCBpcyBub3Qgb3Blbi5cbiAgICAgICAgICAgIC8vIFRPRE8gYWRkIGFiaWxpdHkgdG8gc3RhcnQgdG9vbHRpcCBvcGVuZWRcbiAgICAgICAgICAgIHR0U2NvcGUuaXNPcGVuID0gZmFsc2U7XG4gICAgICAgICAgICBvcGVuZWRUb29sdGlwcy5hZGQodHRTY29wZSwge1xuICAgICAgICAgICAgICBjbG9zZTogaGlkZVxuICAgICAgICAgICAgfSk7XG5cbiAgICAgICAgICAgIGZ1bmN0aW9uIHRvZ2dsZVRvb2x0aXBCaW5kKCkge1xuICAgICAgICAgICAgICBpZiAoIXR0U2NvcGUuaXNPcGVuKSB7XG4gICAgICAgICAgICAgICAgc2hvd1Rvb2x0aXBCaW5kKCk7XG4gICAgICAgICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgICAgICAgaGlkZVRvb2x0aXBCaW5kKCk7XG4gICAgICAgICAgICAgIH1cbiAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgLy8gU2hvdyB0aGUgdG9vbHRpcCB3aXRoIGRlbGF5IGlmIHNwZWNpZmllZCwgb3RoZXJ3aXNlIHNob3cgaXQgaW1tZWRpYXRlbHlcbiAgICAgICAgICAgIGZ1bmN0aW9uIHNob3dUb29sdGlwQmluZCgpIHtcbiAgICAgICAgICAgICAgaWYgKGhhc0VuYWJsZUV4cCAmJiAhc2NvcGUuJGV2YWwoYXR0cnNbcHJlZml4ICsgJ0VuYWJsZSddKSkge1xuICAgICAgICAgICAgICAgIHJldHVybjtcbiAgICAgICAgICAgICAgfVxuXG4gICAgICAgICAgICAgIGNhbmNlbEhpZGUoKTtcbiAgICAgICAgICAgICAgcHJlcGFyZVRvb2x0aXAoKTtcblxuICAgICAgICAgICAgICBpZiAodHRTY29wZS5wb3B1cERlbGF5KSB7XG4gICAgICAgICAgICAgICAgLy8gRG8gbm90aGluZyBpZiB0aGUgdG9vbHRpcCB3YXMgYWxyZWFkeSBzY2hlZHVsZWQgdG8gcG9wLXVwLlxuICAgICAgICAgICAgICAgIC8vIFRoaXMgaGFwcGVucyBpZiBzaG93IGlzIHRyaWdnZXJlZCBtdWx0aXBsZSB0aW1lcyBiZWZvcmUgYW55IGhpZGUgaXMgdHJpZ2dlcmVkLlxuICAgICAgICAgICAgICAgIGlmICghc2hvd1RpbWVvdXQpIHtcbiAgICAgICAgICAgICAgICAgIHNob3dUaW1lb3V0ID0gJHRpbWVvdXQoc2hvdywgdHRTY29wZS5wb3B1cERlbGF5LCBmYWxzZSk7XG4gICAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgICAgIHNob3coKTtcbiAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgfVxuXG4gICAgICAgICAgICBmdW5jdGlvbiBoaWRlVG9vbHRpcEJpbmQoKSB7XG4gICAgICAgICAgICAgIGNhbmNlbFNob3coKTtcblxuICAgICAgICAgICAgICBpZiAodHRTY29wZS5wb3B1cENsb3NlRGVsYXkpIHtcbiAgICAgICAgICAgICAgICBpZiAoIWhpZGVUaW1lb3V0KSB7XG4gICAgICAgICAgICAgICAgICBoaWRlVGltZW91dCA9ICR0aW1lb3V0KGhpZGUsIHR0U2NvcGUucG9wdXBDbG9zZURlbGF5LCBmYWxzZSk7XG4gICAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgICAgIGhpZGUoKTtcbiAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgfVxuXG4gICAgICAgICAgICAvLyBTaG93IHRoZSB0b29sdGlwIHBvcHVwIGVsZW1lbnQuXG4gICAgICAgICAgICBmdW5jdGlvbiBzaG93KCkge1xuICAgICAgICAgICAgICBjYW5jZWxTaG93KCk7XG4gICAgICAgICAgICAgIGNhbmNlbEhpZGUoKTtcblxuICAgICAgICAgICAgICAvLyBEb24ndCBzaG93IGVtcHR5IHRvb2x0aXBzLlxuICAgICAgICAgICAgICBpZiAoIXR0U2NvcGUuY29udGVudCkge1xuICAgICAgICAgICAgICAgIHJldHVybiBhbmd1bGFyLm5vb3A7XG4gICAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgICBjcmVhdGVUb29sdGlwKCk7XG5cbiAgICAgICAgICAgICAgLy8gQW5kIHNob3cgdGhlIHRvb2x0aXAuXG4gICAgICAgICAgICAgIHR0U2NvcGUuJGV2YWxBc3luYyhmdW5jdGlvbigpIHtcbiAgICAgICAgICAgICAgICB0dFNjb3BlLmlzT3BlbiA9IHRydWU7XG4gICAgICAgICAgICAgICAgYXNzaWduSXNPcGVuKHRydWUpO1xuICAgICAgICAgICAgICAgIHBvc2l0aW9uVG9vbHRpcCgpO1xuICAgICAgICAgICAgICB9KTtcbiAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgZnVuY3Rpb24gY2FuY2VsU2hvdygpIHtcbiAgICAgICAgICAgICAgaWYgKHNob3dUaW1lb3V0KSB7XG4gICAgICAgICAgICAgICAgJHRpbWVvdXQuY2FuY2VsKHNob3dUaW1lb3V0KTtcbiAgICAgICAgICAgICAgICBzaG93VGltZW91dCA9IG51bGw7XG4gICAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgICBpZiAocG9zaXRpb25UaW1lb3V0KSB7XG4gICAgICAgICAgICAgICAgJHRpbWVvdXQuY2FuY2VsKHBvc2l0aW9uVGltZW91dCk7XG4gICAgICAgICAgICAgICAgcG9zaXRpb25UaW1lb3V0ID0gbnVsbDtcbiAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgfVxuXG4gICAgICAgICAgICAvLyBIaWRlIHRoZSB0b29sdGlwIHBvcHVwIGVsZW1lbnQuXG4gICAgICAgICAgICBmdW5jdGlvbiBoaWRlKCkge1xuICAgICAgICAgICAgICBjYW5jZWxTaG93KCk7XG4gICAgICAgICAgICAgIGNhbmNlbEhpZGUoKTtcblxuICAgICAgICAgICAgICBpZiAoIXR0U2NvcGUpIHtcbiAgICAgICAgICAgICAgICByZXR1cm47XG4gICAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgICAvLyBGaXJzdCB0aGluZ3MgZmlyc3Q6IHdlIGRvbid0IHNob3cgaXQgYW55bW9yZS5cbiAgICAgICAgICAgICAgdHRTY29wZS4kZXZhbEFzeW5jKGZ1bmN0aW9uKCkge1xuICAgICAgICAgICAgICAgIHR0U2NvcGUuaXNPcGVuID0gZmFsc2U7XG4gICAgICAgICAgICAgICAgYXNzaWduSXNPcGVuKGZhbHNlKTtcbiAgICAgICAgICAgICAgICAvLyBBbmQgbm93IHdlIHJlbW92ZSBpdCBmcm9tIHRoZSBET00uIEhvd2V2ZXIsIGlmIHdlIGhhdmUgYW5pbWF0aW9uLCB3ZVxuICAgICAgICAgICAgICAgIC8vIG5lZWQgdG8gd2FpdCBmb3IgaXQgdG8gZXhwaXJlIGJlZm9yZWhhbmQuXG4gICAgICAgICAgICAgICAgLy8gRklYTUU6IHRoaXMgaXMgYSBwbGFjZWhvbGRlciBmb3IgYSBwb3J0IG9mIHRoZSB0cmFuc2l0aW9ucyBsaWJyYXJ5LlxuICAgICAgICAgICAgICAgIC8vIFRoZSBmYWRlIHRyYW5zaXRpb24gaW4gVFdCUyBpcyAxNTBtcy5cbiAgICAgICAgICAgICAgICBpZiAodHRTY29wZS5hbmltYXRpb24pIHtcbiAgICAgICAgICAgICAgICAgIGlmICghdHJhbnNpdGlvblRpbWVvdXQpIHtcbiAgICAgICAgICAgICAgICAgICAgdHJhbnNpdGlvblRpbWVvdXQgPSAkdGltZW91dChyZW1vdmVUb29sdGlwLCAxNTAsIGZhbHNlKTtcbiAgICAgICAgICAgICAgICAgIH1cbiAgICAgICAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgICAgICAgcmVtb3ZlVG9vbHRpcCgpO1xuICAgICAgICAgICAgICAgIH1cbiAgICAgICAgICAgICAgfSk7XG4gICAgICAgICAgICB9XG5cbiAgICAgICAgICAgIGZ1bmN0aW9uIGNhbmNlbEhpZGUoKSB7XG4gICAgICAgICAgICAgIGlmIChoaWRlVGltZW91dCkge1xuICAgICAgICAgICAgICAgICR0aW1lb3V0LmNhbmNlbChoaWRlVGltZW91dCk7XG4gICAgICAgICAgICAgICAgaGlkZVRpbWVvdXQgPSBudWxsO1xuICAgICAgICAgICAgICB9XG4gICAgICAgICAgICAgIGlmICh0cmFuc2l0aW9uVGltZW91dCkge1xuICAgICAgICAgICAgICAgICR0aW1lb3V0LmNhbmNlbCh0cmFuc2l0aW9uVGltZW91dCk7XG4gICAgICAgICAgICAgICAgdHJhbnNpdGlvblRpbWVvdXQgPSBudWxsO1xuICAgICAgICAgICAgICB9XG4gICAgICAgICAgICB9XG5cbiAgICAgICAgICAgIGZ1bmN0aW9uIGNyZWF0ZVRvb2x0aXAoKSB7XG4gICAgICAgICAgICAgIC8vIFRoZXJlIGNhbiBvbmx5IGJlIG9uZSB0b29sdGlwIGVsZW1lbnQgcGVyIGRpcmVjdGl2ZSBzaG93biBhdCBvbmNlLlxuICAgICAgICAgICAgICBpZiAodG9vbHRpcCkge1xuICAgICAgICAgICAgICAgIHJldHVybjtcbiAgICAgICAgICAgICAgfVxuXG4gICAgICAgICAgICAgIHRvb2x0aXBMaW5rZWRTY29wZSA9IHR0U2NvcGUuJG5ldygpO1xuICAgICAgICAgICAgICB0b29sdGlwID0gdG9vbHRpcExpbmtlcih0b29sdGlwTGlua2VkU2NvcGUsIGZ1bmN0aW9uKHRvb2x0aXApIHtcbiAgICAgICAgICAgICAgICBpZiAoYXBwZW5kVG9Cb2R5KSB7XG4gICAgICAgICAgICAgICAgICAkZG9jdW1lbnQuZmluZCgnYm9keScpLmFwcGVuZCh0b29sdGlwKTtcbiAgICAgICAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgICAgICAgZWxlbWVudC5hZnRlcih0b29sdGlwKTtcbiAgICAgICAgICAgICAgICB9XG4gICAgICAgICAgICAgIH0pO1xuXG4gICAgICAgICAgICAgIHByZXBPYnNlcnZlcnMoKTtcbiAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgZnVuY3Rpb24gcmVtb3ZlVG9vbHRpcCgpIHtcbiAgICAgICAgICAgICAgdW5yZWdpc3Rlck9ic2VydmVycygpO1xuXG4gICAgICAgICAgICAgIHRyYW5zaXRpb25UaW1lb3V0ID0gbnVsbDtcbiAgICAgICAgICAgICAgaWYgKHRvb2x0aXApIHtcbiAgICAgICAgICAgICAgICB0b29sdGlwLnJlbW92ZSgpO1xuICAgICAgICAgICAgICAgIHRvb2x0aXAgPSBudWxsO1xuICAgICAgICAgICAgICB9XG4gICAgICAgICAgICAgIGlmICh0b29sdGlwTGlua2VkU2NvcGUpIHtcbiAgICAgICAgICAgICAgICB0b29sdGlwTGlua2VkU2NvcGUuJGRlc3Ryb3koKTtcbiAgICAgICAgICAgICAgICB0b29sdGlwTGlua2VkU2NvcGUgPSBudWxsO1xuICAgICAgICAgICAgICB9XG4gICAgICAgICAgICB9XG5cbiAgICAgICAgICAgIC8qKlxuICAgICAgICAgICAgICogU2V0IHRoZSBpbml0YWwgc2NvcGUgdmFsdWVzLiBPbmNlXG4gICAgICAgICAgICAgKiB0aGUgdG9vbHRpcCBpcyBjcmVhdGVkLCB0aGUgb2JzZXJ2ZXJzXG4gICAgICAgICAgICAgKiB3aWxsIGJlIGFkZGVkIHRvIGtlZXAgdGhpbmdzIGluIHN5bmNoLlxuICAgICAgICAgICAgICovXG4gICAgICAgICAgICBmdW5jdGlvbiBwcmVwYXJlVG9vbHRpcCgpIHtcbiAgICAgICAgICAgICAgdHRTY29wZS50aXRsZSA9IGF0dHJzW3ByZWZpeCArICdUaXRsZSddO1xuICAgICAgICAgICAgICBpZiAoY29udGVudFBhcnNlKSB7XG4gICAgICAgICAgICAgICAgdHRTY29wZS5jb250ZW50ID0gY29udGVudFBhcnNlKHNjb3BlKTtcbiAgICAgICAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICAgICAgICB0dFNjb3BlLmNvbnRlbnQgPSBhdHRyc1t0dFR5cGVdO1xuICAgICAgICAgICAgICB9XG5cbiAgICAgICAgICAgICAgdHRTY29wZS5wb3B1cENsYXNzID0gYXR0cnNbcHJlZml4ICsgJ0NsYXNzJ107XG4gICAgICAgICAgICAgIHR0U2NvcGUucGxhY2VtZW50ID0gYW5ndWxhci5pc0RlZmluZWQoYXR0cnNbcHJlZml4ICsgJ1BsYWNlbWVudCddKSA/IGF0dHJzW3ByZWZpeCArICdQbGFjZW1lbnQnXSA6IG9wdGlvbnMucGxhY2VtZW50O1xuXG4gICAgICAgICAgICAgIHZhciBkZWxheSA9IHBhcnNlSW50KGF0dHJzW3ByZWZpeCArICdQb3B1cERlbGF5J10sIDEwKTtcbiAgICAgICAgICAgICAgdmFyIGNsb3NlRGVsYXkgPSBwYXJzZUludChhdHRyc1twcmVmaXggKyAnUG9wdXBDbG9zZURlbGF5J10sIDEwKTtcbiAgICAgICAgICAgICAgdHRTY29wZS5wb3B1cERlbGF5ID0gIWlzTmFOKGRlbGF5KSA/IGRlbGF5IDogb3B0aW9ucy5wb3B1cERlbGF5O1xuICAgICAgICAgICAgICB0dFNjb3BlLnBvcHVwQ2xvc2VEZWxheSA9ICFpc05hTihjbG9zZURlbGF5KSA/IGNsb3NlRGVsYXkgOiBvcHRpb25zLnBvcHVwQ2xvc2VEZWxheTtcbiAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgZnVuY3Rpb24gYXNzaWduSXNPcGVuKGlzT3Blbikge1xuICAgICAgICAgICAgICBpZiAoaXNPcGVuUGFyc2UgJiYgYW5ndWxhci5pc0Z1bmN0aW9uKGlzT3BlblBhcnNlLmFzc2lnbikpIHtcbiAgICAgICAgICAgICAgICBpc09wZW5QYXJzZS5hc3NpZ24oc2NvcGUsIGlzT3Blbik7XG4gICAgICAgICAgICAgIH1cbiAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgdHRTY29wZS5jb250ZW50RXhwID0gZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICAgIHJldHVybiB0dFNjb3BlLmNvbnRlbnQ7XG4gICAgICAgICAgICB9O1xuXG4gICAgICAgICAgICAvKipcbiAgICAgICAgICAgICAqIE9ic2VydmUgdGhlIHJlbGV2YW50IGF0dHJpYnV0ZXMuXG4gICAgICAgICAgICAgKi9cbiAgICAgICAgICAgIGF0dHJzLiRvYnNlcnZlKCdkaXNhYmxlZCcsIGZ1bmN0aW9uKHZhbCkge1xuICAgICAgICAgICAgICBpZiAodmFsKSB7XG4gICAgICAgICAgICAgICAgY2FuY2VsU2hvdygpO1xuICAgICAgICAgICAgICB9XG5cbiAgICAgICAgICAgICAgaWYgKHZhbCAmJiB0dFNjb3BlLmlzT3Blbikge1xuICAgICAgICAgICAgICAgIGhpZGUoKTtcbiAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgfSk7XG5cbiAgICAgICAgICAgIGlmIChpc09wZW5QYXJzZSkge1xuICAgICAgICAgICAgICBzY29wZS4kd2F0Y2goaXNPcGVuUGFyc2UsIGZ1bmN0aW9uKHZhbCkge1xuICAgICAgICAgICAgICAgIC8qanNoaW50IC1XMDE4ICovXG4gICAgICAgICAgICAgICAgaWYgKHR0U2NvcGUgJiYgIXZhbCA9PT0gdHRTY29wZS5pc09wZW4pIHtcbiAgICAgICAgICAgICAgICAgIHRvZ2dsZVRvb2x0aXBCaW5kKCk7XG4gICAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgICAgIC8qanNoaW50ICtXMDE4ICovXG4gICAgICAgICAgICAgIH0pO1xuICAgICAgICAgICAgfVxuXG4gICAgICAgICAgICBmdW5jdGlvbiBwcmVwT2JzZXJ2ZXJzKCkge1xuICAgICAgICAgICAgICBvYnNlcnZlcnMubGVuZ3RoID0gMDtcblxuICAgICAgICAgICAgICBpZiAoY29udGVudFBhcnNlKSB7XG4gICAgICAgICAgICAgICAgb2JzZXJ2ZXJzLnB1c2goXG4gICAgICAgICAgICAgICAgICBzY29wZS4kd2F0Y2goY29udGVudFBhcnNlLCBmdW5jdGlvbih2YWwpIHtcbiAgICAgICAgICAgICAgICAgICAgdHRTY29wZS5jb250ZW50ID0gdmFsO1xuICAgICAgICAgICAgICAgICAgICBpZiAoIXZhbCAmJiB0dFNjb3BlLmlzT3Blbikge1xuICAgICAgICAgICAgICAgICAgICAgIGhpZGUoKTtcbiAgICAgICAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgICAgICAgfSlcbiAgICAgICAgICAgICAgICApO1xuXG4gICAgICAgICAgICAgICAgb2JzZXJ2ZXJzLnB1c2goXG4gICAgICAgICAgICAgICAgICB0b29sdGlwTGlua2VkU2NvcGUuJHdhdGNoKGZ1bmN0aW9uKCkge1xuICAgICAgICAgICAgICAgICAgICBpZiAoIXJlcG9zaXRpb25TY2hlZHVsZWQpIHtcbiAgICAgICAgICAgICAgICAgICAgICByZXBvc2l0aW9uU2NoZWR1bGVkID0gdHJ1ZTtcbiAgICAgICAgICAgICAgICAgICAgICB0b29sdGlwTGlua2VkU2NvcGUuJCRwb3N0RGlnZXN0KGZ1bmN0aW9uKCkge1xuICAgICAgICAgICAgICAgICAgICAgICAgcmVwb3NpdGlvblNjaGVkdWxlZCA9IGZhbHNlO1xuICAgICAgICAgICAgICAgICAgICAgICAgaWYgKHR0U2NvcGUgJiYgdHRTY29wZS5pc09wZW4pIHtcbiAgICAgICAgICAgICAgICAgICAgICAgICAgcG9zaXRpb25Ub29sdGlwKCk7XG4gICAgICAgICAgICAgICAgICAgICAgICB9XG4gICAgICAgICAgICAgICAgICAgICAgfSk7XG4gICAgICAgICAgICAgICAgICAgIH1cbiAgICAgICAgICAgICAgICAgIH0pXG4gICAgICAgICAgICAgICAgKTtcbiAgICAgICAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICAgICAgICBvYnNlcnZlcnMucHVzaChcbiAgICAgICAgICAgICAgICAgIGF0dHJzLiRvYnNlcnZlKHR0VHlwZSwgZnVuY3Rpb24odmFsKSB7XG4gICAgICAgICAgICAgICAgICAgIHR0U2NvcGUuY29udGVudCA9IHZhbDtcbiAgICAgICAgICAgICAgICAgICAgaWYgKCF2YWwgJiYgdHRTY29wZS5pc09wZW4pIHtcbiAgICAgICAgICAgICAgICAgICAgICBoaWRlKCk7XG4gICAgICAgICAgICAgICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgICAgICAgICAgICAgcG9zaXRpb25Ub29sdGlwKCk7XG4gICAgICAgICAgICAgICAgICAgIH1cbiAgICAgICAgICAgICAgICAgIH0pXG4gICAgICAgICAgICAgICAgKTtcbiAgICAgICAgICAgICAgfVxuXG4gICAgICAgICAgICAgIG9ic2VydmVycy5wdXNoKFxuICAgICAgICAgICAgICAgIGF0dHJzLiRvYnNlcnZlKHByZWZpeCArICdUaXRsZScsIGZ1bmN0aW9uKHZhbCkge1xuICAgICAgICAgICAgICAgICAgdHRTY29wZS50aXRsZSA9IHZhbDtcbiAgICAgICAgICAgICAgICAgIGlmICh0dFNjb3BlLmlzT3Blbikge1xuICAgICAgICAgICAgICAgICAgICBwb3NpdGlvblRvb2x0aXAoKTtcbiAgICAgICAgICAgICAgICAgIH1cbiAgICAgICAgICAgICAgICB9KVxuICAgICAgICAgICAgICApO1xuXG4gICAgICAgICAgICAgIG9ic2VydmVycy5wdXNoKFxuICAgICAgICAgICAgICAgIGF0dHJzLiRvYnNlcnZlKHByZWZpeCArICdQbGFjZW1lbnQnLCBmdW5jdGlvbih2YWwpIHtcbiAgICAgICAgICAgICAgICAgIHR0U2NvcGUucGxhY2VtZW50ID0gdmFsID8gdmFsIDogb3B0aW9ucy5wbGFjZW1lbnQ7XG4gICAgICAgICAgICAgICAgICBpZiAodHRTY29wZS5pc09wZW4pIHtcbiAgICAgICAgICAgICAgICAgICAgcG9zaXRpb25Ub29sdGlwKCk7XG4gICAgICAgICAgICAgICAgICB9XG4gICAgICAgICAgICAgICAgfSlcbiAgICAgICAgICAgICAgKTtcbiAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgZnVuY3Rpb24gdW5yZWdpc3Rlck9ic2VydmVycygpIHtcbiAgICAgICAgICAgICAgaWYgKG9ic2VydmVycy5sZW5ndGgpIHtcbiAgICAgICAgICAgICAgICBhbmd1bGFyLmZvckVhY2gob2JzZXJ2ZXJzLCBmdW5jdGlvbihvYnNlcnZlcikge1xuICAgICAgICAgICAgICAgICAgb2JzZXJ2ZXIoKTtcbiAgICAgICAgICAgICAgICB9KTtcbiAgICAgICAgICAgICAgICBvYnNlcnZlcnMubGVuZ3RoID0gMDtcbiAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgfVxuXG4gICAgICAgICAgICB2YXIgdW5yZWdpc3RlclRyaWdnZXJzID0gZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICAgIHRyaWdnZXJzLnNob3cuZm9yRWFjaChmdW5jdGlvbih0cmlnZ2VyKSB7XG4gICAgICAgICAgICAgICAgZWxlbWVudC51bmJpbmQodHJpZ2dlciwgc2hvd1Rvb2x0aXBCaW5kKTtcbiAgICAgICAgICAgICAgfSk7XG4gICAgICAgICAgICAgIHRyaWdnZXJzLmhpZGUuZm9yRWFjaChmdW5jdGlvbih0cmlnZ2VyKSB7XG4gICAgICAgICAgICAgICAgdHJpZ2dlci5zcGxpdCgnICcpLmZvckVhY2goZnVuY3Rpb24oaGlkZVRyaWdnZXIpIHtcbiAgICAgICAgICAgICAgICAgIGVsZW1lbnRbMF0ucmVtb3ZlRXZlbnRMaXN0ZW5lcihoaWRlVHJpZ2dlciwgaGlkZVRvb2x0aXBCaW5kKTtcbiAgICAgICAgICAgICAgICB9KTtcbiAgICAgICAgICAgICAgfSk7XG4gICAgICAgICAgICB9O1xuXG4gICAgICAgICAgICBmdW5jdGlvbiBwcmVwVHJpZ2dlcnMoKSB7XG4gICAgICAgICAgICAgIHZhciB2YWwgPSBhdHRyc1twcmVmaXggKyAnVHJpZ2dlciddO1xuICAgICAgICAgICAgICB1bnJlZ2lzdGVyVHJpZ2dlcnMoKTtcblxuICAgICAgICAgICAgICB0cmlnZ2VycyA9IGdldFRyaWdnZXJzKHZhbCk7XG5cbiAgICAgICAgICAgICAgaWYgKHRyaWdnZXJzLnNob3cgIT09ICdub25lJykge1xuICAgICAgICAgICAgICAgIHRyaWdnZXJzLnNob3cuZm9yRWFjaChmdW5jdGlvbih0cmlnZ2VyLCBpZHgpIHtcbiAgICAgICAgICAgICAgICAgIC8vIFVzaW5nIHJhdyBhZGRFdmVudExpc3RlbmVyIGR1ZSB0byBqcUxpdGUvalF1ZXJ5IGJ1ZyAtICM0MDYwXG4gICAgICAgICAgICAgICAgICBpZiAodHJpZ2dlciA9PT0gdHJpZ2dlcnMuaGlkZVtpZHhdKSB7XG4gICAgICAgICAgICAgICAgICAgIGVsZW1lbnRbMF0uYWRkRXZlbnRMaXN0ZW5lcih0cmlnZ2VyLCB0b2dnbGVUb29sdGlwQmluZCk7XG4gICAgICAgICAgICAgICAgICB9IGVsc2UgaWYgKHRyaWdnZXIpIHtcbiAgICAgICAgICAgICAgICAgICAgZWxlbWVudFswXS5hZGRFdmVudExpc3RlbmVyKHRyaWdnZXIsIHNob3dUb29sdGlwQmluZCk7XG4gICAgICAgICAgICAgICAgICAgIHRyaWdnZXJzLmhpZGVbaWR4XS5zcGxpdCgnICcpLmZvckVhY2goZnVuY3Rpb24odHJpZ2dlcikge1xuICAgICAgICAgICAgICAgICAgICAgIGVsZW1lbnRbMF0uYWRkRXZlbnRMaXN0ZW5lcih0cmlnZ2VyLCBoaWRlVG9vbHRpcEJpbmQpO1xuICAgICAgICAgICAgICAgICAgICB9KTtcbiAgICAgICAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgICAgICAgZWxlbWVudC5vbigna2V5cHJlc3MnLCBmdW5jdGlvbihlKSB7XG4gICAgICAgICAgICAgICAgICAgIGlmIChlLndoaWNoID09PSAyNykge1xuICAgICAgICAgICAgICAgICAgICAgIGhpZGVUb29sdGlwQmluZCgpO1xuICAgICAgICAgICAgICAgICAgICB9XG4gICAgICAgICAgICAgICAgICB9KTtcbiAgICAgICAgICAgICAgICB9KTtcbiAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgfVxuXG4gICAgICAgICAgICBwcmVwVHJpZ2dlcnMoKTtcblxuICAgICAgICAgICAgdmFyIGFuaW1hdGlvbiA9IHNjb3BlLiRldmFsKGF0dHJzW3ByZWZpeCArICdBbmltYXRpb24nXSk7XG4gICAgICAgICAgICB0dFNjb3BlLmFuaW1hdGlvbiA9IGFuZ3VsYXIuaXNEZWZpbmVkKGFuaW1hdGlvbikgPyAhIWFuaW1hdGlvbiA6IG9wdGlvbnMuYW5pbWF0aW9uO1xuXG4gICAgICAgICAgICB2YXIgYXBwZW5kVG9Cb2R5VmFsID0gc2NvcGUuJGV2YWwoYXR0cnNbcHJlZml4ICsgJ0FwcGVuZFRvQm9keSddKTtcbiAgICAgICAgICAgIGFwcGVuZFRvQm9keSA9IGFuZ3VsYXIuaXNEZWZpbmVkKGFwcGVuZFRvQm9keVZhbCkgPyBhcHBlbmRUb0JvZHlWYWwgOiBhcHBlbmRUb0JvZHk7XG5cbiAgICAgICAgICAgIC8vIGlmIGEgdG9vbHRpcCBpcyBhdHRhY2hlZCB0byA8Ym9keT4gd2UgbmVlZCB0byByZW1vdmUgaXQgb25cbiAgICAgICAgICAgIC8vIGxvY2F0aW9uIGNoYW5nZSBhcyBpdHMgcGFyZW50IHNjb3BlIHdpbGwgcHJvYmFibHkgbm90IGJlIGRlc3Ryb3llZFxuICAgICAgICAgICAgLy8gYnkgdGhlIGNoYW5nZS5cbiAgICAgICAgICAgIGlmIChhcHBlbmRUb0JvZHkpIHtcbiAgICAgICAgICAgICAgc2NvcGUuJG9uKCckbG9jYXRpb25DaGFuZ2VTdWNjZXNzJywgZnVuY3Rpb24gY2xvc2VUb29sdGlwT25Mb2NhdGlvbkNoYW5nZVN1Y2Nlc3MoKSB7XG4gICAgICAgICAgICAgICAgaWYgKHR0U2NvcGUuaXNPcGVuKSB7XG4gICAgICAgICAgICAgICAgICBoaWRlKCk7XG4gICAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgICB9KTtcbiAgICAgICAgICAgIH1cblxuICAgICAgICAgICAgLy8gTWFrZSBzdXJlIHRvb2x0aXAgaXMgZGVzdHJveWVkIGFuZCByZW1vdmVkLlxuICAgICAgICAgICAgc2NvcGUuJG9uKCckZGVzdHJveScsIGZ1bmN0aW9uIG9uRGVzdHJveVRvb2x0aXAoKSB7XG4gICAgICAgICAgICAgIGNhbmNlbFNob3coKTtcbiAgICAgICAgICAgICAgY2FuY2VsSGlkZSgpO1xuICAgICAgICAgICAgICB1bnJlZ2lzdGVyVHJpZ2dlcnMoKTtcbiAgICAgICAgICAgICAgcmVtb3ZlVG9vbHRpcCgpO1xuICAgICAgICAgICAgICBvcGVuZWRUb29sdGlwcy5yZW1vdmUodHRTY29wZSk7XG4gICAgICAgICAgICAgIHR0U2NvcGUgPSBudWxsO1xuICAgICAgICAgICAgfSk7XG4gICAgICAgICAgfTtcbiAgICAgICAgfVxuICAgICAgfTtcbiAgICB9O1xuICB9XTtcbn0pXG5cbi8vIFRoaXMgaXMgbW9zdGx5IG5nSW5jbHVkZSBjb2RlIGJ1dCB3aXRoIGEgY3VzdG9tIHNjb3BlXG4uZGlyZWN0aXZlKCd1aWJUb29sdGlwVGVtcGxhdGVUcmFuc2NsdWRlJywgW1xuICAgICAgICAgJyRhbmltYXRlJywgJyRzY2UnLCAnJGNvbXBpbGUnLCAnJHRlbXBsYXRlUmVxdWVzdCcsXG5mdW5jdGlvbiAoJGFuaW1hdGUgLCAgJHNjZSAsICAkY29tcGlsZSAsICAkdGVtcGxhdGVSZXF1ZXN0KSB7XG4gIHJldHVybiB7XG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW0sIGF0dHJzKSB7XG4gICAgICB2YXIgb3JpZ1Njb3BlID0gc2NvcGUuJGV2YWwoYXR0cnMudG9vbHRpcFRlbXBsYXRlVHJhbnNjbHVkZVNjb3BlKTtcblxuICAgICAgdmFyIGNoYW5nZUNvdW50ZXIgPSAwLFxuICAgICAgICBjdXJyZW50U2NvcGUsXG4gICAgICAgIHByZXZpb3VzRWxlbWVudCxcbiAgICAgICAgY3VycmVudEVsZW1lbnQ7XG5cbiAgICAgIHZhciBjbGVhbnVwTGFzdEluY2x1ZGVDb250ZW50ID0gZnVuY3Rpb24oKSB7XG4gICAgICAgIGlmIChwcmV2aW91c0VsZW1lbnQpIHtcbiAgICAgICAgICBwcmV2aW91c0VsZW1lbnQucmVtb3ZlKCk7XG4gICAgICAgICAgcHJldmlvdXNFbGVtZW50ID0gbnVsbDtcbiAgICAgICAgfVxuXG4gICAgICAgIGlmIChjdXJyZW50U2NvcGUpIHtcbiAgICAgICAgICBjdXJyZW50U2NvcGUuJGRlc3Ryb3koKTtcbiAgICAgICAgICBjdXJyZW50U2NvcGUgPSBudWxsO1xuICAgICAgICB9XG5cbiAgICAgICAgaWYgKGN1cnJlbnRFbGVtZW50KSB7XG4gICAgICAgICAgJGFuaW1hdGUubGVhdmUoY3VycmVudEVsZW1lbnQpLnRoZW4oZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICBwcmV2aW91c0VsZW1lbnQgPSBudWxsO1xuICAgICAgICAgIH0pO1xuICAgICAgICAgIHByZXZpb3VzRWxlbWVudCA9IGN1cnJlbnRFbGVtZW50O1xuICAgICAgICAgIGN1cnJlbnRFbGVtZW50ID0gbnVsbDtcbiAgICAgICAgfVxuICAgICAgfTtcblxuICAgICAgc2NvcGUuJHdhdGNoKCRzY2UucGFyc2VBc1Jlc291cmNlVXJsKGF0dHJzLnVpYlRvb2x0aXBUZW1wbGF0ZVRyYW5zY2x1ZGUpLCBmdW5jdGlvbihzcmMpIHtcbiAgICAgICAgdmFyIHRoaXNDaGFuZ2VJZCA9ICsrY2hhbmdlQ291bnRlcjtcblxuICAgICAgICBpZiAoc3JjKSB7XG4gICAgICAgICAgLy9zZXQgdGhlIDJuZCBwYXJhbSB0byB0cnVlIHRvIGlnbm9yZSB0aGUgdGVtcGxhdGUgcmVxdWVzdCBlcnJvciBzbyB0aGF0IHRoZSBpbm5lclxuICAgICAgICAgIC8vY29udGVudHMgYW5kIHNjb3BlIGNhbiBiZSBjbGVhbmVkIHVwLlxuICAgICAgICAgICR0ZW1wbGF0ZVJlcXVlc3Qoc3JjLCB0cnVlKS50aGVuKGZ1bmN0aW9uKHJlc3BvbnNlKSB7XG4gICAgICAgICAgICBpZiAodGhpc0NoYW5nZUlkICE9PSBjaGFuZ2VDb3VudGVyKSB7IHJldHVybjsgfVxuICAgICAgICAgICAgdmFyIG5ld1Njb3BlID0gb3JpZ1Njb3BlLiRuZXcoKTtcbiAgICAgICAgICAgIHZhciB0ZW1wbGF0ZSA9IHJlc3BvbnNlO1xuXG4gICAgICAgICAgICB2YXIgY2xvbmUgPSAkY29tcGlsZSh0ZW1wbGF0ZSkobmV3U2NvcGUsIGZ1bmN0aW9uKGNsb25lKSB7XG4gICAgICAgICAgICAgIGNsZWFudXBMYXN0SW5jbHVkZUNvbnRlbnQoKTtcbiAgICAgICAgICAgICAgJGFuaW1hdGUuZW50ZXIoY2xvbmUsIGVsZW0pO1xuICAgICAgICAgICAgfSk7XG5cbiAgICAgICAgICAgIGN1cnJlbnRTY29wZSA9IG5ld1Njb3BlO1xuICAgICAgICAgICAgY3VycmVudEVsZW1lbnQgPSBjbG9uZTtcblxuICAgICAgICAgICAgY3VycmVudFNjb3BlLiRlbWl0KCckaW5jbHVkZUNvbnRlbnRMb2FkZWQnLCBzcmMpO1xuICAgICAgICAgIH0sIGZ1bmN0aW9uKCkge1xuICAgICAgICAgICAgaWYgKHRoaXNDaGFuZ2VJZCA9PT0gY2hhbmdlQ291bnRlcikge1xuICAgICAgICAgICAgICBjbGVhbnVwTGFzdEluY2x1ZGVDb250ZW50KCk7XG4gICAgICAgICAgICAgIHNjb3BlLiRlbWl0KCckaW5jbHVkZUNvbnRlbnRFcnJvcicsIHNyYyk7XG4gICAgICAgICAgICB9XG4gICAgICAgICAgfSk7XG4gICAgICAgICAgc2NvcGUuJGVtaXQoJyRpbmNsdWRlQ29udGVudFJlcXVlc3RlZCcsIHNyYyk7XG4gICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgY2xlYW51cExhc3RJbmNsdWRlQ29udGVudCgpO1xuICAgICAgICB9XG4gICAgICB9KTtcblxuICAgICAgc2NvcGUuJG9uKCckZGVzdHJveScsIGNsZWFudXBMYXN0SW5jbHVkZUNvbnRlbnQpO1xuICAgIH1cbiAgfTtcbn1dKVxuXG4vKipcbiAqIE5vdGUgdGhhdCBpdCdzIGludGVudGlvbmFsIHRoYXQgdGhlc2UgY2xhc3NlcyBhcmUgKm5vdCogYXBwbGllZCB0aHJvdWdoICRhbmltYXRlLlxuICogVGhleSBtdXN0IG5vdCBiZSBhbmltYXRlZCBhcyB0aGV5J3JlIGV4cGVjdGVkIHRvIGJlIHByZXNlbnQgb24gdGhlIHRvb2x0aXAgb25cbiAqIGluaXRpYWxpemF0aW9uLlxuICovXG4uZGlyZWN0aXZlKCd1aWJUb29sdGlwQ2xhc3NlcycsIGZ1bmN0aW9uKCkge1xuICByZXR1cm4ge1xuICAgIHJlc3RyaWN0OiAnQScsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHJzKSB7XG4gICAgICBpZiAoc2NvcGUucGxhY2VtZW50KSB7XG4gICAgICAgIGVsZW1lbnQuYWRkQ2xhc3Moc2NvcGUucGxhY2VtZW50KTtcbiAgICAgIH1cblxuICAgICAgaWYgKHNjb3BlLnBvcHVwQ2xhc3MpIHtcbiAgICAgICAgZWxlbWVudC5hZGRDbGFzcyhzY29wZS5wb3B1cENsYXNzKTtcbiAgICAgIH1cblxuICAgICAgaWYgKHNjb3BlLmFuaW1hdGlvbigpKSB7XG4gICAgICAgIGVsZW1lbnQuYWRkQ2xhc3MoYXR0cnMudG9vbHRpcEFuaW1hdGlvbkNsYXNzKTtcbiAgICAgIH1cbiAgICB9XG4gIH07XG59KVxuXG4uZGlyZWN0aXZlKCd1aWJUb29sdGlwUG9wdXAnLCBmdW5jdGlvbigpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHNjb3BlOiB7IGNvbnRlbnQ6ICdAJywgcGxhY2VtZW50OiAnQCcsIHBvcHVwQ2xhc3M6ICdAJywgYW5pbWF0aW9uOiAnJicsIGlzT3BlbjogJyYnIH0sXG4gICAgdGVtcGxhdGVVcmw6ICd0ZW1wbGF0ZS90b29sdGlwL3Rvb2x0aXAtcG9wdXAuaHRtbCcsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQpIHtcbiAgICAgIGVsZW1lbnQuYWRkQ2xhc3MoJ3Rvb2x0aXAnKTtcbiAgICB9XG4gIH07XG59KVxuXG4uZGlyZWN0aXZlKCd1aWJUb29sdGlwJywgWyAnJHVpYlRvb2x0aXAnLCBmdW5jdGlvbigkdWliVG9vbHRpcCkge1xuICByZXR1cm4gJHVpYlRvb2x0aXAoJ3VpYlRvb2x0aXAnLCAndG9vbHRpcCcsICdtb3VzZWVudGVyJyk7XG59XSlcblxuLmRpcmVjdGl2ZSgndWliVG9vbHRpcFRlbXBsYXRlUG9wdXAnLCBmdW5jdGlvbigpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHNjb3BlOiB7IGNvbnRlbnRFeHA6ICcmJywgcGxhY2VtZW50OiAnQCcsIHBvcHVwQ2xhc3M6ICdAJywgYW5pbWF0aW9uOiAnJicsIGlzT3BlbjogJyYnLFxuICAgICAgb3JpZ2luU2NvcGU6ICcmJyB9LFxuICAgIHRlbXBsYXRlVXJsOiAndGVtcGxhdGUvdG9vbHRpcC90b29sdGlwLXRlbXBsYXRlLXBvcHVwLmh0bWwnLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50KSB7XG4gICAgICBlbGVtZW50LmFkZENsYXNzKCd0b29sdGlwJyk7XG4gICAgfVxuICB9O1xufSlcblxuLmRpcmVjdGl2ZSgndWliVG9vbHRpcFRlbXBsYXRlJywgWyckdWliVG9vbHRpcCcsIGZ1bmN0aW9uKCR1aWJUb29sdGlwKSB7XG4gIHJldHVybiAkdWliVG9vbHRpcCgndWliVG9vbHRpcFRlbXBsYXRlJywgJ3Rvb2x0aXAnLCAnbW91c2VlbnRlcicsIHtcbiAgICB1c2VDb250ZW50RXhwOiB0cnVlXG4gIH0pO1xufV0pXG5cbi5kaXJlY3RpdmUoJ3VpYlRvb2x0aXBIdG1sUG9wdXAnLCBmdW5jdGlvbigpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHNjb3BlOiB7IGNvbnRlbnRFeHA6ICcmJywgcGxhY2VtZW50OiAnQCcsIHBvcHVwQ2xhc3M6ICdAJywgYW5pbWF0aW9uOiAnJicsIGlzT3BlbjogJyYnIH0sXG4gICAgdGVtcGxhdGVVcmw6ICd0ZW1wbGF0ZS90b29sdGlwL3Rvb2x0aXAtaHRtbC1wb3B1cC5odG1sJyxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCkge1xuICAgICAgZWxlbWVudC5hZGRDbGFzcygndG9vbHRpcCcpO1xuICAgIH1cbiAgfTtcbn0pXG5cbi5kaXJlY3RpdmUoJ3VpYlRvb2x0aXBIdG1sJywgWyckdWliVG9vbHRpcCcsIGZ1bmN0aW9uKCR1aWJUb29sdGlwKSB7XG4gIHJldHVybiAkdWliVG9vbHRpcCgndWliVG9vbHRpcEh0bWwnLCAndG9vbHRpcCcsICdtb3VzZWVudGVyJywge1xuICAgIHVzZUNvbnRlbnRFeHA6IHRydWVcbiAgfSk7XG59XSk7XG5cbi8qIERlcHJlY2F0ZWQgdG9vbHRpcCBiZWxvdyAqL1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLnRvb2x0aXAnKVxuXG4udmFsdWUoJyR0b29sdGlwU3VwcHJlc3NXYXJuaW5nJywgZmFsc2UpXG5cbi5wcm92aWRlcignJHRvb2x0aXAnLCBbJyR1aWJUb29sdGlwUHJvdmlkZXInLCBmdW5jdGlvbigkdWliVG9vbHRpcFByb3ZpZGVyKSB7XG4gIGFuZ3VsYXIuZXh0ZW5kKHRoaXMsICR1aWJUb29sdGlwUHJvdmlkZXIpO1xuXG4gIHRoaXMuJGdldCA9IFsnJGxvZycsICckdG9vbHRpcFN1cHByZXNzV2FybmluZycsICckaW5qZWN0b3InLCBmdW5jdGlvbigkbG9nLCAkdG9vbHRpcFN1cHByZXNzV2FybmluZywgJGluamVjdG9yKSB7XG4gICAgaWYgKCEkdG9vbHRpcFN1cHByZXNzV2FybmluZykge1xuICAgICAgJGxvZy53YXJuKCckdG9vbHRpcCBpcyBub3cgZGVwcmVjYXRlZC4gVXNlICR1aWJUb29sdGlwIGluc3RlYWQuJyk7XG4gICAgfVxuXG4gICAgcmV0dXJuICRpbmplY3Rvci5pbnZva2UoJHVpYlRvb2x0aXBQcm92aWRlci4kZ2V0KTtcbiAgfV07XG59XSlcblxuLy8gVGhpcyBpcyBtb3N0bHkgbmdJbmNsdWRlIGNvZGUgYnV0IHdpdGggYSBjdXN0b20gc2NvcGVcbi5kaXJlY3RpdmUoJ3Rvb2x0aXBUZW1wbGF0ZVRyYW5zY2x1ZGUnLCBbXG4gICAgICAgICAnJGFuaW1hdGUnLCAnJHNjZScsICckY29tcGlsZScsICckdGVtcGxhdGVSZXF1ZXN0JywgJyRsb2cnLCAnJHRvb2x0aXBTdXBwcmVzc1dhcm5pbmcnLFxuZnVuY3Rpb24gKCRhbmltYXRlICwgICRzY2UgLCAgJGNvbXBpbGUgLCAgJHRlbXBsYXRlUmVxdWVzdCwgICAkbG9nLCAgICR0b29sdGlwU3VwcHJlc3NXYXJuaW5nKSB7XG4gIHJldHVybiB7XG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW0sIGF0dHJzKSB7XG4gICAgICBpZiAoISR0b29sdGlwU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICRsb2cud2FybigndG9vbHRpcC10ZW1wbGF0ZS10cmFuc2NsdWRlIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliLXRvb2x0aXAtdGVtcGxhdGUtdHJhbnNjbHVkZSBpbnN0ZWFkLicpO1xuICAgICAgfVxuXG4gICAgICB2YXIgb3JpZ1Njb3BlID0gc2NvcGUuJGV2YWwoYXR0cnMudG9vbHRpcFRlbXBsYXRlVHJhbnNjbHVkZVNjb3BlKTtcblxuICAgICAgdmFyIGNoYW5nZUNvdW50ZXIgPSAwLFxuICAgICAgICBjdXJyZW50U2NvcGUsXG4gICAgICAgIHByZXZpb3VzRWxlbWVudCxcbiAgICAgICAgY3VycmVudEVsZW1lbnQ7XG5cbiAgICAgIHZhciBjbGVhbnVwTGFzdEluY2x1ZGVDb250ZW50ID0gZnVuY3Rpb24oKSB7XG4gICAgICAgIGlmIChwcmV2aW91c0VsZW1lbnQpIHtcbiAgICAgICAgICBwcmV2aW91c0VsZW1lbnQucmVtb3ZlKCk7XG4gICAgICAgICAgcHJldmlvdXNFbGVtZW50ID0gbnVsbDtcbiAgICAgICAgfVxuICAgICAgICBpZiAoY3VycmVudFNjb3BlKSB7XG4gICAgICAgICAgY3VycmVudFNjb3BlLiRkZXN0cm95KCk7XG4gICAgICAgICAgY3VycmVudFNjb3BlID0gbnVsbDtcbiAgICAgICAgfVxuICAgICAgICBpZiAoY3VycmVudEVsZW1lbnQpIHtcbiAgICAgICAgICAkYW5pbWF0ZS5sZWF2ZShjdXJyZW50RWxlbWVudCkudGhlbihmdW5jdGlvbigpIHtcbiAgICAgICAgICAgIHByZXZpb3VzRWxlbWVudCA9IG51bGw7XG4gICAgICAgICAgfSk7XG4gICAgICAgICAgcHJldmlvdXNFbGVtZW50ID0gY3VycmVudEVsZW1lbnQ7XG4gICAgICAgICAgY3VycmVudEVsZW1lbnQgPSBudWxsO1xuICAgICAgICB9XG4gICAgICB9O1xuXG4gICAgICBzY29wZS4kd2F0Y2goJHNjZS5wYXJzZUFzUmVzb3VyY2VVcmwoYXR0cnMudG9vbHRpcFRlbXBsYXRlVHJhbnNjbHVkZSksIGZ1bmN0aW9uKHNyYykge1xuICAgICAgICB2YXIgdGhpc0NoYW5nZUlkID0gKytjaGFuZ2VDb3VudGVyO1xuXG4gICAgICAgIGlmIChzcmMpIHtcbiAgICAgICAgICAvL3NldCB0aGUgMm5kIHBhcmFtIHRvIHRydWUgdG8gaWdub3JlIHRoZSB0ZW1wbGF0ZSByZXF1ZXN0IGVycm9yIHNvIHRoYXQgdGhlIGlubmVyXG4gICAgICAgICAgLy9jb250ZW50cyBhbmQgc2NvcGUgY2FuIGJlIGNsZWFuZWQgdXAuXG4gICAgICAgICAgJHRlbXBsYXRlUmVxdWVzdChzcmMsIHRydWUpLnRoZW4oZnVuY3Rpb24ocmVzcG9uc2UpIHtcbiAgICAgICAgICAgIGlmICh0aGlzQ2hhbmdlSWQgIT09IGNoYW5nZUNvdW50ZXIpIHsgcmV0dXJuOyB9XG4gICAgICAgICAgICB2YXIgbmV3U2NvcGUgPSBvcmlnU2NvcGUuJG5ldygpO1xuICAgICAgICAgICAgdmFyIHRlbXBsYXRlID0gcmVzcG9uc2U7XG5cbiAgICAgICAgICAgIHZhciBjbG9uZSA9ICRjb21waWxlKHRlbXBsYXRlKShuZXdTY29wZSwgZnVuY3Rpb24oY2xvbmUpIHtcbiAgICAgICAgICAgICAgY2xlYW51cExhc3RJbmNsdWRlQ29udGVudCgpO1xuICAgICAgICAgICAgICAkYW5pbWF0ZS5lbnRlcihjbG9uZSwgZWxlbSk7XG4gICAgICAgICAgICB9KTtcblxuICAgICAgICAgICAgY3VycmVudFNjb3BlID0gbmV3U2NvcGU7XG4gICAgICAgICAgICBjdXJyZW50RWxlbWVudCA9IGNsb25lO1xuXG4gICAgICAgICAgICBjdXJyZW50U2NvcGUuJGVtaXQoJyRpbmNsdWRlQ29udGVudExvYWRlZCcsIHNyYyk7XG4gICAgICAgICAgfSwgZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICBpZiAodGhpc0NoYW5nZUlkID09PSBjaGFuZ2VDb3VudGVyKSB7XG4gICAgICAgICAgICAgIGNsZWFudXBMYXN0SW5jbHVkZUNvbnRlbnQoKTtcbiAgICAgICAgICAgICAgc2NvcGUuJGVtaXQoJyRpbmNsdWRlQ29udGVudEVycm9yJywgc3JjKTtcbiAgICAgICAgICAgIH1cbiAgICAgICAgICB9KTtcbiAgICAgICAgICBzY29wZS4kZW1pdCgnJGluY2x1ZGVDb250ZW50UmVxdWVzdGVkJywgc3JjKTtcbiAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICBjbGVhbnVwTGFzdEluY2x1ZGVDb250ZW50KCk7XG4gICAgICAgIH1cbiAgICAgIH0pO1xuXG4gICAgICBzY29wZS4kb24oJyRkZXN0cm95JywgY2xlYW51cExhc3RJbmNsdWRlQ29udGVudCk7XG4gICAgfVxuICB9O1xufV0pXG5cbi5kaXJlY3RpdmUoJ3Rvb2x0aXBDbGFzc2VzJywgWyckbG9nJywgJyR0b29sdGlwU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJGxvZywgJHRvb2x0aXBTdXBwcmVzc1dhcm5pbmcpIHtcbiAgcmV0dXJuIHtcbiAgICByZXN0cmljdDogJ0EnLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycykge1xuICAgICAgaWYgKCEkdG9vbHRpcFN1cHByZXNzV2FybmluZykge1xuICAgICAgICAkbG9nLndhcm4oJ3Rvb2x0aXAtY2xhc3NlcyBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi10b29sdGlwLWNsYXNzZXMgaW5zdGVhZC4nKTtcbiAgICAgIH1cblxuICAgICAgaWYgKHNjb3BlLnBsYWNlbWVudCkge1xuICAgICAgICBlbGVtZW50LmFkZENsYXNzKHNjb3BlLnBsYWNlbWVudCk7XG4gICAgICB9XG4gICAgICBpZiAoc2NvcGUucG9wdXBDbGFzcykge1xuICAgICAgICBlbGVtZW50LmFkZENsYXNzKHNjb3BlLnBvcHVwQ2xhc3MpO1xuICAgICAgfVxuICAgICAgaWYgKHNjb3BlLmFuaW1hdGlvbigpKSB7XG4gICAgICAgIGVsZW1lbnQuYWRkQ2xhc3MoYXR0cnMudG9vbHRpcEFuaW1hdGlvbkNsYXNzKTtcbiAgICAgIH1cbiAgICB9XG4gIH07XG59XSlcblxuLmRpcmVjdGl2ZSgndG9vbHRpcFBvcHVwJywgWyckbG9nJywgJyR0b29sdGlwU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJGxvZywgJHRvb2x0aXBTdXBwcmVzc1dhcm5pbmcpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHNjb3BlOiB7IGNvbnRlbnQ6ICdAJywgcGxhY2VtZW50OiAnQCcsIHBvcHVwQ2xhc3M6ICdAJywgYW5pbWF0aW9uOiAnJicsIGlzT3BlbjogJyYnIH0sXG4gICAgdGVtcGxhdGVVcmw6ICd0ZW1wbGF0ZS90b29sdGlwL3Rvb2x0aXAtcG9wdXAuaHRtbCcsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQpIHtcbiAgICAgIGlmICghJHRvb2x0aXBTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgJGxvZy53YXJuKCd0b29sdGlwLXBvcHVwIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliLXRvb2x0aXAtcG9wdXAgaW5zdGVhZC4nKTtcbiAgICAgIH1cblxuICAgICAgZWxlbWVudC5hZGRDbGFzcygndG9vbHRpcCcpO1xuICAgIH1cbiAgfTtcbn1dKVxuXG4uZGlyZWN0aXZlKCd0b29sdGlwJywgWyckdG9vbHRpcCcsIGZ1bmN0aW9uKCR0b29sdGlwKSB7XG4gIHJldHVybiAkdG9vbHRpcCgndG9vbHRpcCcsICd0b29sdGlwJywgJ21vdXNlZW50ZXInKTtcbn1dKVxuXG4uZGlyZWN0aXZlKCd0b29sdGlwVGVtcGxhdGVQb3B1cCcsIFsnJGxvZycsICckdG9vbHRpcFN1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRsb2csICR0b29sdGlwU3VwcHJlc3NXYXJuaW5nKSB7XG4gIHJldHVybiB7XG4gICAgcmVwbGFjZTogdHJ1ZSxcbiAgICBzY29wZTogeyBjb250ZW50RXhwOiAnJicsIHBsYWNlbWVudDogJ0AnLCBwb3B1cENsYXNzOiAnQCcsIGFuaW1hdGlvbjogJyYnLCBpc09wZW46ICcmJyxcbiAgICAgIG9yaWdpblNjb3BlOiAnJicgfSxcbiAgICB0ZW1wbGF0ZVVybDogJ3RlbXBsYXRlL3Rvb2x0aXAvdG9vbHRpcC10ZW1wbGF0ZS1wb3B1cC5odG1sJyxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCkge1xuICAgICAgaWYgKCEkdG9vbHRpcFN1cHByZXNzV2FybmluZykge1xuICAgICAgICAkbG9nLndhcm4oJ3Rvb2x0aXAtdGVtcGxhdGUtcG9wdXAgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSB1aWItdG9vbHRpcC10ZW1wbGF0ZS1wb3B1cCBpbnN0ZWFkLicpO1xuICAgICAgfVxuXG4gICAgICBlbGVtZW50LmFkZENsYXNzKCd0b29sdGlwJyk7XG4gICAgfVxuICB9O1xufV0pXG5cbi5kaXJlY3RpdmUoJ3Rvb2x0aXBUZW1wbGF0ZScsIFsnJHRvb2x0aXAnLCBmdW5jdGlvbigkdG9vbHRpcCkge1xuICByZXR1cm4gJHRvb2x0aXAoJ3Rvb2x0aXBUZW1wbGF0ZScsICd0b29sdGlwJywgJ21vdXNlZW50ZXInLCB7XG4gICAgdXNlQ29udGVudEV4cDogdHJ1ZVxuICB9KTtcbn1dKVxuXG4uZGlyZWN0aXZlKCd0b29sdGlwSHRtbFBvcHVwJywgWyckbG9nJywgJyR0b29sdGlwU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJGxvZywgJHRvb2x0aXBTdXBwcmVzc1dhcm5pbmcpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHNjb3BlOiB7IGNvbnRlbnRFeHA6ICcmJywgcGxhY2VtZW50OiAnQCcsIHBvcHVwQ2xhc3M6ICdAJywgYW5pbWF0aW9uOiAnJicsIGlzT3BlbjogJyYnIH0sXG4gICAgdGVtcGxhdGVVcmw6ICd0ZW1wbGF0ZS90b29sdGlwL3Rvb2x0aXAtaHRtbC1wb3B1cC5odG1sJyxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCkge1xuICAgICAgaWYgKCEkdG9vbHRpcFN1cHByZXNzV2FybmluZykge1xuICAgICAgICAkbG9nLndhcm4oJ3Rvb2x0aXAtaHRtbC1wb3B1cCBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi10b29sdGlwLWh0bWwtcG9wdXAgaW5zdGVhZC4nKTtcbiAgICAgIH1cblxuICAgICAgZWxlbWVudC5hZGRDbGFzcygndG9vbHRpcCcpO1xuICAgIH1cbiAgfTtcbn1dKVxuXG4uZGlyZWN0aXZlKCd0b29sdGlwSHRtbCcsIFsnJHRvb2x0aXAnLCBmdW5jdGlvbigkdG9vbHRpcCkge1xuICByZXR1cm4gJHRvb2x0aXAoJ3Rvb2x0aXBIdG1sJywgJ3Rvb2x0aXAnLCAnbW91c2VlbnRlcicsIHtcbiAgICB1c2VDb250ZW50RXhwOiB0cnVlXG4gIH0pO1xufV0pO1xuXG4vKipcbiAqIFRoZSBmb2xsb3dpbmcgZmVhdHVyZXMgYXJlIHN0aWxsIG91dHN0YW5kaW5nOiBwb3B1cCBkZWxheSwgYW5pbWF0aW9uIGFzIGFcbiAqIGZ1bmN0aW9uLCBwbGFjZW1lbnQgYXMgYSBmdW5jdGlvbiwgaW5zaWRlLCBzdXBwb3J0IGZvciBtb3JlIHRyaWdnZXJzIHRoYW5cbiAqIGp1c3QgbW91c2UgZW50ZXIvbGVhdmUsIGFuZCBzZWxlY3RvciBkZWxlZ2F0YXRpb24uXG4gKi9cbmFuZ3VsYXIubW9kdWxlKCd1aS5ib290c3RyYXAucG9wb3ZlcicsIFsndWkuYm9vdHN0cmFwLnRvb2x0aXAnXSlcblxuLmRpcmVjdGl2ZSgndWliUG9wb3ZlclRlbXBsYXRlUG9wdXAnLCBmdW5jdGlvbigpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHNjb3BlOiB7IHRpdGxlOiAnQCcsIGNvbnRlbnRFeHA6ICcmJywgcGxhY2VtZW50OiAnQCcsIHBvcHVwQ2xhc3M6ICdAJywgYW5pbWF0aW9uOiAnJicsIGlzT3BlbjogJyYnLFxuICAgICAgb3JpZ2luU2NvcGU6ICcmJyB9LFxuICAgIHRlbXBsYXRlVXJsOiAndGVtcGxhdGUvcG9wb3Zlci9wb3BvdmVyLXRlbXBsYXRlLmh0bWwnLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50KSB7XG4gICAgICBlbGVtZW50LmFkZENsYXNzKCdwb3BvdmVyJyk7XG4gICAgfVxuICB9O1xufSlcblxuLmRpcmVjdGl2ZSgndWliUG9wb3ZlclRlbXBsYXRlJywgWyckdWliVG9vbHRpcCcsIGZ1bmN0aW9uKCR1aWJUb29sdGlwKSB7XG4gIHJldHVybiAkdWliVG9vbHRpcCgndWliUG9wb3ZlclRlbXBsYXRlJywgJ3BvcG92ZXInLCAnY2xpY2snLCB7XG4gICAgdXNlQ29udGVudEV4cDogdHJ1ZVxuICB9KTtcbn1dKVxuXG4uZGlyZWN0aXZlKCd1aWJQb3BvdmVySHRtbFBvcHVwJywgZnVuY3Rpb24oKSB7XG4gIHJldHVybiB7XG4gICAgcmVwbGFjZTogdHJ1ZSxcbiAgICBzY29wZTogeyBjb250ZW50RXhwOiAnJicsIHRpdGxlOiAnQCcsIHBsYWNlbWVudDogJ0AnLCBwb3B1cENsYXNzOiAnQCcsIGFuaW1hdGlvbjogJyYnLCBpc09wZW46ICcmJyB9LFxuICAgIHRlbXBsYXRlVXJsOiAndGVtcGxhdGUvcG9wb3Zlci9wb3BvdmVyLWh0bWwuaHRtbCcsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQpIHtcbiAgICAgIGVsZW1lbnQuYWRkQ2xhc3MoJ3BvcG92ZXInKTtcbiAgICB9XG4gIH07XG59KVxuXG4uZGlyZWN0aXZlKCd1aWJQb3BvdmVySHRtbCcsIFsnJHVpYlRvb2x0aXAnLCBmdW5jdGlvbigkdWliVG9vbHRpcCkge1xuICByZXR1cm4gJHVpYlRvb2x0aXAoJ3VpYlBvcG92ZXJIdG1sJywgJ3BvcG92ZXInLCAnY2xpY2snLCB7XG4gICAgdXNlQ29udGVudEV4cDogdHJ1ZVxuICB9KTtcbn1dKVxuXG4uZGlyZWN0aXZlKCd1aWJQb3BvdmVyUG9wdXAnLCBmdW5jdGlvbigpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHNjb3BlOiB7IHRpdGxlOiAnQCcsIGNvbnRlbnQ6ICdAJywgcGxhY2VtZW50OiAnQCcsIHBvcHVwQ2xhc3M6ICdAJywgYW5pbWF0aW9uOiAnJicsIGlzT3BlbjogJyYnIH0sXG4gICAgdGVtcGxhdGVVcmw6ICd0ZW1wbGF0ZS9wb3BvdmVyL3BvcG92ZXIuaHRtbCcsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQpIHtcbiAgICAgIGVsZW1lbnQuYWRkQ2xhc3MoJ3BvcG92ZXInKTtcbiAgICB9XG4gIH07XG59KVxuXG4uZGlyZWN0aXZlKCd1aWJQb3BvdmVyJywgWyckdWliVG9vbHRpcCcsIGZ1bmN0aW9uKCR1aWJUb29sdGlwKSB7XG4gIHJldHVybiAkdWliVG9vbHRpcCgndWliUG9wb3ZlcicsICdwb3BvdmVyJywgJ2NsaWNrJyk7XG59XSk7XG5cbi8qIERlcHJlY2F0ZWQgcG9wb3ZlciBiZWxvdyAqL1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLnBvcG92ZXInKVxuXG4udmFsdWUoJyRwb3BvdmVyU3VwcHJlc3NXYXJuaW5nJywgZmFsc2UpXG5cbi5kaXJlY3RpdmUoJ3BvcG92ZXJUZW1wbGF0ZVBvcHVwJywgWyckbG9nJywgJyRwb3BvdmVyU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJGxvZywgJHBvcG92ZXJTdXBwcmVzc1dhcm5pbmcpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHNjb3BlOiB7IHRpdGxlOiAnQCcsIGNvbnRlbnRFeHA6ICcmJywgcGxhY2VtZW50OiAnQCcsIHBvcHVwQ2xhc3M6ICdAJywgYW5pbWF0aW9uOiAnJicsIGlzT3BlbjogJyYnLFxuICAgICAgb3JpZ2luU2NvcGU6ICcmJyB9LFxuICAgIHRlbXBsYXRlVXJsOiAndGVtcGxhdGUvcG9wb3Zlci9wb3BvdmVyLXRlbXBsYXRlLmh0bWwnLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50KSB7XG4gICAgICBpZiAoISRwb3BvdmVyU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICRsb2cud2FybigncG9wb3Zlci10ZW1wbGF0ZS1wb3B1cCBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi1wb3BvdmVyLXRlbXBsYXRlLXBvcHVwIGluc3RlYWQuJyk7XG4gICAgICB9XG5cbiAgICAgIGVsZW1lbnQuYWRkQ2xhc3MoJ3BvcG92ZXInKTtcbiAgICB9XG4gIH07XG59XSlcblxuLmRpcmVjdGl2ZSgncG9wb3ZlclRlbXBsYXRlJywgWyckdG9vbHRpcCcsIGZ1bmN0aW9uKCR0b29sdGlwKSB7XG4gIHJldHVybiAkdG9vbHRpcCgncG9wb3ZlclRlbXBsYXRlJywgJ3BvcG92ZXInLCAnY2xpY2snLCB7XG4gICAgdXNlQ29udGVudEV4cDogdHJ1ZVxuICB9KTtcbn1dKVxuXG4uZGlyZWN0aXZlKCdwb3BvdmVySHRtbFBvcHVwJywgWyckbG9nJywgJyRwb3BvdmVyU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJGxvZywgJHBvcG92ZXJTdXBwcmVzc1dhcm5pbmcpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHNjb3BlOiB7IGNvbnRlbnRFeHA6ICcmJywgdGl0bGU6ICdAJywgcGxhY2VtZW50OiAnQCcsIHBvcHVwQ2xhc3M6ICdAJywgYW5pbWF0aW9uOiAnJicsIGlzT3BlbjogJyYnIH0sXG4gICAgdGVtcGxhdGVVcmw6ICd0ZW1wbGF0ZS9wb3BvdmVyL3BvcG92ZXItaHRtbC5odG1sJyxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCkge1xuICAgICAgaWYgKCEkcG9wb3ZlclN1cHByZXNzV2FybmluZykge1xuICAgICAgICAkbG9nLndhcm4oJ3BvcG92ZXItaHRtbC1wb3B1cCBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi1wb3BvdmVyLWh0bWwtcG9wdXAgaW5zdGVhZC4nKTtcbiAgICAgIH1cblxuICAgICAgZWxlbWVudC5hZGRDbGFzcygncG9wb3ZlcicpO1xuICAgIH1cbiAgfTtcbn1dKVxuXG4uZGlyZWN0aXZlKCdwb3BvdmVySHRtbCcsIFsnJHRvb2x0aXAnLCBmdW5jdGlvbigkdG9vbHRpcCkge1xuICByZXR1cm4gJHRvb2x0aXAoJ3BvcG92ZXJIdG1sJywgJ3BvcG92ZXInLCAnY2xpY2snLCB7XG4gICAgdXNlQ29udGVudEV4cDogdHJ1ZVxuICB9KTtcbn1dKVxuXG4uZGlyZWN0aXZlKCdwb3BvdmVyUG9wdXAnLCBbJyRsb2cnLCAnJHBvcG92ZXJTdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkbG9nLCAkcG9wb3ZlclN1cHByZXNzV2FybmluZykge1xuICByZXR1cm4ge1xuICAgIHJlcGxhY2U6IHRydWUsXG4gICAgc2NvcGU6IHsgdGl0bGU6ICdAJywgY29udGVudDogJ0AnLCBwbGFjZW1lbnQ6ICdAJywgcG9wdXBDbGFzczogJ0AnLCBhbmltYXRpb246ICcmJywgaXNPcGVuOiAnJicgfSxcbiAgICB0ZW1wbGF0ZVVybDogJ3RlbXBsYXRlL3BvcG92ZXIvcG9wb3Zlci5odG1sJyxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCkge1xuICAgICAgaWYgKCEkcG9wb3ZlclN1cHByZXNzV2FybmluZykge1xuICAgICAgICAkbG9nLndhcm4oJ3BvcG92ZXItcG9wdXAgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSB1aWItcG9wb3Zlci1wb3B1cCBpbnN0ZWFkLicpO1xuICAgICAgfVxuXG4gICAgICBlbGVtZW50LmFkZENsYXNzKCdwb3BvdmVyJyk7XG4gICAgfVxuICB9O1xufV0pXG5cbi5kaXJlY3RpdmUoJ3BvcG92ZXInLCBbJyR0b29sdGlwJywgZnVuY3Rpb24oJHRvb2x0aXApIHtcblxuICByZXR1cm4gJHRvb2x0aXAoJ3BvcG92ZXInLCAncG9wb3ZlcicsICdjbGljaycpO1xufV0pO1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLnByb2dyZXNzYmFyJywgW10pXG5cbi5jb25zdGFudCgndWliUHJvZ3Jlc3NDb25maWcnLCB7XG4gIGFuaW1hdGU6IHRydWUsXG4gIG1heDogMTAwXG59KVxuXG4uY29udHJvbGxlcignVWliUHJvZ3Jlc3NDb250cm9sbGVyJywgWyckc2NvcGUnLCAnJGF0dHJzJywgJ3VpYlByb2dyZXNzQ29uZmlnJywgZnVuY3Rpb24oJHNjb3BlLCAkYXR0cnMsIHByb2dyZXNzQ29uZmlnKSB7XG4gIHZhciBzZWxmID0gdGhpcyxcbiAgICAgIGFuaW1hdGUgPSBhbmd1bGFyLmlzRGVmaW5lZCgkYXR0cnMuYW5pbWF0ZSkgPyAkc2NvcGUuJHBhcmVudC4kZXZhbCgkYXR0cnMuYW5pbWF0ZSkgOiBwcm9ncmVzc0NvbmZpZy5hbmltYXRlO1xuXG4gIHRoaXMuYmFycyA9IFtdO1xuICAkc2NvcGUubWF4ID0gYW5ndWxhci5pc0RlZmluZWQoJHNjb3BlLm1heCkgPyAkc2NvcGUubWF4IDogcHJvZ3Jlc3NDb25maWcubWF4O1xuXG4gIHRoaXMuYWRkQmFyID0gZnVuY3Rpb24oYmFyLCBlbGVtZW50LCBhdHRycykge1xuICAgIGlmICghYW5pbWF0ZSkge1xuICAgICAgZWxlbWVudC5jc3Moeyd0cmFuc2l0aW9uJzogJ25vbmUnfSk7XG4gICAgfVxuXG4gICAgdGhpcy5iYXJzLnB1c2goYmFyKTtcblxuICAgIGJhci5tYXggPSAkc2NvcGUubWF4O1xuICAgIGJhci50aXRsZSA9IGF0dHJzICYmIGFuZ3VsYXIuaXNEZWZpbmVkKGF0dHJzLnRpdGxlKSA/IGF0dHJzLnRpdGxlIDogJ3Byb2dyZXNzYmFyJztcblxuICAgIGJhci4kd2F0Y2goJ3ZhbHVlJywgZnVuY3Rpb24odmFsdWUpIHtcbiAgICAgIGJhci5yZWNhbGN1bGF0ZVBlcmNlbnRhZ2UoKTtcbiAgICB9KTtcblxuICAgIGJhci5yZWNhbGN1bGF0ZVBlcmNlbnRhZ2UgPSBmdW5jdGlvbigpIHtcbiAgICAgIHZhciB0b3RhbFBlcmNlbnRhZ2UgPSBzZWxmLmJhcnMucmVkdWNlKGZ1bmN0aW9uKHRvdGFsLCBiYXIpIHtcbiAgICAgICAgYmFyLnBlcmNlbnQgPSArKDEwMCAqIGJhci52YWx1ZSAvIGJhci5tYXgpLnRvRml4ZWQoMik7XG4gICAgICAgIHJldHVybiB0b3RhbCArIGJhci5wZXJjZW50O1xuICAgICAgfSwgMCk7XG5cbiAgICAgIGlmICh0b3RhbFBlcmNlbnRhZ2UgPiAxMDApIHtcbiAgICAgICAgYmFyLnBlcmNlbnQgLT0gdG90YWxQZXJjZW50YWdlIC0gMTAwO1xuICAgICAgfVxuICAgIH07XG5cbiAgICBiYXIuJG9uKCckZGVzdHJveScsIGZ1bmN0aW9uKCkge1xuICAgICAgZWxlbWVudCA9IG51bGw7XG4gICAgICBzZWxmLnJlbW92ZUJhcihiYXIpO1xuICAgIH0pO1xuICB9O1xuXG4gIHRoaXMucmVtb3ZlQmFyID0gZnVuY3Rpb24oYmFyKSB7XG4gICAgdGhpcy5iYXJzLnNwbGljZSh0aGlzLmJhcnMuaW5kZXhPZihiYXIpLCAxKTtcbiAgICB0aGlzLmJhcnMuZm9yRWFjaChmdW5jdGlvbiAoYmFyKSB7XG4gICAgICBiYXIucmVjYWxjdWxhdGVQZXJjZW50YWdlKCk7XG4gICAgfSk7XG4gIH07XG5cbiAgJHNjb3BlLiR3YXRjaCgnbWF4JywgZnVuY3Rpb24obWF4KSB7XG4gICAgc2VsZi5iYXJzLmZvckVhY2goZnVuY3Rpb24oYmFyKSB7XG4gICAgICBiYXIubWF4ID0gJHNjb3BlLm1heDtcbiAgICAgIGJhci5yZWNhbGN1bGF0ZVBlcmNlbnRhZ2UoKTtcbiAgICB9KTtcbiAgfSk7XG59XSlcblxuLmRpcmVjdGl2ZSgndWliUHJvZ3Jlc3MnLCBmdW5jdGlvbigpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHRyYW5zY2x1ZGU6IHRydWUsXG4gICAgY29udHJvbGxlcjogJ1VpYlByb2dyZXNzQ29udHJvbGxlcicsXG4gICAgcmVxdWlyZTogJ3VpYlByb2dyZXNzJyxcbiAgICBzY29wZToge1xuICAgICAgbWF4OiAnPT8nXG4gICAgfSxcbiAgICB0ZW1wbGF0ZVVybDogJ3RlbXBsYXRlL3Byb2dyZXNzYmFyL3Byb2dyZXNzLmh0bWwnXG4gIH07XG59KVxuXG4uZGlyZWN0aXZlKCd1aWJCYXInLCBmdW5jdGlvbigpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHRyYW5zY2x1ZGU6IHRydWUsXG4gICAgcmVxdWlyZTogJ151aWJQcm9ncmVzcycsXG4gICAgc2NvcGU6IHtcbiAgICAgIHZhbHVlOiAnPScsXG4gICAgICB0eXBlOiAnQCdcbiAgICB9LFxuICAgIHRlbXBsYXRlVXJsOiAndGVtcGxhdGUvcHJvZ3Jlc3NiYXIvYmFyLmh0bWwnLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgcHJvZ3Jlc3NDdHJsKSB7XG4gICAgICBwcm9ncmVzc0N0cmwuYWRkQmFyKHNjb3BlLCBlbGVtZW50LCBhdHRycyk7XG4gICAgfVxuICB9O1xufSlcblxuLmRpcmVjdGl2ZSgndWliUHJvZ3Jlc3NiYXInLCBmdW5jdGlvbigpIHtcbiAgcmV0dXJuIHtcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHRyYW5zY2x1ZGU6IHRydWUsXG4gICAgY29udHJvbGxlcjogJ1VpYlByb2dyZXNzQ29udHJvbGxlcicsXG4gICAgc2NvcGU6IHtcbiAgICAgIHZhbHVlOiAnPScsXG4gICAgICBtYXg6ICc9PycsXG4gICAgICB0eXBlOiAnQCdcbiAgICB9LFxuICAgIHRlbXBsYXRlVXJsOiAndGVtcGxhdGUvcHJvZ3Jlc3NiYXIvcHJvZ3Jlc3NiYXIuaHRtbCcsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHJzLCBwcm9ncmVzc0N0cmwpIHtcbiAgICAgIHByb2dyZXNzQ3RybC5hZGRCYXIoc2NvcGUsIGFuZ3VsYXIuZWxlbWVudChlbGVtZW50LmNoaWxkcmVuKClbMF0pLCB7dGl0bGU6IGF0dHJzLnRpdGxlfSk7XG4gICAgfVxuICB9O1xufSk7XG5cbi8qIERlcHJlY2F0ZWQgcHJvZ3Jlc3NiYXIgYmVsb3cgKi9cblxuYW5ndWxhci5tb2R1bGUoJ3VpLmJvb3RzdHJhcC5wcm9ncmVzc2JhcicpXG5cbi52YWx1ZSgnJHByb2dyZXNzU3VwcHJlc3NXYXJuaW5nJywgZmFsc2UpXG5cbi5jb250cm9sbGVyKCdQcm9ncmVzc0NvbnRyb2xsZXInLCBbJyRzY29wZScsICckYXR0cnMnLCAndWliUHJvZ3Jlc3NDb25maWcnLCAnJGxvZycsICckcHJvZ3Jlc3NTdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkc2NvcGUsICRhdHRycywgcHJvZ3Jlc3NDb25maWcsICRsb2csICRwcm9ncmVzc1N1cHByZXNzV2FybmluZykge1xuICBpZiAoISRwcm9ncmVzc1N1cHByZXNzV2FybmluZykge1xuICAgICRsb2cud2FybignUHJvZ3Jlc3NDb250cm9sbGVyIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgVWliUHJvZ3Jlc3NDb250cm9sbGVyIGluc3RlYWQuJyk7XG4gIH1cblxuICB2YXIgc2VsZiA9IHRoaXMsXG4gICAgYW5pbWF0ZSA9IGFuZ3VsYXIuaXNEZWZpbmVkKCRhdHRycy5hbmltYXRlKSA/ICRzY29wZS4kcGFyZW50LiRldmFsKCRhdHRycy5hbmltYXRlKSA6IHByb2dyZXNzQ29uZmlnLmFuaW1hdGU7XG5cbiAgdGhpcy5iYXJzID0gW107XG4gICRzY29wZS5tYXggPSBhbmd1bGFyLmlzRGVmaW5lZCgkc2NvcGUubWF4KSA/ICRzY29wZS5tYXggOiBwcm9ncmVzc0NvbmZpZy5tYXg7XG5cbiAgdGhpcy5hZGRCYXIgPSBmdW5jdGlvbihiYXIsIGVsZW1lbnQsIGF0dHJzKSB7XG4gICAgaWYgKCFhbmltYXRlKSB7XG4gICAgICBlbGVtZW50LmNzcyh7J3RyYW5zaXRpb24nOiAnbm9uZSd9KTtcbiAgICB9XG5cbiAgICB0aGlzLmJhcnMucHVzaChiYXIpO1xuXG4gICAgYmFyLm1heCA9ICRzY29wZS5tYXg7XG4gICAgYmFyLnRpdGxlID0gYXR0cnMgJiYgYW5ndWxhci5pc0RlZmluZWQoYXR0cnMudGl0bGUpID8gYXR0cnMudGl0bGUgOiAncHJvZ3Jlc3NiYXInO1xuXG4gICAgYmFyLiR3YXRjaCgndmFsdWUnLCBmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgYmFyLnJlY2FsY3VsYXRlUGVyY2VudGFnZSgpO1xuICAgIH0pO1xuXG4gICAgYmFyLnJlY2FsY3VsYXRlUGVyY2VudGFnZSA9IGZ1bmN0aW9uKCkge1xuICAgICAgYmFyLnBlcmNlbnQgPSArKDEwMCAqIGJhci52YWx1ZSAvIGJhci5tYXgpLnRvRml4ZWQoMik7XG5cbiAgICAgIHZhciB0b3RhbFBlcmNlbnRhZ2UgPSBzZWxmLmJhcnMucmVkdWNlKGZ1bmN0aW9uKHRvdGFsLCBiYXIpIHtcbiAgICAgICAgcmV0dXJuIHRvdGFsICsgYmFyLnBlcmNlbnQ7XG4gICAgICB9LCAwKTtcblxuICAgICAgaWYgKHRvdGFsUGVyY2VudGFnZSA+IDEwMCkge1xuICAgICAgICBiYXIucGVyY2VudCAtPSB0b3RhbFBlcmNlbnRhZ2UgLSAxMDA7XG4gICAgICB9XG4gICAgfTtcblxuICAgIGJhci4kb24oJyRkZXN0cm95JywgZnVuY3Rpb24oKSB7XG4gICAgICBlbGVtZW50ID0gbnVsbDtcbiAgICAgIHNlbGYucmVtb3ZlQmFyKGJhcik7XG4gICAgfSk7XG4gIH07XG5cbiAgdGhpcy5yZW1vdmVCYXIgPSBmdW5jdGlvbihiYXIpIHtcbiAgICB0aGlzLmJhcnMuc3BsaWNlKHRoaXMuYmFycy5pbmRleE9mKGJhciksIDEpO1xuICB9O1xuXG4gICRzY29wZS4kd2F0Y2goJ21heCcsIGZ1bmN0aW9uKG1heCkge1xuICAgIHNlbGYuYmFycy5mb3JFYWNoKGZ1bmN0aW9uKGJhcikge1xuICAgICAgYmFyLm1heCA9ICRzY29wZS5tYXg7XG4gICAgICBiYXIucmVjYWxjdWxhdGVQZXJjZW50YWdlKCk7XG4gICAgfSk7XG4gIH0pO1xufV0pXG5cbi5kaXJlY3RpdmUoJ3Byb2dyZXNzJywgWyckbG9nJywgJyRwcm9ncmVzc1N1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRsb2csICRwcm9ncmVzc1N1cHByZXNzV2FybmluZykge1xuICByZXR1cm4ge1xuICAgIHJlcGxhY2U6IHRydWUsXG4gICAgdHJhbnNjbHVkZTogdHJ1ZSxcbiAgICBjb250cm9sbGVyOiAnUHJvZ3Jlc3NDb250cm9sbGVyJyxcbiAgICByZXF1aXJlOiAncHJvZ3Jlc3MnLFxuICAgIHNjb3BlOiB7XG4gICAgICBtYXg6ICc9PycsXG4gICAgICB0aXRsZTogJ0A/J1xuICAgIH0sXG4gICAgdGVtcGxhdGVVcmw6ICd0ZW1wbGF0ZS9wcm9ncmVzc2Jhci9wcm9ncmVzcy5odG1sJyxcbiAgICBsaW5rOiBmdW5jdGlvbigpIHtcbiAgICAgIGlmICghJHByb2dyZXNzU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICRsb2cud2FybigncHJvZ3Jlc3MgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSB1aWItcHJvZ3Jlc3MgaW5zdGVhZC4nKTtcbiAgICAgIH1cbiAgICB9XG4gIH07XG59XSlcblxuLmRpcmVjdGl2ZSgnYmFyJywgWyckbG9nJywgJyRwcm9ncmVzc1N1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRsb2csICRwcm9ncmVzc1N1cHByZXNzV2FybmluZykge1xuICByZXR1cm4ge1xuICAgIHJlcGxhY2U6IHRydWUsXG4gICAgdHJhbnNjbHVkZTogdHJ1ZSxcbiAgICByZXF1aXJlOiAnXnByb2dyZXNzJyxcbiAgICBzY29wZToge1xuICAgICAgdmFsdWU6ICc9JyxcbiAgICAgIHR5cGU6ICdAJ1xuICAgIH0sXG4gICAgdGVtcGxhdGVVcmw6ICd0ZW1wbGF0ZS9wcm9ncmVzc2Jhci9iYXIuaHRtbCcsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHJzLCBwcm9ncmVzc0N0cmwpIHtcbiAgICAgIGlmICghJHByb2dyZXNzU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICRsb2cud2FybignYmFyIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliLWJhciBpbnN0ZWFkLicpO1xuICAgICAgfVxuICAgICAgcHJvZ3Jlc3NDdHJsLmFkZEJhcihzY29wZSwgZWxlbWVudCk7XG4gICAgfVxuICB9O1xufV0pXG5cbi5kaXJlY3RpdmUoJ3Byb2dyZXNzYmFyJywgWyckbG9nJywgJyRwcm9ncmVzc1N1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRsb2csICRwcm9ncmVzc1N1cHByZXNzV2FybmluZykge1xuICByZXR1cm4ge1xuICAgIHJlcGxhY2U6IHRydWUsXG4gICAgdHJhbnNjbHVkZTogdHJ1ZSxcbiAgICBjb250cm9sbGVyOiAnUHJvZ3Jlc3NDb250cm9sbGVyJyxcbiAgICBzY29wZToge1xuICAgICAgdmFsdWU6ICc9JyxcbiAgICAgIG1heDogJz0/JyxcbiAgICAgIHR5cGU6ICdAJ1xuICAgIH0sXG4gICAgdGVtcGxhdGVVcmw6ICd0ZW1wbGF0ZS9wcm9ncmVzc2Jhci9wcm9ncmVzc2Jhci5odG1sJyxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMsIHByb2dyZXNzQ3RybCkge1xuICAgICAgaWYgKCEkcHJvZ3Jlc3NTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgJGxvZy53YXJuKCdwcm9ncmVzc2JhciBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi1wcm9ncmVzc2JhciBpbnN0ZWFkLicpO1xuICAgICAgfVxuICAgICAgcHJvZ3Jlc3NDdHJsLmFkZEJhcihzY29wZSwgYW5ndWxhci5lbGVtZW50KGVsZW1lbnQuY2hpbGRyZW4oKVswXSksIHt0aXRsZTogYXR0cnMudGl0bGV9KTtcbiAgICB9XG4gIH07XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKCd1aS5ib290c3RyYXAucmF0aW5nJywgW10pXG5cbi5jb25zdGFudCgndWliUmF0aW5nQ29uZmlnJywge1xuICBtYXg6IDUsXG4gIHN0YXRlT246IG51bGwsXG4gIHN0YXRlT2ZmOiBudWxsLFxuICB0aXRsZXMgOiBbJ29uZScsICd0d28nLCAndGhyZWUnLCAnZm91cicsICdmaXZlJ11cbn0pXG5cbi5jb250cm9sbGVyKCdVaWJSYXRpbmdDb250cm9sbGVyJywgWyckc2NvcGUnLCAnJGF0dHJzJywgJ3VpYlJhdGluZ0NvbmZpZycsIGZ1bmN0aW9uKCRzY29wZSwgJGF0dHJzLCByYXRpbmdDb25maWcpIHtcbiAgdmFyIG5nTW9kZWxDdHJsICA9IHsgJHNldFZpZXdWYWx1ZTogYW5ndWxhci5ub29wIH07XG5cbiAgdGhpcy5pbml0ID0gZnVuY3Rpb24obmdNb2RlbEN0cmxfKSB7XG4gICAgbmdNb2RlbEN0cmwgPSBuZ01vZGVsQ3RybF87XG4gICAgbmdNb2RlbEN0cmwuJHJlbmRlciA9IHRoaXMucmVuZGVyO1xuXG4gICAgbmdNb2RlbEN0cmwuJGZvcm1hdHRlcnMucHVzaChmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgaWYgKGFuZ3VsYXIuaXNOdW1iZXIodmFsdWUpICYmIHZhbHVlIDw8IDAgIT09IHZhbHVlKSB7XG4gICAgICAgIHZhbHVlID0gTWF0aC5yb3VuZCh2YWx1ZSk7XG4gICAgICB9XG4gICAgICByZXR1cm4gdmFsdWU7XG4gICAgfSk7XG5cbiAgICB0aGlzLnN0YXRlT24gPSBhbmd1bGFyLmlzRGVmaW5lZCgkYXR0cnMuc3RhdGVPbikgPyAkc2NvcGUuJHBhcmVudC4kZXZhbCgkYXR0cnMuc3RhdGVPbikgOiByYXRpbmdDb25maWcuc3RhdGVPbjtcbiAgICB0aGlzLnN0YXRlT2ZmID0gYW5ndWxhci5pc0RlZmluZWQoJGF0dHJzLnN0YXRlT2ZmKSA/ICRzY29wZS4kcGFyZW50LiRldmFsKCRhdHRycy5zdGF0ZU9mZikgOiByYXRpbmdDb25maWcuc3RhdGVPZmY7XG4gICAgdmFyIHRtcFRpdGxlcyA9IGFuZ3VsYXIuaXNEZWZpbmVkKCRhdHRycy50aXRsZXMpICA/ICRzY29wZS4kcGFyZW50LiRldmFsKCRhdHRycy50aXRsZXMpIDogcmF0aW5nQ29uZmlnLnRpdGxlcyA7XG4gICAgdGhpcy50aXRsZXMgPSBhbmd1bGFyLmlzQXJyYXkodG1wVGl0bGVzKSAmJiB0bXBUaXRsZXMubGVuZ3RoID4gMCA/XG4gICAgICB0bXBUaXRsZXMgOiByYXRpbmdDb25maWcudGl0bGVzO1xuXG4gICAgdmFyIHJhdGluZ1N0YXRlcyA9IGFuZ3VsYXIuaXNEZWZpbmVkKCRhdHRycy5yYXRpbmdTdGF0ZXMpID9cbiAgICAgICRzY29wZS4kcGFyZW50LiRldmFsKCRhdHRycy5yYXRpbmdTdGF0ZXMpIDpcbiAgICAgIG5ldyBBcnJheShhbmd1bGFyLmlzRGVmaW5lZCgkYXR0cnMubWF4KSA/ICRzY29wZS4kcGFyZW50LiRldmFsKCRhdHRycy5tYXgpIDogcmF0aW5nQ29uZmlnLm1heCk7XG4gICAgJHNjb3BlLnJhbmdlID0gdGhpcy5idWlsZFRlbXBsYXRlT2JqZWN0cyhyYXRpbmdTdGF0ZXMpO1xuICB9O1xuXG4gIHRoaXMuYnVpbGRUZW1wbGF0ZU9iamVjdHMgPSBmdW5jdGlvbihzdGF0ZXMpIHtcbiAgICBmb3IgKHZhciBpID0gMCwgbiA9IHN0YXRlcy5sZW5ndGg7IGkgPCBuOyBpKyspIHtcbiAgICAgIHN0YXRlc1tpXSA9IGFuZ3VsYXIuZXh0ZW5kKHsgaW5kZXg6IGkgfSwgeyBzdGF0ZU9uOiB0aGlzLnN0YXRlT24sIHN0YXRlT2ZmOiB0aGlzLnN0YXRlT2ZmLCB0aXRsZTogdGhpcy5nZXRUaXRsZShpKSB9LCBzdGF0ZXNbaV0pO1xuICAgIH1cbiAgICByZXR1cm4gc3RhdGVzO1xuICB9O1xuXG4gIHRoaXMuZ2V0VGl0bGUgPSBmdW5jdGlvbihpbmRleCkge1xuICAgIGlmIChpbmRleCA+PSB0aGlzLnRpdGxlcy5sZW5ndGgpIHtcbiAgICAgIHJldHVybiBpbmRleCArIDE7XG4gICAgfSBlbHNlIHtcbiAgICAgIHJldHVybiB0aGlzLnRpdGxlc1tpbmRleF07XG4gICAgfVxuICB9O1xuXG4gICRzY29wZS5yYXRlID0gZnVuY3Rpb24odmFsdWUpIHtcbiAgICBpZiAoISRzY29wZS5yZWFkb25seSAmJiB2YWx1ZSA+PSAwICYmIHZhbHVlIDw9ICRzY29wZS5yYW5nZS5sZW5ndGgpIHtcbiAgICAgIG5nTW9kZWxDdHJsLiRzZXRWaWV3VmFsdWUobmdNb2RlbEN0cmwuJHZpZXdWYWx1ZSA9PT0gdmFsdWUgPyAwIDogdmFsdWUpO1xuICAgICAgbmdNb2RlbEN0cmwuJHJlbmRlcigpO1xuICAgIH1cbiAgfTtcblxuICAkc2NvcGUuZW50ZXIgPSBmdW5jdGlvbih2YWx1ZSkge1xuICAgIGlmICghJHNjb3BlLnJlYWRvbmx5KSB7XG4gICAgICAkc2NvcGUudmFsdWUgPSB2YWx1ZTtcbiAgICB9XG4gICAgJHNjb3BlLm9uSG92ZXIoe3ZhbHVlOiB2YWx1ZX0pO1xuICB9O1xuXG4gICRzY29wZS5yZXNldCA9IGZ1bmN0aW9uKCkge1xuICAgICRzY29wZS52YWx1ZSA9IG5nTW9kZWxDdHJsLiR2aWV3VmFsdWU7XG4gICAgJHNjb3BlLm9uTGVhdmUoKTtcbiAgfTtcblxuICAkc2NvcGUub25LZXlkb3duID0gZnVuY3Rpb24oZXZ0KSB7XG4gICAgaWYgKC8oMzd8Mzh8Mzl8NDApLy50ZXN0KGV2dC53aGljaCkpIHtcbiAgICAgIGV2dC5wcmV2ZW50RGVmYXVsdCgpO1xuICAgICAgZXZ0LnN0b3BQcm9wYWdhdGlvbigpO1xuICAgICAgJHNjb3BlLnJhdGUoJHNjb3BlLnZhbHVlICsgKGV2dC53aGljaCA9PT0gMzggfHwgZXZ0LndoaWNoID09PSAzOSA/IDEgOiAtMSkpO1xuICAgIH1cbiAgfTtcblxuICB0aGlzLnJlbmRlciA9IGZ1bmN0aW9uKCkge1xuICAgICRzY29wZS52YWx1ZSA9IG5nTW9kZWxDdHJsLiR2aWV3VmFsdWU7XG4gIH07XG59XSlcblxuLmRpcmVjdGl2ZSgndWliUmF0aW5nJywgZnVuY3Rpb24oKSB7XG4gIHJldHVybiB7XG4gICAgcmVxdWlyZTogWyd1aWJSYXRpbmcnLCAnbmdNb2RlbCddLFxuICAgIHNjb3BlOiB7XG4gICAgICByZWFkb25seTogJz0/JyxcbiAgICAgIG9uSG92ZXI6ICcmJyxcbiAgICAgIG9uTGVhdmU6ICcmJ1xuICAgIH0sXG4gICAgY29udHJvbGxlcjogJ1VpYlJhdGluZ0NvbnRyb2xsZXInLFxuICAgIHRlbXBsYXRlVXJsOiAndGVtcGxhdGUvcmF0aW5nL3JhdGluZy5odG1sJyxcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycywgY3RybHMpIHtcbiAgICAgIHZhciByYXRpbmdDdHJsID0gY3RybHNbMF0sIG5nTW9kZWxDdHJsID0gY3RybHNbMV07XG4gICAgICByYXRpbmdDdHJsLmluaXQobmdNb2RlbEN0cmwpO1xuICAgIH1cbiAgfTtcbn0pO1xuXG4vKiBEZXByZWNhdGVkIHJhdGluZyBiZWxvdyAqL1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLnJhdGluZycpXG5cbi52YWx1ZSgnJHJhdGluZ1N1cHByZXNzV2FybmluZycsIGZhbHNlKVxuXG4uY29udHJvbGxlcignUmF0aW5nQ29udHJvbGxlcicsIFsnJHNjb3BlJywgJyRhdHRycycsICckY29udHJvbGxlcicsICckbG9nJywgJyRyYXRpbmdTdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkc2NvcGUsICRhdHRycywgJGNvbnRyb2xsZXIsICRsb2csICRyYXRpbmdTdXBwcmVzc1dhcm5pbmcpIHtcbiAgaWYgKCEkcmF0aW5nU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgJGxvZy53YXJuKCdSYXRpbmdDb250cm9sbGVyIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgVWliUmF0aW5nQ29udHJvbGxlciBpbnN0ZWFkLicpO1xuICB9XG5cbiAgYW5ndWxhci5leHRlbmQodGhpcywgJGNvbnRyb2xsZXIoJ1VpYlJhdGluZ0NvbnRyb2xsZXInLCB7XG4gICAgJHNjb3BlOiAkc2NvcGUsXG4gICAgJGF0dHJzOiAkYXR0cnNcbiAgfSkpO1xufV0pXG5cbi5kaXJlY3RpdmUoJ3JhdGluZycsIFsnJGxvZycsICckcmF0aW5nU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJGxvZywgJHJhdGluZ1N1cHByZXNzV2FybmluZykge1xuICByZXR1cm4ge1xuICAgIHJlcXVpcmU6IFsncmF0aW5nJywgJ25nTW9kZWwnXSxcbiAgICBzY29wZToge1xuICAgICAgcmVhZG9ubHk6ICc9PycsXG4gICAgICBvbkhvdmVyOiAnJicsXG4gICAgICBvbkxlYXZlOiAnJidcbiAgICB9LFxuICAgIGNvbnRyb2xsZXI6ICdSYXRpbmdDb250cm9sbGVyJyxcbiAgICB0ZW1wbGF0ZVVybDogJ3RlbXBsYXRlL3JhdGluZy9yYXRpbmcuaHRtbCcsXG4gICAgcmVwbGFjZTogdHJ1ZSxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMsIGN0cmxzKSB7XG4gICAgICBpZiAoISRyYXRpbmdTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgJGxvZy53YXJuKCdyYXRpbmcgaXMgbm93IGRlcHJlY2F0ZWQuIFVzZSB1aWItcmF0aW5nIGluc3RlYWQuJyk7XG4gICAgICB9XG4gICAgICB2YXIgcmF0aW5nQ3RybCA9IGN0cmxzWzBdLCBuZ01vZGVsQ3RybCA9IGN0cmxzWzFdO1xuICAgICAgcmF0aW5nQ3RybC5pbml0KG5nTW9kZWxDdHJsKTtcbiAgICB9XG4gIH07XG59XSk7XG5cblxuLyoqXG4gKiBAbmdkb2Mgb3ZlcnZpZXdcbiAqIEBuYW1lIHVpLmJvb3RzdHJhcC50YWJzXG4gKlxuICogQGRlc2NyaXB0aW9uXG4gKiBBbmd1bGFySlMgdmVyc2lvbiBvZiB0aGUgdGFicyBkaXJlY3RpdmUuXG4gKi9cblxuYW5ndWxhci5tb2R1bGUoJ3VpLmJvb3RzdHJhcC50YWJzJywgW10pXG5cbi5jb250cm9sbGVyKCdVaWJUYWJzZXRDb250cm9sbGVyJywgWyckc2NvcGUnLCBmdW5jdGlvbiAoJHNjb3BlKSB7XG4gIHZhciBjdHJsID0gdGhpcyxcbiAgICAgIHRhYnMgPSBjdHJsLnRhYnMgPSAkc2NvcGUudGFicyA9IFtdO1xuXG4gIGN0cmwuc2VsZWN0ID0gZnVuY3Rpb24oc2VsZWN0ZWRUYWIpIHtcbiAgICBhbmd1bGFyLmZvckVhY2godGFicywgZnVuY3Rpb24odGFiKSB7XG4gICAgICBpZiAodGFiLmFjdGl2ZSAmJiB0YWIgIT09IHNlbGVjdGVkVGFiKSB7XG4gICAgICAgIHRhYi5hY3RpdmUgPSBmYWxzZTtcbiAgICAgICAgdGFiLm9uRGVzZWxlY3QoKTtcbiAgICAgICAgc2VsZWN0ZWRUYWIuc2VsZWN0Q2FsbGVkID0gZmFsc2U7XG4gICAgICB9XG4gICAgfSk7XG4gICAgc2VsZWN0ZWRUYWIuYWN0aXZlID0gdHJ1ZTtcbiAgICAvLyBvbmx5IGNhbGwgc2VsZWN0IGlmIGl0IGhhcyBub3QgYWxyZWFkeSBiZWVuIGNhbGxlZFxuICAgIGlmICghc2VsZWN0ZWRUYWIuc2VsZWN0Q2FsbGVkKSB7XG4gICAgICBzZWxlY3RlZFRhYi5vblNlbGVjdCgpO1xuICAgICAgc2VsZWN0ZWRUYWIuc2VsZWN0Q2FsbGVkID0gdHJ1ZTtcbiAgICB9XG4gIH07XG5cbiAgY3RybC5hZGRUYWIgPSBmdW5jdGlvbiBhZGRUYWIodGFiKSB7XG4gICAgdGFicy5wdXNoKHRhYik7XG4gICAgLy8gd2UgY2FuJ3QgcnVuIHRoZSBzZWxlY3QgZnVuY3Rpb24gb24gdGhlIGZpcnN0IHRhYlxuICAgIC8vIHNpbmNlIHRoYXQgd291bGQgc2VsZWN0IGl0IHR3aWNlXG4gICAgaWYgKHRhYnMubGVuZ3RoID09PSAxICYmIHRhYi5hY3RpdmUgIT09IGZhbHNlKSB7XG4gICAgICB0YWIuYWN0aXZlID0gdHJ1ZTtcbiAgICB9IGVsc2UgaWYgKHRhYi5hY3RpdmUpIHtcbiAgICAgIGN0cmwuc2VsZWN0KHRhYik7XG4gICAgfSBlbHNlIHtcbiAgICAgIHRhYi5hY3RpdmUgPSBmYWxzZTtcbiAgICB9XG4gIH07XG5cbiAgY3RybC5yZW1vdmVUYWIgPSBmdW5jdGlvbiByZW1vdmVUYWIodGFiKSB7XG4gICAgdmFyIGluZGV4ID0gdGFicy5pbmRleE9mKHRhYik7XG4gICAgLy9TZWxlY3QgYSBuZXcgdGFiIGlmIHRoZSB0YWIgdG8gYmUgcmVtb3ZlZCBpcyBzZWxlY3RlZCBhbmQgbm90IGRlc3Ryb3llZFxuICAgIGlmICh0YWIuYWN0aXZlICYmIHRhYnMubGVuZ3RoID4gMSAmJiAhZGVzdHJveWVkKSB7XG4gICAgICAvL0lmIHRoaXMgaXMgdGhlIGxhc3QgdGFiLCBzZWxlY3QgdGhlIHByZXZpb3VzIHRhYi4gZWxzZSwgdGhlIG5leHQgdGFiLlxuICAgICAgdmFyIG5ld0FjdGl2ZUluZGV4ID0gaW5kZXggPT0gdGFicy5sZW5ndGggLSAxID8gaW5kZXggLSAxIDogaW5kZXggKyAxO1xuICAgICAgY3RybC5zZWxlY3QodGFic1tuZXdBY3RpdmVJbmRleF0pO1xuICAgIH1cbiAgICB0YWJzLnNwbGljZShpbmRleCwgMSk7XG4gIH07XG5cbiAgdmFyIGRlc3Ryb3llZDtcbiAgJHNjb3BlLiRvbignJGRlc3Ryb3knLCBmdW5jdGlvbigpIHtcbiAgICBkZXN0cm95ZWQgPSB0cnVlO1xuICB9KTtcbn1dKVxuXG4vKipcbiAqIEBuZ2RvYyBkaXJlY3RpdmVcbiAqIEBuYW1lIHVpLmJvb3RzdHJhcC50YWJzLmRpcmVjdGl2ZTp0YWJzZXRcbiAqIEByZXN0cmljdCBFQVxuICpcbiAqIEBkZXNjcmlwdGlvblxuICogVGFic2V0IGlzIHRoZSBvdXRlciBjb250YWluZXIgZm9yIHRoZSB0YWJzIGRpcmVjdGl2ZVxuICpcbiAqIEBwYXJhbSB7Ym9vbGVhbj19IHZlcnRpY2FsIFdoZXRoZXIgb3Igbm90IHRvIHVzZSB2ZXJ0aWNhbCBzdHlsaW5nIGZvciB0aGUgdGFicy5cbiAqIEBwYXJhbSB7Ym9vbGVhbj19IGp1c3RpZmllZCBXaGV0aGVyIG9yIG5vdCB0byB1c2UganVzdGlmaWVkIHN0eWxpbmcgZm9yIHRoZSB0YWJzLlxuICpcbiAqIEBleGFtcGxlXG48ZXhhbXBsZSBtb2R1bGU9XCJ1aS5ib290c3RyYXBcIj5cbiAgPGZpbGUgbmFtZT1cImluZGV4Lmh0bWxcIj5cbiAgICA8dWliLXRhYnNldD5cbiAgICAgIDx1aWItdGFiIGhlYWRpbmc9XCJUYWIgMVwiPjxiPkZpcnN0PC9iPiBDb250ZW50ITwvdWliLXRhYj5cbiAgICAgIDx1aWItdGFiIGhlYWRpbmc9XCJUYWIgMlwiPjxpPlNlY29uZDwvaT4gQ29udGVudCE8L3VpYi10YWI+XG4gICAgPC91aWItdGFic2V0PlxuICAgIDxociAvPlxuICAgIDx1aWItdGFic2V0IHZlcnRpY2FsPVwidHJ1ZVwiPlxuICAgICAgPHVpYi10YWIgaGVhZGluZz1cIlZlcnRpY2FsIFRhYiAxXCI+PGI+Rmlyc3Q8L2I+IFZlcnRpY2FsIENvbnRlbnQhPC91aWItdGFiPlxuICAgICAgPHVpYi10YWIgaGVhZGluZz1cIlZlcnRpY2FsIFRhYiAyXCI+PGk+U2Vjb25kPC9pPiBWZXJ0aWNhbCBDb250ZW50ITwvdWliLXRhYj5cbiAgICA8L3VpYi10YWJzZXQ+XG4gICAgPHVpYi10YWJzZXQganVzdGlmaWVkPVwidHJ1ZVwiPlxuICAgICAgPHVpYi10YWIgaGVhZGluZz1cIkp1c3RpZmllZCBUYWIgMVwiPjxiPkZpcnN0PC9iPiBKdXN0aWZpZWQgQ29udGVudCE8L3VpYi10YWI+XG4gICAgICA8dWliLXRhYiBoZWFkaW5nPVwiSnVzdGlmaWVkIFRhYiAyXCI+PGk+U2Vjb25kPC9pPiBKdXN0aWZpZWQgQ29udGVudCE8L3VpYi10YWI+XG4gICAgPC91aWItdGFic2V0PlxuICA8L2ZpbGU+XG48L2V4YW1wbGU+XG4gKi9cbi5kaXJlY3RpdmUoJ3VpYlRhYnNldCcsIGZ1bmN0aW9uKCkge1xuICByZXR1cm4ge1xuICAgIHJlc3RyaWN0OiAnRUEnLFxuICAgIHRyYW5zY2x1ZGU6IHRydWUsXG4gICAgcmVwbGFjZTogdHJ1ZSxcbiAgICBzY29wZToge1xuICAgICAgdHlwZTogJ0AnXG4gICAgfSxcbiAgICBjb250cm9sbGVyOiAnVWliVGFic2V0Q29udHJvbGxlcicsXG4gICAgdGVtcGxhdGVVcmw6ICd0ZW1wbGF0ZS90YWJzL3RhYnNldC5odG1sJyxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMpIHtcbiAgICAgIHNjb3BlLnZlcnRpY2FsID0gYW5ndWxhci5pc0RlZmluZWQoYXR0cnMudmVydGljYWwpID8gc2NvcGUuJHBhcmVudC4kZXZhbChhdHRycy52ZXJ0aWNhbCkgOiBmYWxzZTtcbiAgICAgIHNjb3BlLmp1c3RpZmllZCA9IGFuZ3VsYXIuaXNEZWZpbmVkKGF0dHJzLmp1c3RpZmllZCkgPyBzY29wZS4kcGFyZW50LiRldmFsKGF0dHJzLmp1c3RpZmllZCkgOiBmYWxzZTtcbiAgICB9XG4gIH07XG59KVxuXG4vKipcbiAqIEBuZ2RvYyBkaXJlY3RpdmVcbiAqIEBuYW1lIHVpLmJvb3RzdHJhcC50YWJzLmRpcmVjdGl2ZTp0YWJcbiAqIEByZXN0cmljdCBFQVxuICpcbiAqIEBwYXJhbSB7c3RyaW5nPX0gaGVhZGluZyBUaGUgdmlzaWJsZSBoZWFkaW5nLCBvciB0aXRsZSwgb2YgdGhlIHRhYi4gU2V0IEhUTUwgaGVhZGluZ3Mgd2l0aCB7QGxpbmsgdWkuYm9vdHN0cmFwLnRhYnMuZGlyZWN0aXZlOnRhYkhlYWRpbmcgdGFiSGVhZGluZ30uXG4gKiBAcGFyYW0ge3N0cmluZz19IHNlbGVjdCBBbiBleHByZXNzaW9uIHRvIGV2YWx1YXRlIHdoZW4gdGhlIHRhYiBpcyBzZWxlY3RlZC5cbiAqIEBwYXJhbSB7Ym9vbGVhbj19IGFjdGl2ZSBBIGJpbmRpbmcsIHRlbGxpbmcgd2hldGhlciBvciBub3QgdGhpcyB0YWIgaXMgc2VsZWN0ZWQuXG4gKiBAcGFyYW0ge2Jvb2xlYW49fSBkaXNhYmxlZCBBIGJpbmRpbmcsIHRlbGxpbmcgd2hldGhlciBvciBub3QgdGhpcyB0YWIgaXMgZGlzYWJsZWQuXG4gKlxuICogQGRlc2NyaXB0aW9uXG4gKiBDcmVhdGVzIGEgdGFiIHdpdGggYSBoZWFkaW5nIGFuZCBjb250ZW50LiBNdXN0IGJlIHBsYWNlZCB3aXRoaW4gYSB7QGxpbmsgdWkuYm9vdHN0cmFwLnRhYnMuZGlyZWN0aXZlOnRhYnNldCB0YWJzZXR9LlxuICpcbiAqIEBleGFtcGxlXG48ZXhhbXBsZSBtb2R1bGU9XCJ1aS5ib290c3RyYXBcIj5cbiAgPGZpbGUgbmFtZT1cImluZGV4Lmh0bWxcIj5cbiAgICA8ZGl2IG5nLWNvbnRyb2xsZXI9XCJUYWJzRGVtb0N0cmxcIj5cbiAgICAgIDxidXR0b24gY2xhc3M9XCJidG4gYnRuLXNtYWxsXCIgbmctY2xpY2s9XCJpdGVtc1swXS5hY3RpdmUgPSB0cnVlXCI+XG4gICAgICAgIFNlbGVjdCBpdGVtIDEsIHVzaW5nIGFjdGl2ZSBiaW5kaW5nXG4gICAgICA8L2J1dHRvbj5cbiAgICAgIDxidXR0b24gY2xhc3M9XCJidG4gYnRuLXNtYWxsXCIgbmctY2xpY2s9XCJpdGVtc1sxXS5kaXNhYmxlZCA9ICFpdGVtc1sxXS5kaXNhYmxlZFwiPlxuICAgICAgICBFbmFibGUvZGlzYWJsZSBpdGVtIDIsIHVzaW5nIGRpc2FibGVkIGJpbmRpbmdcbiAgICAgIDwvYnV0dG9uPlxuICAgICAgPGJyIC8+XG4gICAgICA8dWliLXRhYnNldD5cbiAgICAgICAgPHVpYi10YWIgaGVhZGluZz1cIlRhYiAxXCI+Rmlyc3QgVGFiPC91aWItdGFiPlxuICAgICAgICA8dWliLXRhYiBzZWxlY3Q9XCJhbGVydE1lKClcIj5cbiAgICAgICAgICA8dWliLXRhYi1oZWFkaW5nPjxpIGNsYXNzPVwiaWNvbi1iZWxsXCI+PC9pPiBBbGVydCBtZSE8L3RhYi1oZWFkaW5nPlxuICAgICAgICAgIFNlY29uZCBUYWIsIHdpdGggYWxlcnQgY2FsbGJhY2sgYW5kIGh0bWwgaGVhZGluZyFcbiAgICAgICAgPC91aWItdGFiPlxuICAgICAgICA8dWliLXRhYiBuZy1yZXBlYXQ9XCJpdGVtIGluIGl0ZW1zXCJcbiAgICAgICAgICBoZWFkaW5nPVwie3tpdGVtLnRpdGxlfX1cIlxuICAgICAgICAgIGRpc2FibGVkPVwiaXRlbS5kaXNhYmxlZFwiXG4gICAgICAgICAgYWN0aXZlPVwiaXRlbS5hY3RpdmVcIj5cbiAgICAgICAgICB7e2l0ZW0uY29udGVudH19XG4gICAgICAgIDwvdWliLXRhYj5cbiAgICAgIDwvdWliLXRhYnNldD5cbiAgICA8L2Rpdj5cbiAgPC9maWxlPlxuICA8ZmlsZSBuYW1lPVwic2NyaXB0LmpzXCI+XG4gICAgZnVuY3Rpb24gVGFic0RlbW9DdHJsKCRzY29wZSkge1xuICAgICAgJHNjb3BlLml0ZW1zID0gW1xuICAgICAgICB7IHRpdGxlOlwiRHluYW1pYyBUaXRsZSAxXCIsIGNvbnRlbnQ6XCJEeW5hbWljIEl0ZW0gMFwiIH0sXG4gICAgICAgIHsgdGl0bGU6XCJEeW5hbWljIFRpdGxlIDJcIiwgY29udGVudDpcIkR5bmFtaWMgSXRlbSAxXCIsIGRpc2FibGVkOiB0cnVlIH1cbiAgICAgIF07XG5cbiAgICAgICRzY29wZS5hbGVydE1lID0gZnVuY3Rpb24oKSB7XG4gICAgICAgIHNldFRpbWVvdXQoZnVuY3Rpb24oKSB7XG4gICAgICAgICAgYWxlcnQoXCJZb3UndmUgc2VsZWN0ZWQgdGhlIGFsZXJ0IHRhYiFcIik7XG4gICAgICAgIH0pO1xuICAgICAgfTtcbiAgICB9O1xuICA8L2ZpbGU+XG48L2V4YW1wbGU+XG4gKi9cblxuLyoqXG4gKiBAbmdkb2MgZGlyZWN0aXZlXG4gKiBAbmFtZSB1aS5ib290c3RyYXAudGFicy5kaXJlY3RpdmU6dGFiSGVhZGluZ1xuICogQHJlc3RyaWN0IEVBXG4gKlxuICogQGRlc2NyaXB0aW9uXG4gKiBDcmVhdGVzIGFuIEhUTUwgaGVhZGluZyBmb3IgYSB7QGxpbmsgdWkuYm9vdHN0cmFwLnRhYnMuZGlyZWN0aXZlOnRhYiB0YWJ9LiBNdXN0IGJlIHBsYWNlZCBhcyBhIGNoaWxkIG9mIGEgdGFiIGVsZW1lbnQuXG4gKlxuICogQGV4YW1wbGVcbjxleGFtcGxlIG1vZHVsZT1cInVpLmJvb3RzdHJhcFwiPlxuICA8ZmlsZSBuYW1lPVwiaW5kZXguaHRtbFwiPlxuICAgIDx1aWItdGFic2V0PlxuICAgICAgPHVpYi10YWI+XG4gICAgICAgIDx1aWItdGFiLWhlYWRpbmc+PGI+SFRNTDwvYj4gaW4gbXkgdGl0bGVzPyE8L3RhYi1oZWFkaW5nPlxuICAgICAgICBBbmQgc29tZSBjb250ZW50LCB0b28hXG4gICAgICA8L3VpYi10YWI+XG4gICAgICA8dWliLXRhYj5cbiAgICAgICAgPHVpYi10YWItaGVhZGluZz48aSBjbGFzcz1cImljb24taGVhcnRcIj48L2k+IEljb24gaGVhZGluZz8hPzwvdGFiLWhlYWRpbmc+XG4gICAgICAgIFRoYXQncyByaWdodC5cbiAgICAgIDwvdWliLXRhYj5cbiAgICA8L3VpYi10YWJzZXQ+XG4gIDwvZmlsZT5cbjwvZXhhbXBsZT5cbiAqL1xuLmRpcmVjdGl2ZSgndWliVGFiJywgWyckcGFyc2UnLCBmdW5jdGlvbigkcGFyc2UpIHtcbiAgcmV0dXJuIHtcbiAgICByZXF1aXJlOiAnXnVpYlRhYnNldCcsXG4gICAgcmVzdHJpY3Q6ICdFQScsXG4gICAgcmVwbGFjZTogdHJ1ZSxcbiAgICB0ZW1wbGF0ZVVybDogJ3RlbXBsYXRlL3RhYnMvdGFiLmh0bWwnLFxuICAgIHRyYW5zY2x1ZGU6IHRydWUsXG4gICAgc2NvcGU6IHtcbiAgICAgIGFjdGl2ZTogJz0/JyxcbiAgICAgIGhlYWRpbmc6ICdAJyxcbiAgICAgIG9uU2VsZWN0OiAnJnNlbGVjdCcsIC8vVGhpcyBjYWxsYmFjayBpcyBjYWxsZWQgaW4gY29udGVudEhlYWRpbmdUcmFuc2NsdWRlXG4gICAgICAgICAgICAgICAgICAgICAgICAgIC8vb25jZSBpdCBpbnNlcnRzIHRoZSB0YWIncyBjb250ZW50IGludG8gdGhlIGRvbVxuICAgICAgb25EZXNlbGVjdDogJyZkZXNlbGVjdCdcbiAgICB9LFxuICAgIGNvbnRyb2xsZXI6IGZ1bmN0aW9uKCkge1xuICAgICAgLy9FbXB0eSBjb250cm9sbGVyIHNvIG90aGVyIGRpcmVjdGl2ZXMgY2FuIHJlcXVpcmUgYmVpbmcgJ3VuZGVyJyBhIHRhYlxuICAgIH0sXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsbSwgYXR0cnMsIHRhYnNldEN0cmwsIHRyYW5zY2x1ZGUpIHtcbiAgICAgIHNjb3BlLiR3YXRjaCgnYWN0aXZlJywgZnVuY3Rpb24oYWN0aXZlKSB7XG4gICAgICAgIGlmIChhY3RpdmUpIHtcbiAgICAgICAgICB0YWJzZXRDdHJsLnNlbGVjdChzY29wZSk7XG4gICAgICAgIH1cbiAgICAgIH0pO1xuXG4gICAgICBzY29wZS5kaXNhYmxlZCA9IGZhbHNlO1xuICAgICAgaWYgKGF0dHJzLmRpc2FibGUpIHtcbiAgICAgICAgc2NvcGUuJHBhcmVudC4kd2F0Y2goJHBhcnNlKGF0dHJzLmRpc2FibGUpLCBmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgICAgIHNjb3BlLmRpc2FibGVkID0gISEgdmFsdWU7XG4gICAgICAgIH0pO1xuICAgICAgfVxuXG4gICAgICBzY29wZS5zZWxlY3QgPSBmdW5jdGlvbigpIHtcbiAgICAgICAgaWYgKCFzY29wZS5kaXNhYmxlZCkge1xuICAgICAgICAgIHNjb3BlLmFjdGl2ZSA9IHRydWU7XG4gICAgICAgIH1cbiAgICAgIH07XG5cbiAgICAgIHRhYnNldEN0cmwuYWRkVGFiKHNjb3BlKTtcbiAgICAgIHNjb3BlLiRvbignJGRlc3Ryb3knLCBmdW5jdGlvbigpIHtcbiAgICAgICAgdGFic2V0Q3RybC5yZW1vdmVUYWIoc2NvcGUpO1xuICAgICAgfSk7XG5cbiAgICAgIC8vV2UgbmVlZCB0byB0cmFuc2NsdWRlIGxhdGVyLCBvbmNlIHRoZSBjb250ZW50IGNvbnRhaW5lciBpcyByZWFkeS5cbiAgICAgIC8vd2hlbiB0aGlzIGxpbmsgaGFwcGVucywgd2UncmUgaW5zaWRlIGEgdGFiIGhlYWRpbmcuXG4gICAgICBzY29wZS4kdHJhbnNjbHVkZUZuID0gdHJhbnNjbHVkZTtcbiAgICB9XG4gIH07XG59XSlcblxuLmRpcmVjdGl2ZSgndWliVGFiSGVhZGluZ1RyYW5zY2x1ZGUnLCBmdW5jdGlvbigpIHtcbiAgcmV0dXJuIHtcbiAgICByZXN0cmljdDogJ0EnLFxuICAgIHJlcXVpcmU6IFsnP151aWJUYWInLCAnP150YWInXSwgLy8gVE9ETzogY2hhbmdlIHRvICdedWliVGFiJyBhZnRlciBkZXByZWNhdGlvbiByZW1vdmFsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsbSkge1xuICAgICAgc2NvcGUuJHdhdGNoKCdoZWFkaW5nRWxlbWVudCcsIGZ1bmN0aW9uIHVwZGF0ZUhlYWRpbmdFbGVtZW50KGhlYWRpbmcpIHtcbiAgICAgICAgaWYgKGhlYWRpbmcpIHtcbiAgICAgICAgICBlbG0uaHRtbCgnJyk7XG4gICAgICAgICAgZWxtLmFwcGVuZChoZWFkaW5nKTtcbiAgICAgICAgfVxuICAgICAgfSk7XG4gICAgfVxuICB9O1xufSlcblxuLmRpcmVjdGl2ZSgndWliVGFiQ29udGVudFRyYW5zY2x1ZGUnLCBmdW5jdGlvbigpIHtcbiAgcmV0dXJuIHtcbiAgICByZXN0cmljdDogJ0EnLFxuICAgIHJlcXVpcmU6IFsnP151aWJUYWJzZXQnLCAnP150YWJzZXQnXSwgLy8gVE9ETzogY2hhbmdlIHRvICdedWliVGFic2V0JyBhZnRlciBkZXByZWNhdGlvbiByZW1vdmFsXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsbSwgYXR0cnMpIHtcbiAgICAgIHZhciB0YWIgPSBzY29wZS4kZXZhbChhdHRycy51aWJUYWJDb250ZW50VHJhbnNjbHVkZSk7XG5cbiAgICAgIC8vTm93IG91ciB0YWIgaXMgcmVhZHkgdG8gYmUgdHJhbnNjbHVkZWQ6IGJvdGggdGhlIHRhYiBoZWFkaW5nIGFyZWFcbiAgICAgIC8vYW5kIHRoZSB0YWIgY29udGVudCBhcmVhIGFyZSBsb2FkZWQuICBUcmFuc2NsdWRlICdlbSBib3RoLlxuICAgICAgdGFiLiR0cmFuc2NsdWRlRm4odGFiLiRwYXJlbnQsIGZ1bmN0aW9uKGNvbnRlbnRzKSB7XG4gICAgICAgIGFuZ3VsYXIuZm9yRWFjaChjb250ZW50cywgZnVuY3Rpb24obm9kZSkge1xuICAgICAgICAgIGlmIChpc1RhYkhlYWRpbmcobm9kZSkpIHtcbiAgICAgICAgICAgIC8vTGV0IHRhYkhlYWRpbmdUcmFuc2NsdWRlIGtub3cuXG4gICAgICAgICAgICB0YWIuaGVhZGluZ0VsZW1lbnQgPSBub2RlO1xuICAgICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgICBlbG0uYXBwZW5kKG5vZGUpO1xuICAgICAgICAgIH1cbiAgICAgICAgfSk7XG4gICAgICB9KTtcbiAgICB9XG4gIH07XG5cbiAgZnVuY3Rpb24gaXNUYWJIZWFkaW5nKG5vZGUpIHtcbiAgICByZXR1cm4gbm9kZS50YWdOYW1lICYmIChcbiAgICAgIG5vZGUuaGFzQXR0cmlidXRlKCd0YWItaGVhZGluZycpIHx8IC8vIFRPRE86IHJlbW92ZSBhZnRlciBkZXByZWNhdGlvbiByZW1vdmFsXG4gICAgICBub2RlLmhhc0F0dHJpYnV0ZSgnZGF0YS10YWItaGVhZGluZycpIHx8IC8vIFRPRE86IHJlbW92ZSBhZnRlciBkZXByZWNhdGlvbiByZW1vdmFsXG4gICAgICBub2RlLmhhc0F0dHJpYnV0ZSgneC10YWItaGVhZGluZycpIHx8IC8vIFRPRE86IHJlbW92ZSBhZnRlciBkZXByZWNhdGlvbiByZW1vdmFsXG4gICAgICBub2RlLmhhc0F0dHJpYnV0ZSgndWliLXRhYi1oZWFkaW5nJykgfHxcbiAgICAgIG5vZGUuaGFzQXR0cmlidXRlKCdkYXRhLXVpYi10YWItaGVhZGluZycpIHx8XG4gICAgICBub2RlLmhhc0F0dHJpYnV0ZSgneC11aWItdGFiLWhlYWRpbmcnKSB8fFxuICAgICAgbm9kZS50YWdOYW1lLnRvTG93ZXJDYXNlKCkgPT09ICd0YWItaGVhZGluZycgfHwgLy8gVE9ETzogcmVtb3ZlIGFmdGVyIGRlcHJlY2F0aW9uIHJlbW92YWxcbiAgICAgIG5vZGUudGFnTmFtZS50b0xvd2VyQ2FzZSgpID09PSAnZGF0YS10YWItaGVhZGluZycgfHwgLy8gVE9ETzogcmVtb3ZlIGFmdGVyIGRlcHJlY2F0aW9uIHJlbW92YWxcbiAgICAgIG5vZGUudGFnTmFtZS50b0xvd2VyQ2FzZSgpID09PSAneC10YWItaGVhZGluZycgfHwgLy8gVE9ETzogcmVtb3ZlIGFmdGVyIGRlcHJlY2F0aW9uIHJlbW92YWxcbiAgICAgIG5vZGUudGFnTmFtZS50b0xvd2VyQ2FzZSgpID09PSAndWliLXRhYi1oZWFkaW5nJyB8fFxuICAgICAgbm9kZS50YWdOYW1lLnRvTG93ZXJDYXNlKCkgPT09ICdkYXRhLXVpYi10YWItaGVhZGluZycgfHxcbiAgICAgIG5vZGUudGFnTmFtZS50b0xvd2VyQ2FzZSgpID09PSAneC11aWItdGFiLWhlYWRpbmcnXG4gICAgKTtcbiAgfVxufSk7XG5cbi8qIGRlcHJlY2F0ZWQgdGFicyBiZWxvdyAqL1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLnRhYnMnKVxuXG4gIC52YWx1ZSgnJHRhYnNTdXBwcmVzc1dhcm5pbmcnLCBmYWxzZSlcblxuICAuY29udHJvbGxlcignVGFic2V0Q29udHJvbGxlcicsIFsnJHNjb3BlJywgJyRjb250cm9sbGVyJywgJyRsb2cnLCAnJHRhYnNTdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkc2NvcGUsICRjb250cm9sbGVyLCAkbG9nLCAkdGFic1N1cHByZXNzV2FybmluZykge1xuICAgIGlmICghJHRhYnNTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICRsb2cud2FybignVGFic2V0Q29udHJvbGxlciBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIFVpYlRhYnNldENvbnRyb2xsZXIgaW5zdGVhZC4nKTtcbiAgICB9XG5cbiAgICBhbmd1bGFyLmV4dGVuZCh0aGlzLCAkY29udHJvbGxlcignVWliVGFic2V0Q29udHJvbGxlcicsIHtcbiAgICAgICRzY29wZTogJHNjb3BlXG4gICAgfSkpO1xuICB9XSlcblxuICAuZGlyZWN0aXZlKCd0YWJzZXQnLCBbJyRsb2cnLCAnJHRhYnNTdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkbG9nLCAkdGFic1N1cHByZXNzV2FybmluZykge1xuICAgIHJldHVybiB7XG4gICAgICByZXN0cmljdDogJ0VBJyxcbiAgICAgIHRyYW5zY2x1ZGU6IHRydWUsXG4gICAgICByZXBsYWNlOiB0cnVlLFxuICAgICAgc2NvcGU6IHtcbiAgICAgICAgdHlwZTogJ0AnXG4gICAgICB9LFxuICAgICAgY29udHJvbGxlcjogJ1RhYnNldENvbnRyb2xsZXInLFxuICAgICAgdGVtcGxhdGVVcmw6ICd0ZW1wbGF0ZS90YWJzL3RhYnNldC5odG1sJyxcbiAgICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycykge1xuXG4gICAgICAgIGlmICghJHRhYnNTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgICAkbG9nLndhcm4oJ3RhYnNldCBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi10YWJzZXQgaW5zdGVhZC4nKTtcbiAgICAgICAgfVxuICAgICAgICBzY29wZS52ZXJ0aWNhbCA9IGFuZ3VsYXIuaXNEZWZpbmVkKGF0dHJzLnZlcnRpY2FsKSA/IHNjb3BlLiRwYXJlbnQuJGV2YWwoYXR0cnMudmVydGljYWwpIDogZmFsc2U7XG4gICAgICAgIHNjb3BlLmp1c3RpZmllZCA9IGFuZ3VsYXIuaXNEZWZpbmVkKGF0dHJzLmp1c3RpZmllZCkgPyBzY29wZS4kcGFyZW50LiRldmFsKGF0dHJzLmp1c3RpZmllZCkgOiBmYWxzZTtcbiAgICAgIH1cbiAgICB9O1xuICB9XSlcblxuICAuZGlyZWN0aXZlKCd0YWInLCBbJyRwYXJzZScsICckbG9nJywgJyR0YWJzU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJHBhcnNlLCAkbG9nLCAkdGFic1N1cHByZXNzV2FybmluZykge1xuICAgIHJldHVybiB7XG4gICAgICByZXF1aXJlOiAnXnRhYnNldCcsXG4gICAgICByZXN0cmljdDogJ0VBJyxcbiAgICAgIHJlcGxhY2U6IHRydWUsXG4gICAgICB0ZW1wbGF0ZVVybDogJ3RlbXBsYXRlL3RhYnMvdGFiLmh0bWwnLFxuICAgICAgdHJhbnNjbHVkZTogdHJ1ZSxcbiAgICAgIHNjb3BlOiB7XG4gICAgICAgIGFjdGl2ZTogJz0/JyxcbiAgICAgICAgaGVhZGluZzogJ0AnLFxuICAgICAgICBvblNlbGVjdDogJyZzZWxlY3QnLCAvL1RoaXMgY2FsbGJhY2sgaXMgY2FsbGVkIGluIGNvbnRlbnRIZWFkaW5nVHJhbnNjbHVkZVxuICAgICAgICAvL29uY2UgaXQgaW5zZXJ0cyB0aGUgdGFiJ3MgY29udGVudCBpbnRvIHRoZSBkb21cbiAgICAgICAgb25EZXNlbGVjdDogJyZkZXNlbGVjdCdcbiAgICAgIH0sXG4gICAgICBjb250cm9sbGVyOiBmdW5jdGlvbigpIHtcbiAgICAgICAgLy9FbXB0eSBjb250cm9sbGVyIHNvIG90aGVyIGRpcmVjdGl2ZXMgY2FuIHJlcXVpcmUgYmVpbmcgJ3VuZGVyJyBhIHRhYlxuICAgICAgfSxcbiAgICAgIGxpbms6IGZ1bmN0aW9uKHNjb3BlLCBlbG0sIGF0dHJzLCB0YWJzZXRDdHJsLCB0cmFuc2NsdWRlKSB7XG4gICAgICAgIGlmICghJHRhYnNTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgICAkbG9nLndhcm4oJ3RhYiBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi10YWIgaW5zdGVhZC4nKTtcbiAgICAgICAgfVxuXG4gICAgICAgIHNjb3BlLiR3YXRjaCgnYWN0aXZlJywgZnVuY3Rpb24oYWN0aXZlKSB7XG4gICAgICAgICAgaWYgKGFjdGl2ZSkge1xuICAgICAgICAgICAgdGFic2V0Q3RybC5zZWxlY3Qoc2NvcGUpO1xuICAgICAgICAgIH1cbiAgICAgICAgfSk7XG5cbiAgICAgICAgc2NvcGUuZGlzYWJsZWQgPSBmYWxzZTtcbiAgICAgICAgaWYgKGF0dHJzLmRpc2FibGUpIHtcbiAgICAgICAgICBzY29wZS4kcGFyZW50LiR3YXRjaCgkcGFyc2UoYXR0cnMuZGlzYWJsZSksIGZ1bmN0aW9uKHZhbHVlKSB7XG4gICAgICAgICAgICBzY29wZS5kaXNhYmxlZCA9ICEhdmFsdWU7XG4gICAgICAgICAgfSk7XG4gICAgICAgIH1cblxuICAgICAgICBzY29wZS5zZWxlY3QgPSBmdW5jdGlvbigpIHtcbiAgICAgICAgICBpZiAoIXNjb3BlLmRpc2FibGVkKSB7XG4gICAgICAgICAgICBzY29wZS5hY3RpdmUgPSB0cnVlO1xuICAgICAgICAgIH1cbiAgICAgICAgfTtcblxuICAgICAgICB0YWJzZXRDdHJsLmFkZFRhYihzY29wZSk7XG4gICAgICAgIHNjb3BlLiRvbignJGRlc3Ryb3knLCBmdW5jdGlvbigpIHtcbiAgICAgICAgICB0YWJzZXRDdHJsLnJlbW92ZVRhYihzY29wZSk7XG4gICAgICAgIH0pO1xuXG4gICAgICAgIC8vV2UgbmVlZCB0byB0cmFuc2NsdWRlIGxhdGVyLCBvbmNlIHRoZSBjb250ZW50IGNvbnRhaW5lciBpcyByZWFkeS5cbiAgICAgICAgLy93aGVuIHRoaXMgbGluayBoYXBwZW5zLCB3ZSdyZSBpbnNpZGUgYSB0YWIgaGVhZGluZy5cbiAgICAgICAgc2NvcGUuJHRyYW5zY2x1ZGVGbiA9IHRyYW5zY2x1ZGU7XG4gICAgICB9XG4gICAgfTtcbiAgfV0pXG5cbiAgLmRpcmVjdGl2ZSgndGFiSGVhZGluZ1RyYW5zY2x1ZGUnLCBbJyRsb2cnLCAnJHRhYnNTdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkbG9nLCAkdGFic1N1cHByZXNzV2FybmluZykge1xuICAgIHJldHVybiB7XG4gICAgICByZXN0cmljdDogJ0EnLFxuICAgICAgcmVxdWlyZTogJ150YWInLFxuICAgICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsbSkge1xuICAgICAgICBpZiAoISR0YWJzU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICAgJGxvZy53YXJuKCd0YWItaGVhZGluZy10cmFuc2NsdWRlIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliLXRhYi1oZWFkaW5nLXRyYW5zY2x1ZGUgaW5zdGVhZC4nKTtcbiAgICAgICAgfVxuXG4gICAgICAgIHNjb3BlLiR3YXRjaCgnaGVhZGluZ0VsZW1lbnQnLCBmdW5jdGlvbiB1cGRhdGVIZWFkaW5nRWxlbWVudChoZWFkaW5nKSB7XG4gICAgICAgICAgaWYgKGhlYWRpbmcpIHtcbiAgICAgICAgICAgIGVsbS5odG1sKCcnKTtcbiAgICAgICAgICAgIGVsbS5hcHBlbmQoaGVhZGluZyk7XG4gICAgICAgICAgfVxuICAgICAgICB9KTtcbiAgICAgIH1cbiAgICB9O1xuICB9XSlcblxuICAuZGlyZWN0aXZlKCd0YWJDb250ZW50VHJhbnNjbHVkZScsIFsnJGxvZycsICckdGFic1N1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRsb2csICR0YWJzU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgcmV0dXJuIHtcbiAgICAgIHJlc3RyaWN0OiAnQScsXG4gICAgICByZXF1aXJlOiAnXnRhYnNldCcsXG4gICAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxtLCBhdHRycykge1xuICAgICAgICBpZiAoISR0YWJzU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICAgJGxvZy53YXJuKCd0YWItY29udGVudC10cmFuc2NsdWRlIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliLXRhYi1jb250ZW50LXRyYW5zY2x1ZGUgaW5zdGVhZC4nKTtcbiAgICAgICAgfVxuXG4gICAgICAgIHZhciB0YWIgPSBzY29wZS4kZXZhbChhdHRycy50YWJDb250ZW50VHJhbnNjbHVkZSk7XG5cbiAgICAgICAgLy9Ob3cgb3VyIHRhYiBpcyByZWFkeSB0byBiZSB0cmFuc2NsdWRlZDogYm90aCB0aGUgdGFiIGhlYWRpbmcgYXJlYVxuICAgICAgICAvL2FuZCB0aGUgdGFiIGNvbnRlbnQgYXJlYSBhcmUgbG9hZGVkLiAgVHJhbnNjbHVkZSAnZW0gYm90aC5cbiAgICAgICAgdGFiLiR0cmFuc2NsdWRlRm4odGFiLiRwYXJlbnQsIGZ1bmN0aW9uKGNvbnRlbnRzKSB7XG4gICAgICAgICAgYW5ndWxhci5mb3JFYWNoKGNvbnRlbnRzLCBmdW5jdGlvbihub2RlKSB7XG4gICAgICAgICAgICBpZiAoaXNUYWJIZWFkaW5nKG5vZGUpKSB7XG4gICAgICAgICAgICAgIC8vTGV0IHRhYkhlYWRpbmdUcmFuc2NsdWRlIGtub3cuXG4gICAgICAgICAgICAgIHRhYi5oZWFkaW5nRWxlbWVudCA9IG5vZGU7XG4gICAgICAgICAgICB9XG4gICAgICAgICAgICBlbHNlIHtcbiAgICAgICAgICAgICAgZWxtLmFwcGVuZChub2RlKTtcbiAgICAgICAgICAgIH1cbiAgICAgICAgICB9KTtcbiAgICAgICAgfSk7XG4gICAgICB9XG4gICAgfTtcblxuICAgIGZ1bmN0aW9uIGlzVGFiSGVhZGluZyhub2RlKSB7XG4gICAgICByZXR1cm4gbm9kZS50YWdOYW1lICYmIChcbiAgICAgICAgICBub2RlLmhhc0F0dHJpYnV0ZSgndGFiLWhlYWRpbmcnKSB8fFxuICAgICAgICAgIG5vZGUuaGFzQXR0cmlidXRlKCdkYXRhLXRhYi1oZWFkaW5nJykgfHxcbiAgICAgICAgICBub2RlLmhhc0F0dHJpYnV0ZSgneC10YWItaGVhZGluZycpIHx8XG4gICAgICAgICAgbm9kZS50YWdOYW1lLnRvTG93ZXJDYXNlKCkgPT09ICd0YWItaGVhZGluZycgfHxcbiAgICAgICAgICBub2RlLnRhZ05hbWUudG9Mb3dlckNhc2UoKSA9PT0gJ2RhdGEtdGFiLWhlYWRpbmcnIHx8XG4gICAgICAgICAgbm9kZS50YWdOYW1lLnRvTG93ZXJDYXNlKCkgPT09ICd4LXRhYi1oZWFkaW5nJ1xuICAgICAgICApO1xuICAgIH1cbiAgfV0pO1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLnRpbWVwaWNrZXInLCBbXSlcblxuLmNvbnN0YW50KCd1aWJUaW1lcGlja2VyQ29uZmlnJywge1xuICBob3VyU3RlcDogMSxcbiAgbWludXRlU3RlcDogMSxcbiAgc2hvd01lcmlkaWFuOiB0cnVlLFxuICBtZXJpZGlhbnM6IG51bGwsXG4gIHJlYWRvbmx5SW5wdXQ6IGZhbHNlLFxuICBtb3VzZXdoZWVsOiB0cnVlLFxuICBhcnJvd2tleXM6IHRydWUsXG4gIHNob3dTcGlubmVyczogdHJ1ZVxufSlcblxuLmNvbnRyb2xsZXIoJ1VpYlRpbWVwaWNrZXJDb250cm9sbGVyJywgWyckc2NvcGUnLCAnJGVsZW1lbnQnLCAnJGF0dHJzJywgJyRwYXJzZScsICckbG9nJywgJyRsb2NhbGUnLCAndWliVGltZXBpY2tlckNvbmZpZycsIGZ1bmN0aW9uKCRzY29wZSwgJGVsZW1lbnQsICRhdHRycywgJHBhcnNlLCAkbG9nLCAkbG9jYWxlLCB0aW1lcGlja2VyQ29uZmlnKSB7XG4gIHZhciBzZWxlY3RlZCA9IG5ldyBEYXRlKCksXG4gICAgICBuZ01vZGVsQ3RybCA9IHsgJHNldFZpZXdWYWx1ZTogYW5ndWxhci5ub29wIH0sIC8vIG51bGxNb2RlbEN0cmxcbiAgICAgIG1lcmlkaWFucyA9IGFuZ3VsYXIuaXNEZWZpbmVkKCRhdHRycy5tZXJpZGlhbnMpID8gJHNjb3BlLiRwYXJlbnQuJGV2YWwoJGF0dHJzLm1lcmlkaWFucykgOiB0aW1lcGlja2VyQ29uZmlnLm1lcmlkaWFucyB8fCAkbG9jYWxlLkRBVEVUSU1FX0ZPUk1BVFMuQU1QTVM7XG5cbiAgJHNjb3BlLnRhYmluZGV4ID0gYW5ndWxhci5pc0RlZmluZWQoJGF0dHJzLnRhYmluZGV4KSA/ICRhdHRycy50YWJpbmRleCA6IDA7XG4gICRlbGVtZW50LnJlbW92ZUF0dHIoJ3RhYmluZGV4Jyk7XG5cbiAgdGhpcy5pbml0ID0gZnVuY3Rpb24obmdNb2RlbEN0cmxfLCBpbnB1dHMpIHtcbiAgICBuZ01vZGVsQ3RybCA9IG5nTW9kZWxDdHJsXztcbiAgICBuZ01vZGVsQ3RybC4kcmVuZGVyID0gdGhpcy5yZW5kZXI7XG5cbiAgICBuZ01vZGVsQ3RybC4kZm9ybWF0dGVycy51bnNoaWZ0KGZ1bmN0aW9uKG1vZGVsVmFsdWUpIHtcbiAgICAgIHJldHVybiBtb2RlbFZhbHVlID8gbmV3IERhdGUobW9kZWxWYWx1ZSkgOiBudWxsO1xuICAgIH0pO1xuXG4gICAgdmFyIGhvdXJzSW5wdXRFbCA9IGlucHV0cy5lcSgwKSxcbiAgICAgICAgbWludXRlc0lucHV0RWwgPSBpbnB1dHMuZXEoMSk7XG5cbiAgICB2YXIgbW91c2V3aGVlbCA9IGFuZ3VsYXIuaXNEZWZpbmVkKCRhdHRycy5tb3VzZXdoZWVsKSA/ICRzY29wZS4kcGFyZW50LiRldmFsKCRhdHRycy5tb3VzZXdoZWVsKSA6IHRpbWVwaWNrZXJDb25maWcubW91c2V3aGVlbDtcbiAgICBpZiAobW91c2V3aGVlbCkge1xuICAgICAgdGhpcy5zZXR1cE1vdXNld2hlZWxFdmVudHMoaG91cnNJbnB1dEVsLCBtaW51dGVzSW5wdXRFbCk7XG4gICAgfVxuXG4gICAgdmFyIGFycm93a2V5cyA9IGFuZ3VsYXIuaXNEZWZpbmVkKCRhdHRycy5hcnJvd2tleXMpID8gJHNjb3BlLiRwYXJlbnQuJGV2YWwoJGF0dHJzLmFycm93a2V5cykgOiB0aW1lcGlja2VyQ29uZmlnLmFycm93a2V5cztcbiAgICBpZiAoYXJyb3drZXlzKSB7XG4gICAgICB0aGlzLnNldHVwQXJyb3drZXlFdmVudHMoaG91cnNJbnB1dEVsLCBtaW51dGVzSW5wdXRFbCk7XG4gICAgfVxuXG4gICAgJHNjb3BlLnJlYWRvbmx5SW5wdXQgPSBhbmd1bGFyLmlzRGVmaW5lZCgkYXR0cnMucmVhZG9ubHlJbnB1dCkgPyAkc2NvcGUuJHBhcmVudC4kZXZhbCgkYXR0cnMucmVhZG9ubHlJbnB1dCkgOiB0aW1lcGlja2VyQ29uZmlnLnJlYWRvbmx5SW5wdXQ7XG4gICAgdGhpcy5zZXR1cElucHV0RXZlbnRzKGhvdXJzSW5wdXRFbCwgbWludXRlc0lucHV0RWwpO1xuICB9O1xuXG4gIHZhciBob3VyU3RlcCA9IHRpbWVwaWNrZXJDb25maWcuaG91clN0ZXA7XG4gIGlmICgkYXR0cnMuaG91clN0ZXApIHtcbiAgICAkc2NvcGUuJHBhcmVudC4kd2F0Y2goJHBhcnNlKCRhdHRycy5ob3VyU3RlcCksIGZ1bmN0aW9uKHZhbHVlKSB7XG4gICAgICBob3VyU3RlcCA9IHBhcnNlSW50KHZhbHVlLCAxMCk7XG4gICAgfSk7XG4gIH1cblxuICB2YXIgbWludXRlU3RlcCA9IHRpbWVwaWNrZXJDb25maWcubWludXRlU3RlcDtcbiAgaWYgKCRhdHRycy5taW51dGVTdGVwKSB7XG4gICAgJHNjb3BlLiRwYXJlbnQuJHdhdGNoKCRwYXJzZSgkYXR0cnMubWludXRlU3RlcCksIGZ1bmN0aW9uKHZhbHVlKSB7XG4gICAgICBtaW51dGVTdGVwID0gcGFyc2VJbnQodmFsdWUsIDEwKTtcbiAgICB9KTtcbiAgfVxuXG4gIHZhciBtaW47XG4gICRzY29wZS4kcGFyZW50LiR3YXRjaCgkcGFyc2UoJGF0dHJzLm1pbiksIGZ1bmN0aW9uKHZhbHVlKSB7XG4gICAgdmFyIGR0ID0gbmV3IERhdGUodmFsdWUpO1xuICAgIG1pbiA9IGlzTmFOKGR0KSA/IHVuZGVmaW5lZCA6IGR0O1xuICB9KTtcblxuICB2YXIgbWF4O1xuICAkc2NvcGUuJHBhcmVudC4kd2F0Y2goJHBhcnNlKCRhdHRycy5tYXgpLCBmdW5jdGlvbih2YWx1ZSkge1xuICAgIHZhciBkdCA9IG5ldyBEYXRlKHZhbHVlKTtcbiAgICBtYXggPSBpc05hTihkdCkgPyB1bmRlZmluZWQgOiBkdDtcbiAgfSk7XG5cbiAgJHNjb3BlLm5vSW5jcmVtZW50SG91cnMgPSBmdW5jdGlvbigpIHtcbiAgICB2YXIgaW5jcmVtZW50ZWRTZWxlY3RlZCA9IGFkZE1pbnV0ZXMoc2VsZWN0ZWQsIGhvdXJTdGVwICogNjApO1xuICAgIHJldHVybiBpbmNyZW1lbnRlZFNlbGVjdGVkID4gbWF4IHx8XG4gICAgICAoaW5jcmVtZW50ZWRTZWxlY3RlZCA8IHNlbGVjdGVkICYmIGluY3JlbWVudGVkU2VsZWN0ZWQgPCBtaW4pO1xuICB9O1xuXG4gICRzY29wZS5ub0RlY3JlbWVudEhvdXJzID0gZnVuY3Rpb24oKSB7XG4gICAgdmFyIGRlY3JlbWVudGVkU2VsZWN0ZWQgPSBhZGRNaW51dGVzKHNlbGVjdGVkLCAtaG91clN0ZXAgKiA2MCk7XG4gICAgcmV0dXJuIGRlY3JlbWVudGVkU2VsZWN0ZWQgPCBtaW4gfHxcbiAgICAgIChkZWNyZW1lbnRlZFNlbGVjdGVkID4gc2VsZWN0ZWQgJiYgZGVjcmVtZW50ZWRTZWxlY3RlZCA+IG1heCk7XG4gIH07XG5cbiAgJHNjb3BlLm5vSW5jcmVtZW50TWludXRlcyA9IGZ1bmN0aW9uKCkge1xuICAgIHZhciBpbmNyZW1lbnRlZFNlbGVjdGVkID0gYWRkTWludXRlcyhzZWxlY3RlZCwgbWludXRlU3RlcCk7XG4gICAgcmV0dXJuIGluY3JlbWVudGVkU2VsZWN0ZWQgPiBtYXggfHxcbiAgICAgIChpbmNyZW1lbnRlZFNlbGVjdGVkIDwgc2VsZWN0ZWQgJiYgaW5jcmVtZW50ZWRTZWxlY3RlZCA8IG1pbik7XG4gIH07XG5cbiAgJHNjb3BlLm5vRGVjcmVtZW50TWludXRlcyA9IGZ1bmN0aW9uKCkge1xuICAgIHZhciBkZWNyZW1lbnRlZFNlbGVjdGVkID0gYWRkTWludXRlcyhzZWxlY3RlZCwgLW1pbnV0ZVN0ZXApO1xuICAgIHJldHVybiBkZWNyZW1lbnRlZFNlbGVjdGVkIDwgbWluIHx8XG4gICAgICAoZGVjcmVtZW50ZWRTZWxlY3RlZCA+IHNlbGVjdGVkICYmIGRlY3JlbWVudGVkU2VsZWN0ZWQgPiBtYXgpO1xuICB9O1xuXG4gICRzY29wZS5ub1RvZ2dsZU1lcmlkaWFuID0gZnVuY3Rpb24oKSB7XG4gICAgaWYgKHNlbGVjdGVkLmdldEhvdXJzKCkgPCAxMykge1xuICAgICAgcmV0dXJuIGFkZE1pbnV0ZXMoc2VsZWN0ZWQsIDEyICogNjApID4gbWF4O1xuICAgIH0gZWxzZSB7XG4gICAgICByZXR1cm4gYWRkTWludXRlcyhzZWxlY3RlZCwgLTEyICogNjApIDwgbWluO1xuICAgIH1cbiAgfTtcblxuICAvLyAxMkggLyAyNEggbW9kZVxuICAkc2NvcGUuc2hvd01lcmlkaWFuID0gdGltZXBpY2tlckNvbmZpZy5zaG93TWVyaWRpYW47XG4gIGlmICgkYXR0cnMuc2hvd01lcmlkaWFuKSB7XG4gICAgJHNjb3BlLiRwYXJlbnQuJHdhdGNoKCRwYXJzZSgkYXR0cnMuc2hvd01lcmlkaWFuKSwgZnVuY3Rpb24odmFsdWUpIHtcbiAgICAgICRzY29wZS5zaG93TWVyaWRpYW4gPSAhIXZhbHVlO1xuXG4gICAgICBpZiAobmdNb2RlbEN0cmwuJGVycm9yLnRpbWUpIHtcbiAgICAgICAgLy8gRXZhbHVhdGUgZnJvbSB0ZW1wbGF0ZVxuICAgICAgICB2YXIgaG91cnMgPSBnZXRIb3Vyc0Zyb21UZW1wbGF0ZSgpLCBtaW51dGVzID0gZ2V0TWludXRlc0Zyb21UZW1wbGF0ZSgpO1xuICAgICAgICBpZiAoYW5ndWxhci5pc0RlZmluZWQoaG91cnMpICYmIGFuZ3VsYXIuaXNEZWZpbmVkKG1pbnV0ZXMpKSB7XG4gICAgICAgICAgc2VsZWN0ZWQuc2V0SG91cnMoaG91cnMpO1xuICAgICAgICAgIHJlZnJlc2goKTtcbiAgICAgICAgfVxuICAgICAgfSBlbHNlIHtcbiAgICAgICAgdXBkYXRlVGVtcGxhdGUoKTtcbiAgICAgIH1cbiAgICB9KTtcbiAgfVxuXG4gIC8vIEdldCAkc2NvcGUuaG91cnMgaW4gMjRIIG1vZGUgaWYgdmFsaWRcbiAgZnVuY3Rpb24gZ2V0SG91cnNGcm9tVGVtcGxhdGUoKSB7XG4gICAgdmFyIGhvdXJzID0gcGFyc2VJbnQoJHNjb3BlLmhvdXJzLCAxMCk7XG4gICAgdmFyIHZhbGlkID0gJHNjb3BlLnNob3dNZXJpZGlhbiA/IChob3VycyA+IDAgJiYgaG91cnMgPCAxMykgOiAoaG91cnMgPj0gMCAmJiBob3VycyA8IDI0KTtcbiAgICBpZiAoIXZhbGlkKSB7XG4gICAgICByZXR1cm4gdW5kZWZpbmVkO1xuICAgIH1cblxuICAgIGlmICgkc2NvcGUuc2hvd01lcmlkaWFuKSB7XG4gICAgICBpZiAoaG91cnMgPT09IDEyKSB7XG4gICAgICAgIGhvdXJzID0gMDtcbiAgICAgIH1cbiAgICAgIGlmICgkc2NvcGUubWVyaWRpYW4gPT09IG1lcmlkaWFuc1sxXSkge1xuICAgICAgICBob3VycyA9IGhvdXJzICsgMTI7XG4gICAgICB9XG4gICAgfVxuICAgIHJldHVybiBob3VycztcbiAgfVxuXG4gIGZ1bmN0aW9uIGdldE1pbnV0ZXNGcm9tVGVtcGxhdGUoKSB7XG4gICAgdmFyIG1pbnV0ZXMgPSBwYXJzZUludCgkc2NvcGUubWludXRlcywgMTApO1xuICAgIHJldHVybiAobWludXRlcyA+PSAwICYmIG1pbnV0ZXMgPCA2MCkgPyBtaW51dGVzIDogdW5kZWZpbmVkO1xuICB9XG5cbiAgZnVuY3Rpb24gcGFkKHZhbHVlKSB7XG4gICAgcmV0dXJuIChhbmd1bGFyLmlzRGVmaW5lZCh2YWx1ZSkgJiYgdmFsdWUudG9TdHJpbmcoKS5sZW5ndGggPCAyKSA/ICcwJyArIHZhbHVlIDogdmFsdWUudG9TdHJpbmcoKTtcbiAgfVxuXG4gIC8vIFJlc3BvbmQgb24gbW91c2V3aGVlbCBzcGluXG4gIHRoaXMuc2V0dXBNb3VzZXdoZWVsRXZlbnRzID0gZnVuY3Rpb24oaG91cnNJbnB1dEVsLCBtaW51dGVzSW5wdXRFbCkge1xuICAgIHZhciBpc1Njcm9sbGluZ1VwID0gZnVuY3Rpb24oZSkge1xuICAgICAgaWYgKGUub3JpZ2luYWxFdmVudCkge1xuICAgICAgICBlID0gZS5vcmlnaW5hbEV2ZW50O1xuICAgICAgfVxuICAgICAgLy9waWNrIGNvcnJlY3QgZGVsdGEgdmFyaWFibGUgZGVwZW5kaW5nIG9uIGV2ZW50XG4gICAgICB2YXIgZGVsdGEgPSAoZS53aGVlbERlbHRhKSA/IGUud2hlZWxEZWx0YSA6IC1lLmRlbHRhWTtcbiAgICAgIHJldHVybiAoZS5kZXRhaWwgfHwgZGVsdGEgPiAwKTtcbiAgICB9O1xuXG4gICAgaG91cnNJbnB1dEVsLmJpbmQoJ21vdXNld2hlZWwgd2hlZWwnLCBmdW5jdGlvbihlKSB7XG4gICAgICAkc2NvcGUuJGFwcGx5KGlzU2Nyb2xsaW5nVXAoZSkgPyAkc2NvcGUuaW5jcmVtZW50SG91cnMoKSA6ICRzY29wZS5kZWNyZW1lbnRIb3VycygpKTtcbiAgICAgIGUucHJldmVudERlZmF1bHQoKTtcbiAgICB9KTtcblxuICAgIG1pbnV0ZXNJbnB1dEVsLmJpbmQoJ21vdXNld2hlZWwgd2hlZWwnLCBmdW5jdGlvbihlKSB7XG4gICAgICAkc2NvcGUuJGFwcGx5KGlzU2Nyb2xsaW5nVXAoZSkgPyAkc2NvcGUuaW5jcmVtZW50TWludXRlcygpIDogJHNjb3BlLmRlY3JlbWVudE1pbnV0ZXMoKSk7XG4gICAgICBlLnByZXZlbnREZWZhdWx0KCk7XG4gICAgfSk7XG5cbiAgfTtcblxuICAvLyBSZXNwb25kIG9uIHVwL2Rvd24gYXJyb3drZXlzXG4gIHRoaXMuc2V0dXBBcnJvd2tleUV2ZW50cyA9IGZ1bmN0aW9uKGhvdXJzSW5wdXRFbCwgbWludXRlc0lucHV0RWwpIHtcbiAgICBob3Vyc0lucHV0RWwuYmluZCgna2V5ZG93bicsIGZ1bmN0aW9uKGUpIHtcbiAgICAgIGlmIChlLndoaWNoID09PSAzOCkgeyAvLyB1cFxuICAgICAgICBlLnByZXZlbnREZWZhdWx0KCk7XG4gICAgICAgICRzY29wZS5pbmNyZW1lbnRIb3VycygpO1xuICAgICAgICAkc2NvcGUuJGFwcGx5KCk7XG4gICAgICB9IGVsc2UgaWYgKGUud2hpY2ggPT09IDQwKSB7IC8vIGRvd25cbiAgICAgICAgZS5wcmV2ZW50RGVmYXVsdCgpO1xuICAgICAgICAkc2NvcGUuZGVjcmVtZW50SG91cnMoKTtcbiAgICAgICAgJHNjb3BlLiRhcHBseSgpO1xuICAgICAgfVxuICAgIH0pO1xuXG4gICAgbWludXRlc0lucHV0RWwuYmluZCgna2V5ZG93bicsIGZ1bmN0aW9uKGUpIHtcbiAgICAgIGlmIChlLndoaWNoID09PSAzOCkgeyAvLyB1cFxuICAgICAgICBlLnByZXZlbnREZWZhdWx0KCk7XG4gICAgICAgICRzY29wZS5pbmNyZW1lbnRNaW51dGVzKCk7XG4gICAgICAgICRzY29wZS4kYXBwbHkoKTtcbiAgICAgIH0gZWxzZSBpZiAoZS53aGljaCA9PT0gNDApIHsgLy8gZG93blxuICAgICAgICBlLnByZXZlbnREZWZhdWx0KCk7XG4gICAgICAgICRzY29wZS5kZWNyZW1lbnRNaW51dGVzKCk7XG4gICAgICAgICRzY29wZS4kYXBwbHkoKTtcbiAgICAgIH1cbiAgICB9KTtcbiAgfTtcblxuICB0aGlzLnNldHVwSW5wdXRFdmVudHMgPSBmdW5jdGlvbihob3Vyc0lucHV0RWwsIG1pbnV0ZXNJbnB1dEVsKSB7XG4gICAgaWYgKCRzY29wZS5yZWFkb25seUlucHV0KSB7XG4gICAgICAkc2NvcGUudXBkYXRlSG91cnMgPSBhbmd1bGFyLm5vb3A7XG4gICAgICAkc2NvcGUudXBkYXRlTWludXRlcyA9IGFuZ3VsYXIubm9vcDtcbiAgICAgIHJldHVybjtcbiAgICB9XG5cbiAgICB2YXIgaW52YWxpZGF0ZSA9IGZ1bmN0aW9uKGludmFsaWRIb3VycywgaW52YWxpZE1pbnV0ZXMpIHtcbiAgICAgIG5nTW9kZWxDdHJsLiRzZXRWaWV3VmFsdWUobnVsbCk7XG4gICAgICBuZ01vZGVsQ3RybC4kc2V0VmFsaWRpdHkoJ3RpbWUnLCBmYWxzZSk7XG4gICAgICBpZiAoYW5ndWxhci5pc0RlZmluZWQoaW52YWxpZEhvdXJzKSkge1xuICAgICAgICAkc2NvcGUuaW52YWxpZEhvdXJzID0gaW52YWxpZEhvdXJzO1xuICAgICAgfVxuICAgICAgaWYgKGFuZ3VsYXIuaXNEZWZpbmVkKGludmFsaWRNaW51dGVzKSkge1xuICAgICAgICAkc2NvcGUuaW52YWxpZE1pbnV0ZXMgPSBpbnZhbGlkTWludXRlcztcbiAgICAgIH1cbiAgICB9O1xuXG4gICAgJHNjb3BlLnVwZGF0ZUhvdXJzID0gZnVuY3Rpb24oKSB7XG4gICAgICB2YXIgaG91cnMgPSBnZXRIb3Vyc0Zyb21UZW1wbGF0ZSgpLFxuICAgICAgICBtaW51dGVzID0gZ2V0TWludXRlc0Zyb21UZW1wbGF0ZSgpO1xuXG4gICAgICBpZiAoYW5ndWxhci5pc0RlZmluZWQoaG91cnMpICYmIGFuZ3VsYXIuaXNEZWZpbmVkKG1pbnV0ZXMpKSB7XG4gICAgICAgIHNlbGVjdGVkLnNldEhvdXJzKGhvdXJzKTtcbiAgICAgICAgaWYgKHNlbGVjdGVkIDwgbWluIHx8IHNlbGVjdGVkID4gbWF4KSB7XG4gICAgICAgICAgaW52YWxpZGF0ZSh0cnVlKTtcbiAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICByZWZyZXNoKCdoJyk7XG4gICAgICAgIH1cbiAgICAgIH0gZWxzZSB7XG4gICAgICAgIGludmFsaWRhdGUodHJ1ZSk7XG4gICAgICB9XG4gICAgfTtcblxuICAgIGhvdXJzSW5wdXRFbC5iaW5kKCdibHVyJywgZnVuY3Rpb24oZSkge1xuICAgICAgaWYgKCEkc2NvcGUuaW52YWxpZEhvdXJzICYmICRzY29wZS5ob3VycyA8IDEwKSB7XG4gICAgICAgICRzY29wZS4kYXBwbHkoZnVuY3Rpb24oKSB7XG4gICAgICAgICAgJHNjb3BlLmhvdXJzID0gcGFkKCRzY29wZS5ob3Vycyk7XG4gICAgICAgIH0pO1xuICAgICAgfVxuICAgIH0pO1xuXG4gICAgJHNjb3BlLnVwZGF0ZU1pbnV0ZXMgPSBmdW5jdGlvbigpIHtcbiAgICAgIHZhciBtaW51dGVzID0gZ2V0TWludXRlc0Zyb21UZW1wbGF0ZSgpLFxuICAgICAgICBob3VycyA9IGdldEhvdXJzRnJvbVRlbXBsYXRlKCk7XG5cbiAgICAgIGlmIChhbmd1bGFyLmlzRGVmaW5lZChtaW51dGVzKSAmJiBhbmd1bGFyLmlzRGVmaW5lZChob3VycykpIHtcbiAgICAgICAgc2VsZWN0ZWQuc2V0TWludXRlcyhtaW51dGVzKTtcbiAgICAgICAgaWYgKHNlbGVjdGVkIDwgbWluIHx8IHNlbGVjdGVkID4gbWF4KSB7XG4gICAgICAgICAgaW52YWxpZGF0ZSh1bmRlZmluZWQsIHRydWUpO1xuICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgIHJlZnJlc2goJ20nKTtcbiAgICAgICAgfVxuICAgICAgfSBlbHNlIHtcbiAgICAgICAgaW52YWxpZGF0ZSh1bmRlZmluZWQsIHRydWUpO1xuICAgICAgfVxuICAgIH07XG5cbiAgICBtaW51dGVzSW5wdXRFbC5iaW5kKCdibHVyJywgZnVuY3Rpb24oZSkge1xuICAgICAgaWYgKCEkc2NvcGUuaW52YWxpZE1pbnV0ZXMgJiYgJHNjb3BlLm1pbnV0ZXMgPCAxMCkge1xuICAgICAgICAkc2NvcGUuJGFwcGx5KGZ1bmN0aW9uKCkge1xuICAgICAgICAgICRzY29wZS5taW51dGVzID0gcGFkKCRzY29wZS5taW51dGVzKTtcbiAgICAgICAgfSk7XG4gICAgICB9XG4gICAgfSk7XG5cbiAgfTtcblxuICB0aGlzLnJlbmRlciA9IGZ1bmN0aW9uKCkge1xuICAgIHZhciBkYXRlID0gbmdNb2RlbEN0cmwuJHZpZXdWYWx1ZTtcblxuICAgIGlmIChpc05hTihkYXRlKSkge1xuICAgICAgbmdNb2RlbEN0cmwuJHNldFZhbGlkaXR5KCd0aW1lJywgZmFsc2UpO1xuICAgICAgJGxvZy5lcnJvcignVGltZXBpY2tlciBkaXJlY3RpdmU6IFwibmctbW9kZWxcIiB2YWx1ZSBtdXN0IGJlIGEgRGF0ZSBvYmplY3QsIGEgbnVtYmVyIG9mIG1pbGxpc2Vjb25kcyBzaW5jZSAwMS4wMS4xOTcwIG9yIGEgc3RyaW5nIHJlcHJlc2VudGluZyBhbiBSRkMyODIyIG9yIElTTyA4NjAxIGRhdGUuJyk7XG4gICAgfSBlbHNlIHtcbiAgICAgIGlmIChkYXRlKSB7XG4gICAgICAgIHNlbGVjdGVkID0gZGF0ZTtcbiAgICAgIH1cblxuICAgICAgaWYgKHNlbGVjdGVkIDwgbWluIHx8IHNlbGVjdGVkID4gbWF4KSB7XG4gICAgICAgIG5nTW9kZWxDdHJsLiRzZXRWYWxpZGl0eSgndGltZScsIGZhbHNlKTtcbiAgICAgICAgJHNjb3BlLmludmFsaWRIb3VycyA9IHRydWU7XG4gICAgICAgICRzY29wZS5pbnZhbGlkTWludXRlcyA9IHRydWU7XG4gICAgICB9IGVsc2Uge1xuICAgICAgICBtYWtlVmFsaWQoKTtcbiAgICAgIH1cbiAgICAgIHVwZGF0ZVRlbXBsYXRlKCk7XG4gICAgfVxuICB9O1xuXG4gIC8vIENhbGwgaW50ZXJuYWxseSB3aGVuIHdlIGtub3cgdGhhdCBtb2RlbCBpcyB2YWxpZC5cbiAgZnVuY3Rpb24gcmVmcmVzaChrZXlib2FyZENoYW5nZSkge1xuICAgIG1ha2VWYWxpZCgpO1xuICAgIG5nTW9kZWxDdHJsLiRzZXRWaWV3VmFsdWUobmV3IERhdGUoc2VsZWN0ZWQpKTtcbiAgICB1cGRhdGVUZW1wbGF0ZShrZXlib2FyZENoYW5nZSk7XG4gIH1cblxuICBmdW5jdGlvbiBtYWtlVmFsaWQoKSB7XG4gICAgbmdNb2RlbEN0cmwuJHNldFZhbGlkaXR5KCd0aW1lJywgdHJ1ZSk7XG4gICAgJHNjb3BlLmludmFsaWRIb3VycyA9IGZhbHNlO1xuICAgICRzY29wZS5pbnZhbGlkTWludXRlcyA9IGZhbHNlO1xuICB9XG5cbiAgZnVuY3Rpb24gdXBkYXRlVGVtcGxhdGUoa2V5Ym9hcmRDaGFuZ2UpIHtcbiAgICB2YXIgaG91cnMgPSBzZWxlY3RlZC5nZXRIb3VycygpLCBtaW51dGVzID0gc2VsZWN0ZWQuZ2V0TWludXRlcygpO1xuXG4gICAgaWYgKCRzY29wZS5zaG93TWVyaWRpYW4pIHtcbiAgICAgIGhvdXJzID0gKGhvdXJzID09PSAwIHx8IGhvdXJzID09PSAxMikgPyAxMiA6IGhvdXJzICUgMTI7IC8vIENvbnZlcnQgMjQgdG8gMTIgaG91ciBzeXN0ZW1cbiAgICB9XG5cbiAgICAkc2NvcGUuaG91cnMgPSBrZXlib2FyZENoYW5nZSA9PT0gJ2gnID8gaG91cnMgOiBwYWQoaG91cnMpO1xuICAgIGlmIChrZXlib2FyZENoYW5nZSAhPT0gJ20nKSB7XG4gICAgICAkc2NvcGUubWludXRlcyA9IHBhZChtaW51dGVzKTtcbiAgICB9XG4gICAgJHNjb3BlLm1lcmlkaWFuID0gc2VsZWN0ZWQuZ2V0SG91cnMoKSA8IDEyID8gbWVyaWRpYW5zWzBdIDogbWVyaWRpYW5zWzFdO1xuICB9XG5cbiAgZnVuY3Rpb24gYWRkTWludXRlcyhkYXRlLCBtaW51dGVzKSB7XG4gICAgdmFyIGR0ID0gbmV3IERhdGUoZGF0ZS5nZXRUaW1lKCkgKyBtaW51dGVzICogNjAwMDApO1xuICAgIHZhciBuZXdEYXRlID0gbmV3IERhdGUoZGF0ZSk7XG4gICAgbmV3RGF0ZS5zZXRIb3VycyhkdC5nZXRIb3VycygpLCBkdC5nZXRNaW51dGVzKCkpO1xuICAgIHJldHVybiBuZXdEYXRlO1xuICB9XG5cbiAgZnVuY3Rpb24gYWRkTWludXRlc1RvU2VsZWN0ZWQobWludXRlcykge1xuICAgIHNlbGVjdGVkID0gYWRkTWludXRlcyhzZWxlY3RlZCwgbWludXRlcyk7XG4gICAgcmVmcmVzaCgpO1xuICB9XG5cbiAgJHNjb3BlLnNob3dTcGlubmVycyA9IGFuZ3VsYXIuaXNEZWZpbmVkKCRhdHRycy5zaG93U3Bpbm5lcnMpID9cbiAgICAkc2NvcGUuJHBhcmVudC4kZXZhbCgkYXR0cnMuc2hvd1NwaW5uZXJzKSA6IHRpbWVwaWNrZXJDb25maWcuc2hvd1NwaW5uZXJzO1xuXG4gICRzY29wZS5pbmNyZW1lbnRIb3VycyA9IGZ1bmN0aW9uKCkge1xuICAgIGlmICghJHNjb3BlLm5vSW5jcmVtZW50SG91cnMoKSkge1xuICAgICAgYWRkTWludXRlc1RvU2VsZWN0ZWQoaG91clN0ZXAgKiA2MCk7XG4gICAgfVxuICB9O1xuXG4gICRzY29wZS5kZWNyZW1lbnRIb3VycyA9IGZ1bmN0aW9uKCkge1xuICAgIGlmICghJHNjb3BlLm5vRGVjcmVtZW50SG91cnMoKSkge1xuICAgICAgYWRkTWludXRlc1RvU2VsZWN0ZWQoLWhvdXJTdGVwICogNjApO1xuICAgIH1cbiAgfTtcblxuICAkc2NvcGUuaW5jcmVtZW50TWludXRlcyA9IGZ1bmN0aW9uKCkge1xuICAgIGlmICghJHNjb3BlLm5vSW5jcmVtZW50TWludXRlcygpKSB7XG4gICAgICBhZGRNaW51dGVzVG9TZWxlY3RlZChtaW51dGVTdGVwKTtcbiAgICB9XG4gIH07XG5cbiAgJHNjb3BlLmRlY3JlbWVudE1pbnV0ZXMgPSBmdW5jdGlvbigpIHtcbiAgICBpZiAoISRzY29wZS5ub0RlY3JlbWVudE1pbnV0ZXMoKSkge1xuICAgICAgYWRkTWludXRlc1RvU2VsZWN0ZWQoLW1pbnV0ZVN0ZXApO1xuICAgIH1cbiAgfTtcblxuICAkc2NvcGUudG9nZ2xlTWVyaWRpYW4gPSBmdW5jdGlvbigpIHtcbiAgICBpZiAoISRzY29wZS5ub1RvZ2dsZU1lcmlkaWFuKCkpIHtcbiAgICAgIGFkZE1pbnV0ZXNUb1NlbGVjdGVkKDEyICogNjAgKiAoc2VsZWN0ZWQuZ2V0SG91cnMoKSA8IDEyID8gMSA6IC0xKSk7XG4gICAgfVxuICB9O1xufV0pXG5cbi5kaXJlY3RpdmUoJ3VpYlRpbWVwaWNrZXInLCBmdW5jdGlvbigpIHtcbiAgcmV0dXJuIHtcbiAgICByZXN0cmljdDogJ0VBJyxcbiAgICByZXF1aXJlOiBbJ3VpYlRpbWVwaWNrZXInLCAnP15uZ01vZGVsJ10sXG4gICAgY29udHJvbGxlcjogJ1VpYlRpbWVwaWNrZXJDb250cm9sbGVyJyxcbiAgICBjb250cm9sbGVyQXM6ICd0aW1lcGlja2VyJyxcbiAgICByZXBsYWNlOiB0cnVlLFxuICAgIHNjb3BlOiB7fSxcbiAgICB0ZW1wbGF0ZVVybDogZnVuY3Rpb24oZWxlbWVudCwgYXR0cnMpIHtcbiAgICAgIHJldHVybiBhdHRycy50ZW1wbGF0ZVVybCB8fCAndGVtcGxhdGUvdGltZXBpY2tlci90aW1lcGlja2VyLmh0bWwnO1xuICAgIH0sXG4gICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHJzLCBjdHJscykge1xuICAgICAgdmFyIHRpbWVwaWNrZXJDdHJsID0gY3RybHNbMF0sIG5nTW9kZWxDdHJsID0gY3RybHNbMV07XG5cbiAgICAgIGlmIChuZ01vZGVsQ3RybCkge1xuICAgICAgICB0aW1lcGlja2VyQ3RybC5pbml0KG5nTW9kZWxDdHJsLCBlbGVtZW50LmZpbmQoJ2lucHV0JykpO1xuICAgICAgfVxuICAgIH1cbiAgfTtcbn0pO1xuXG4vKiBEZXByZWNhdGVkIHRpbWVwaWNrZXIgYmVsb3cgKi9cblxuYW5ndWxhci5tb2R1bGUoJ3VpLmJvb3RzdHJhcC50aW1lcGlja2VyJylcblxuLnZhbHVlKCckdGltZXBpY2tlclN1cHByZXNzV2FybmluZycsIGZhbHNlKVxuXG4uY29udHJvbGxlcignVGltZXBpY2tlckNvbnRyb2xsZXInLCBbJyRzY29wZScsICckZWxlbWVudCcsICckYXR0cnMnLCAnJGNvbnRyb2xsZXInLCAnJGxvZycsICckdGltZXBpY2tlclN1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRzY29wZSwgJGVsZW1lbnQsICRhdHRycywgJGNvbnRyb2xsZXIsICRsb2csICR0aW1lcGlja2VyU3VwcHJlc3NXYXJuaW5nKSB7XG4gIGlmICghJHRpbWVwaWNrZXJTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAkbG9nLndhcm4oJ1RpbWVwaWNrZXJDb250cm9sbGVyIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgVWliVGltZXBpY2tlckNvbnRyb2xsZXIgaW5zdGVhZC4nKTtcbiAgfVxuXG4gIGFuZ3VsYXIuZXh0ZW5kKHRoaXMsICRjb250cm9sbGVyKCdVaWJUaW1lcGlja2VyQ29udHJvbGxlcicsIHtcbiAgICAkc2NvcGU6ICRzY29wZSxcbiAgICAkZWxlbWVudDogJGVsZW1lbnQsXG4gICAgJGF0dHJzOiAkYXR0cnNcbiAgfSkpO1xufV0pXG5cbi5kaXJlY3RpdmUoJ3RpbWVwaWNrZXInLCBbJyRsb2cnLCAnJHRpbWVwaWNrZXJTdXBwcmVzc1dhcm5pbmcnLCBmdW5jdGlvbigkbG9nLCAkdGltZXBpY2tlclN1cHByZXNzV2FybmluZykge1xuICByZXR1cm4ge1xuICAgIHJlc3RyaWN0OiAnRUEnLFxuICAgIHJlcXVpcmU6IFsndGltZXBpY2tlcicsICc/Xm5nTW9kZWwnXSxcbiAgICBjb250cm9sbGVyOiAnVGltZXBpY2tlckNvbnRyb2xsZXInLFxuICAgIGNvbnRyb2xsZXJBczogJ3RpbWVwaWNrZXInLFxuICAgIHJlcGxhY2U6IHRydWUsXG4gICAgc2NvcGU6IHt9LFxuICAgIHRlbXBsYXRlVXJsOiBmdW5jdGlvbihlbGVtZW50LCBhdHRycykge1xuICAgICAgcmV0dXJuIGF0dHJzLnRlbXBsYXRlVXJsIHx8ICd0ZW1wbGF0ZS90aW1lcGlja2VyL3RpbWVwaWNrZXIuaHRtbCc7XG4gICAgfSxcbiAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMsIGN0cmxzKSB7XG4gICAgICBpZiAoISR0aW1lcGlja2VyU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICRsb2cud2FybigndGltZXBpY2tlciBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi10aW1lcGlja2VyIGluc3RlYWQuJyk7XG4gICAgICB9XG4gICAgICB2YXIgdGltZXBpY2tlckN0cmwgPSBjdHJsc1swXSwgbmdNb2RlbEN0cmwgPSBjdHJsc1sxXTtcblxuICAgICAgaWYgKG5nTW9kZWxDdHJsKSB7XG4gICAgICAgIHRpbWVwaWNrZXJDdHJsLmluaXQobmdNb2RlbEN0cmwsIGVsZW1lbnQuZmluZCgnaW5wdXQnKSk7XG4gICAgICB9XG4gICAgfVxuICB9O1xufV0pO1xuXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLnR5cGVhaGVhZCcsIFsndWkuYm9vdHN0cmFwLnBvc2l0aW9uJ10pXG5cbi8qKlxuICogQSBoZWxwZXIgc2VydmljZSB0aGF0IGNhbiBwYXJzZSB0eXBlYWhlYWQncyBzeW50YXggKHN0cmluZyBwcm92aWRlZCBieSB1c2VycylcbiAqIEV4dHJhY3RlZCB0byBhIHNlcGFyYXRlIHNlcnZpY2UgZm9yIGVhc2Ugb2YgdW5pdCB0ZXN0aW5nXG4gKi9cbiAgLmZhY3RvcnkoJ3VpYlR5cGVhaGVhZFBhcnNlcicsIFsnJHBhcnNlJywgZnVuY3Rpb24oJHBhcnNlKSB7XG4gICAgLy8gICAgICAgICAgICAgICAgICAgICAgMDAwMDAxMTEwMDAwMDAwMDAwMDAwMjIyMDAwMDAwMDAwMDAwMDAwMDMzMzMzMzMzMzMzMzMzMzAwMDAwMDAwMDAwNDQwMDBcbiAgICB2YXIgVFlQRUFIRUFEX1JFR0VYUCA9IC9eXFxzKihbXFxzXFxTXSs/KSg/Olxccythc1xccysoW1xcc1xcU10rPykpP1xccytmb3JcXHMrKD86KFtcXCRcXHddW1xcJFxcd1xcZF0qKSlcXHMraW5cXHMrKFtcXHNcXFNdKz8pJC87XG4gICAgcmV0dXJuIHtcbiAgICAgIHBhcnNlOiBmdW5jdGlvbihpbnB1dCkge1xuICAgICAgICB2YXIgbWF0Y2ggPSBpbnB1dC5tYXRjaChUWVBFQUhFQURfUkVHRVhQKTtcbiAgICAgICAgaWYgKCFtYXRjaCkge1xuICAgICAgICAgIHRocm93IG5ldyBFcnJvcihcbiAgICAgICAgICAgICdFeHBlY3RlZCB0eXBlYWhlYWQgc3BlY2lmaWNhdGlvbiBpbiBmb3JtIG9mIFwiX21vZGVsVmFsdWVfIChhcyBfbGFiZWxfKT8gZm9yIF9pdGVtXyBpbiBfY29sbGVjdGlvbl9cIicgK1xuICAgICAgICAgICAgICAnIGJ1dCBnb3QgXCInICsgaW5wdXQgKyAnXCIuJyk7XG4gICAgICAgIH1cblxuICAgICAgICByZXR1cm4ge1xuICAgICAgICAgIGl0ZW1OYW1lOiBtYXRjaFszXSxcbiAgICAgICAgICBzb3VyY2U6ICRwYXJzZShtYXRjaFs0XSksXG4gICAgICAgICAgdmlld01hcHBlcjogJHBhcnNlKG1hdGNoWzJdIHx8IG1hdGNoWzFdKSxcbiAgICAgICAgICBtb2RlbE1hcHBlcjogJHBhcnNlKG1hdGNoWzFdKVxuICAgICAgICB9O1xuICAgICAgfVxuICAgIH07XG4gIH1dKVxuXG4gIC5jb250cm9sbGVyKCdVaWJUeXBlYWhlYWRDb250cm9sbGVyJywgWyckc2NvcGUnLCAnJGVsZW1lbnQnLCAnJGF0dHJzJywgJyRjb21waWxlJywgJyRwYXJzZScsICckcScsICckdGltZW91dCcsICckZG9jdW1lbnQnLCAnJHdpbmRvdycsICckcm9vdFNjb3BlJywgJyR1aWJQb3NpdGlvbicsICd1aWJUeXBlYWhlYWRQYXJzZXInLFxuICAgIGZ1bmN0aW9uKG9yaWdpbmFsU2NvcGUsIGVsZW1lbnQsIGF0dHJzLCAkY29tcGlsZSwgJHBhcnNlLCAkcSwgJHRpbWVvdXQsICRkb2N1bWVudCwgJHdpbmRvdywgJHJvb3RTY29wZSwgJHBvc2l0aW9uLCB0eXBlYWhlYWRQYXJzZXIpIHtcbiAgICB2YXIgSE9UX0tFWVMgPSBbOSwgMTMsIDI3LCAzOCwgNDBdO1xuICAgIHZhciBldmVudERlYm91bmNlVGltZSA9IDIwMDtcbiAgICB2YXIgbW9kZWxDdHJsLCBuZ01vZGVsT3B0aW9ucztcbiAgICAvL1NVUFBPUlRFRCBBVFRSSUJVVEVTIChPUFRJT05TKVxuXG4gICAgLy9taW5pbWFsIG5vIG9mIGNoYXJhY3RlcnMgdGhhdCBuZWVkcyB0byBiZSBlbnRlcmVkIGJlZm9yZSB0eXBlYWhlYWQga2lja3MtaW5cbiAgICB2YXIgbWluTGVuZ3RoID0gb3JpZ2luYWxTY29wZS4kZXZhbChhdHRycy50eXBlYWhlYWRNaW5MZW5ndGgpO1xuICAgIGlmICghbWluTGVuZ3RoICYmIG1pbkxlbmd0aCAhPT0gMCkge1xuICAgICAgbWluTGVuZ3RoID0gMTtcbiAgICB9XG5cbiAgICAvL21pbmltYWwgd2FpdCB0aW1lIGFmdGVyIGxhc3QgY2hhcmFjdGVyIHR5cGVkIGJlZm9yZSB0eXBlYWhlYWQga2lja3MtaW5cbiAgICB2YXIgd2FpdFRpbWUgPSBvcmlnaW5hbFNjb3BlLiRldmFsKGF0dHJzLnR5cGVhaGVhZFdhaXRNcykgfHwgMDtcblxuICAgIC8vc2hvdWxkIGl0IHJlc3RyaWN0IG1vZGVsIHZhbHVlcyB0byB0aGUgb25lcyBzZWxlY3RlZCBmcm9tIHRoZSBwb3B1cCBvbmx5P1xuICAgIHZhciBpc0VkaXRhYmxlID0gb3JpZ2luYWxTY29wZS4kZXZhbChhdHRycy50eXBlYWhlYWRFZGl0YWJsZSkgIT09IGZhbHNlO1xuXG4gICAgLy9iaW5kaW5nIHRvIGEgdmFyaWFibGUgdGhhdCBpbmRpY2F0ZXMgaWYgbWF0Y2hlcyBhcmUgYmVpbmcgcmV0cmlldmVkIGFzeW5jaHJvbm91c2x5XG4gICAgdmFyIGlzTG9hZGluZ1NldHRlciA9ICRwYXJzZShhdHRycy50eXBlYWhlYWRMb2FkaW5nKS5hc3NpZ24gfHwgYW5ndWxhci5ub29wO1xuXG4gICAgLy9hIGNhbGxiYWNrIGV4ZWN1dGVkIHdoZW4gYSBtYXRjaCBpcyBzZWxlY3RlZFxuICAgIHZhciBvblNlbGVjdENhbGxiYWNrID0gJHBhcnNlKGF0dHJzLnR5cGVhaGVhZE9uU2VsZWN0KTtcblxuICAgIC8vc2hvdWxkIGl0IHNlbGVjdCBoaWdobGlnaHRlZCBwb3B1cCB2YWx1ZSB3aGVuIGxvc2luZyBmb2N1cz9cbiAgICB2YXIgaXNTZWxlY3RPbkJsdXIgPSBhbmd1bGFyLmlzRGVmaW5lZChhdHRycy50eXBlYWhlYWRTZWxlY3RPbkJsdXIpID8gb3JpZ2luYWxTY29wZS4kZXZhbChhdHRycy50eXBlYWhlYWRTZWxlY3RPbkJsdXIpIDogZmFsc2U7XG5cbiAgICAvL2JpbmRpbmcgdG8gYSB2YXJpYWJsZSB0aGF0IGluZGljYXRlcyBpZiB0aGVyZSB3ZXJlIG5vIHJlc3VsdHMgYWZ0ZXIgdGhlIHF1ZXJ5IGlzIGNvbXBsZXRlZFxuICAgIHZhciBpc05vUmVzdWx0c1NldHRlciA9ICRwYXJzZShhdHRycy50eXBlYWhlYWROb1Jlc3VsdHMpLmFzc2lnbiB8fCBhbmd1bGFyLm5vb3A7XG5cbiAgICB2YXIgaW5wdXRGb3JtYXR0ZXIgPSBhdHRycy50eXBlYWhlYWRJbnB1dEZvcm1hdHRlciA/ICRwYXJzZShhdHRycy50eXBlYWhlYWRJbnB1dEZvcm1hdHRlcikgOiB1bmRlZmluZWQ7XG5cbiAgICB2YXIgYXBwZW5kVG9Cb2R5ID0gIGF0dHJzLnR5cGVhaGVhZEFwcGVuZFRvQm9keSA/IG9yaWdpbmFsU2NvcGUuJGV2YWwoYXR0cnMudHlwZWFoZWFkQXBwZW5kVG9Cb2R5KSA6IGZhbHNlO1xuXG4gICAgdmFyIGFwcGVuZFRvRWxlbWVudElkID0gIGF0dHJzLnR5cGVhaGVhZEFwcGVuZFRvRWxlbWVudElkIHx8IGZhbHNlO1xuXG4gICAgdmFyIGZvY3VzRmlyc3QgPSBvcmlnaW5hbFNjb3BlLiRldmFsKGF0dHJzLnR5cGVhaGVhZEZvY3VzRmlyc3QpICE9PSBmYWxzZTtcblxuICAgIC8vSWYgaW5wdXQgbWF0Y2hlcyBhbiBpdGVtIG9mIHRoZSBsaXN0IGV4YWN0bHksIHNlbGVjdCBpdCBhdXRvbWF0aWNhbGx5XG4gICAgdmFyIHNlbGVjdE9uRXhhY3QgPSBhdHRycy50eXBlYWhlYWRTZWxlY3RPbkV4YWN0ID8gb3JpZ2luYWxTY29wZS4kZXZhbChhdHRycy50eXBlYWhlYWRTZWxlY3RPbkV4YWN0KSA6IGZhbHNlO1xuXG4gICAgLy9JTlRFUk5BTCBWQVJJQUJMRVNcblxuICAgIC8vbW9kZWwgc2V0dGVyIGV4ZWN1dGVkIHVwb24gbWF0Y2ggc2VsZWN0aW9uXG4gICAgdmFyIHBhcnNlZE1vZGVsID0gJHBhcnNlKGF0dHJzLm5nTW9kZWwpO1xuICAgIHZhciBpbnZva2VNb2RlbFNldHRlciA9ICRwYXJzZShhdHRycy5uZ01vZGVsICsgJygkJCRwKScpO1xuICAgIHZhciAkc2V0TW9kZWxWYWx1ZSA9IGZ1bmN0aW9uKHNjb3BlLCBuZXdWYWx1ZSkge1xuICAgICAgaWYgKGFuZ3VsYXIuaXNGdW5jdGlvbihwYXJzZWRNb2RlbChvcmlnaW5hbFNjb3BlKSkgJiZcbiAgICAgICAgbmdNb2RlbE9wdGlvbnMgJiYgbmdNb2RlbE9wdGlvbnMuJG9wdGlvbnMgJiYgbmdNb2RlbE9wdGlvbnMuJG9wdGlvbnMuZ2V0dGVyU2V0dGVyKSB7XG4gICAgICAgIHJldHVybiBpbnZva2VNb2RlbFNldHRlcihzY29wZSwgeyQkJHA6IG5ld1ZhbHVlfSk7XG4gICAgICB9IGVsc2Uge1xuICAgICAgICByZXR1cm4gcGFyc2VkTW9kZWwuYXNzaWduKHNjb3BlLCBuZXdWYWx1ZSk7XG4gICAgICB9XG4gICAgfTtcblxuICAgIC8vZXhwcmVzc2lvbnMgdXNlZCBieSB0eXBlYWhlYWRcbiAgICB2YXIgcGFyc2VyUmVzdWx0ID0gdHlwZWFoZWFkUGFyc2VyLnBhcnNlKGF0dHJzLnVpYlR5cGVhaGVhZCk7XG5cbiAgICB2YXIgaGFzRm9jdXM7XG5cbiAgICAvL1VzZWQgdG8gYXZvaWQgYnVnIGluIGlPUyB3ZWJ2aWV3IHdoZXJlIGlPUyBrZXlib2FyZCBkb2VzIG5vdCBmaXJlXG4gICAgLy9tb3VzZWRvd24gJiBtb3VzZXVwIGV2ZW50c1xuICAgIC8vSXNzdWUgIzM2OTlcbiAgICB2YXIgc2VsZWN0ZWQ7XG5cbiAgICAvL2NyZWF0ZSBhIGNoaWxkIHNjb3BlIGZvciB0aGUgdHlwZWFoZWFkIGRpcmVjdGl2ZSBzbyB3ZSBhcmUgbm90IHBvbGx1dGluZyBvcmlnaW5hbCBzY29wZVxuICAgIC8vd2l0aCB0eXBlYWhlYWQtc3BlY2lmaWMgZGF0YSAobWF0Y2hlcywgcXVlcnkgZXRjLilcbiAgICB2YXIgc2NvcGUgPSBvcmlnaW5hbFNjb3BlLiRuZXcoKTtcbiAgICB2YXIgb2ZmRGVzdHJveSA9IG9yaWdpbmFsU2NvcGUuJG9uKCckZGVzdHJveScsIGZ1bmN0aW9uKCkge1xuICAgICAgc2NvcGUuJGRlc3Ryb3koKTtcbiAgICB9KTtcbiAgICBzY29wZS4kb24oJyRkZXN0cm95Jywgb2ZmRGVzdHJveSk7XG5cbiAgICAvLyBXQUktQVJJQVxuICAgIHZhciBwb3B1cElkID0gJ3R5cGVhaGVhZC0nICsgc2NvcGUuJGlkICsgJy0nICsgTWF0aC5mbG9vcihNYXRoLnJhbmRvbSgpICogMTAwMDApO1xuICAgIGVsZW1lbnQuYXR0cih7XG4gICAgICAnYXJpYS1hdXRvY29tcGxldGUnOiAnbGlzdCcsXG4gICAgICAnYXJpYS1leHBhbmRlZCc6IGZhbHNlLFxuICAgICAgJ2FyaWEtb3ducyc6IHBvcHVwSWRcbiAgICB9KTtcblxuICAgIC8vcG9wLXVwIGVsZW1lbnQgdXNlZCB0byBkaXNwbGF5IG1hdGNoZXNcbiAgICB2YXIgcG9wVXBFbCA9IGFuZ3VsYXIuZWxlbWVudCgnPGRpdiB1aWItdHlwZWFoZWFkLXBvcHVwPjwvZGl2PicpO1xuICAgIHBvcFVwRWwuYXR0cih7XG4gICAgICBpZDogcG9wdXBJZCxcbiAgICAgIG1hdGNoZXM6ICdtYXRjaGVzJyxcbiAgICAgIGFjdGl2ZTogJ2FjdGl2ZUlkeCcsXG4gICAgICBzZWxlY3Q6ICdzZWxlY3QoYWN0aXZlSWR4KScsXG4gICAgICAnbW92ZS1pbi1wcm9ncmVzcyc6ICdtb3ZlSW5Qcm9ncmVzcycsXG4gICAgICBxdWVyeTogJ3F1ZXJ5JyxcbiAgICAgIHBvc2l0aW9uOiAncG9zaXRpb24nXG4gICAgfSk7XG4gICAgLy9jdXN0b20gaXRlbSB0ZW1wbGF0ZVxuICAgIGlmIChhbmd1bGFyLmlzRGVmaW5lZChhdHRycy50eXBlYWhlYWRUZW1wbGF0ZVVybCkpIHtcbiAgICAgIHBvcFVwRWwuYXR0cigndGVtcGxhdGUtdXJsJywgYXR0cnMudHlwZWFoZWFkVGVtcGxhdGVVcmwpO1xuICAgIH1cblxuICAgIGlmIChhbmd1bGFyLmlzRGVmaW5lZChhdHRycy50eXBlYWhlYWRQb3B1cFRlbXBsYXRlVXJsKSkge1xuICAgICAgcG9wVXBFbC5hdHRyKCdwb3B1cC10ZW1wbGF0ZS11cmwnLCBhdHRycy50eXBlYWhlYWRQb3B1cFRlbXBsYXRlVXJsKTtcbiAgICB9XG5cbiAgICB2YXIgcmVzZXRNYXRjaGVzID0gZnVuY3Rpb24oKSB7XG4gICAgICBzY29wZS5tYXRjaGVzID0gW107XG4gICAgICBzY29wZS5hY3RpdmVJZHggPSAtMTtcbiAgICAgIGVsZW1lbnQuYXR0cignYXJpYS1leHBhbmRlZCcsIGZhbHNlKTtcbiAgICB9O1xuXG4gICAgdmFyIGdldE1hdGNoSWQgPSBmdW5jdGlvbihpbmRleCkge1xuICAgICAgcmV0dXJuIHBvcHVwSWQgKyAnLW9wdGlvbi0nICsgaW5kZXg7XG4gICAgfTtcblxuICAgIC8vIEluZGljYXRlIHRoYXQgdGhlIHNwZWNpZmllZCBtYXRjaCBpcyB0aGUgYWN0aXZlIChwcmUtc2VsZWN0ZWQpIGl0ZW0gaW4gdGhlIGxpc3Qgb3duZWQgYnkgdGhpcyB0eXBlYWhlYWQuXG4gICAgLy8gVGhpcyBhdHRyaWJ1dGUgaXMgYWRkZWQgb3IgcmVtb3ZlZCBhdXRvbWF0aWNhbGx5IHdoZW4gdGhlIGBhY3RpdmVJZHhgIGNoYW5nZXMuXG4gICAgc2NvcGUuJHdhdGNoKCdhY3RpdmVJZHgnLCBmdW5jdGlvbihpbmRleCkge1xuICAgICAgaWYgKGluZGV4IDwgMCkge1xuICAgICAgICBlbGVtZW50LnJlbW92ZUF0dHIoJ2FyaWEtYWN0aXZlZGVzY2VuZGFudCcpO1xuICAgICAgfSBlbHNlIHtcbiAgICAgICAgZWxlbWVudC5hdHRyKCdhcmlhLWFjdGl2ZWRlc2NlbmRhbnQnLCBnZXRNYXRjaElkKGluZGV4KSk7XG4gICAgICB9XG4gICAgfSk7XG5cbiAgICB2YXIgaW5wdXRJc0V4YWN0TWF0Y2ggPSBmdW5jdGlvbihpbnB1dFZhbHVlLCBpbmRleCkge1xuICAgICAgaWYgKHNjb3BlLm1hdGNoZXMubGVuZ3RoID4gaW5kZXggJiYgaW5wdXRWYWx1ZSkge1xuICAgICAgICByZXR1cm4gaW5wdXRWYWx1ZS50b1VwcGVyQ2FzZSgpID09PSBzY29wZS5tYXRjaGVzW2luZGV4XS5sYWJlbC50b1VwcGVyQ2FzZSgpO1xuICAgICAgfVxuXG4gICAgICByZXR1cm4gZmFsc2U7XG4gICAgfTtcblxuICAgIHZhciBnZXRNYXRjaGVzQXN5bmMgPSBmdW5jdGlvbihpbnB1dFZhbHVlKSB7XG4gICAgICB2YXIgbG9jYWxzID0geyR2aWV3VmFsdWU6IGlucHV0VmFsdWV9O1xuICAgICAgaXNMb2FkaW5nU2V0dGVyKG9yaWdpbmFsU2NvcGUsIHRydWUpO1xuICAgICAgaXNOb1Jlc3VsdHNTZXR0ZXIob3JpZ2luYWxTY29wZSwgZmFsc2UpO1xuICAgICAgJHEud2hlbihwYXJzZXJSZXN1bHQuc291cmNlKG9yaWdpbmFsU2NvcGUsIGxvY2FscykpLnRoZW4oZnVuY3Rpb24obWF0Y2hlcykge1xuICAgICAgICAvL2l0IG1pZ2h0IGhhcHBlbiB0aGF0IHNldmVyYWwgYXN5bmMgcXVlcmllcyB3ZXJlIGluIHByb2dyZXNzIGlmIGEgdXNlciB3ZXJlIHR5cGluZyBmYXN0XG4gICAgICAgIC8vYnV0IHdlIGFyZSBpbnRlcmVzdGVkIG9ubHkgaW4gcmVzcG9uc2VzIHRoYXQgY29ycmVzcG9uZCB0byB0aGUgY3VycmVudCB2aWV3IHZhbHVlXG4gICAgICAgIHZhciBvbkN1cnJlbnRSZXF1ZXN0ID0gKGlucHV0VmFsdWUgPT09IG1vZGVsQ3RybC4kdmlld1ZhbHVlKTtcbiAgICAgICAgaWYgKG9uQ3VycmVudFJlcXVlc3QgJiYgaGFzRm9jdXMpIHtcbiAgICAgICAgICBpZiAobWF0Y2hlcyAmJiBtYXRjaGVzLmxlbmd0aCA+IDApIHtcbiAgICAgICAgICAgIHNjb3BlLmFjdGl2ZUlkeCA9IGZvY3VzRmlyc3QgPyAwIDogLTE7XG4gICAgICAgICAgICBpc05vUmVzdWx0c1NldHRlcihvcmlnaW5hbFNjb3BlLCBmYWxzZSk7XG4gICAgICAgICAgICBzY29wZS5tYXRjaGVzLmxlbmd0aCA9IDA7XG5cbiAgICAgICAgICAgIC8vdHJhbnNmb3JtIGxhYmVsc1xuICAgICAgICAgICAgZm9yICh2YXIgaSA9IDA7IGkgPCBtYXRjaGVzLmxlbmd0aDsgaSsrKSB7XG4gICAgICAgICAgICAgIGxvY2Fsc1twYXJzZXJSZXN1bHQuaXRlbU5hbWVdID0gbWF0Y2hlc1tpXTtcbiAgICAgICAgICAgICAgc2NvcGUubWF0Y2hlcy5wdXNoKHtcbiAgICAgICAgICAgICAgICBpZDogZ2V0TWF0Y2hJZChpKSxcbiAgICAgICAgICAgICAgICBsYWJlbDogcGFyc2VyUmVzdWx0LnZpZXdNYXBwZXIoc2NvcGUsIGxvY2FscyksXG4gICAgICAgICAgICAgICAgbW9kZWw6IG1hdGNoZXNbaV1cbiAgICAgICAgICAgICAgfSk7XG4gICAgICAgICAgICB9XG5cbiAgICAgICAgICAgIHNjb3BlLnF1ZXJ5ID0gaW5wdXRWYWx1ZTtcbiAgICAgICAgICAgIC8vcG9zaXRpb24gcG9wLXVwIHdpdGggbWF0Y2hlcyAtIHdlIG5lZWQgdG8gcmUtY2FsY3VsYXRlIGl0cyBwb3NpdGlvbiBlYWNoIHRpbWUgd2UgYXJlIG9wZW5pbmcgYSB3aW5kb3dcbiAgICAgICAgICAgIC8vd2l0aCBtYXRjaGVzIGFzIGEgcG9wLXVwIG1pZ2h0IGJlIGFic29sdXRlLXBvc2l0aW9uZWQgYW5kIHBvc2l0aW9uIG9mIGFuIGlucHV0IG1pZ2h0IGhhdmUgY2hhbmdlZCBvbiBhIHBhZ2VcbiAgICAgICAgICAgIC8vZHVlIHRvIG90aGVyIGVsZW1lbnRzIGJlaW5nIHJlbmRlcmVkXG4gICAgICAgICAgICByZWNhbGN1bGF0ZVBvc2l0aW9uKCk7XG5cbiAgICAgICAgICAgIGVsZW1lbnQuYXR0cignYXJpYS1leHBhbmRlZCcsIHRydWUpO1xuXG4gICAgICAgICAgICAvL1NlbGVjdCB0aGUgc2luZ2xlIHJlbWFpbmluZyBvcHRpb24gaWYgdXNlciBpbnB1dCBtYXRjaGVzXG4gICAgICAgICAgICBpZiAoc2VsZWN0T25FeGFjdCAmJiBzY29wZS5tYXRjaGVzLmxlbmd0aCA9PT0gMSAmJiBpbnB1dElzRXhhY3RNYXRjaChpbnB1dFZhbHVlLCAwKSkge1xuICAgICAgICAgICAgICBzY29wZS5zZWxlY3QoMCk7XG4gICAgICAgICAgICB9XG4gICAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICAgIHJlc2V0TWF0Y2hlcygpO1xuICAgICAgICAgICAgaXNOb1Jlc3VsdHNTZXR0ZXIob3JpZ2luYWxTY29wZSwgdHJ1ZSk7XG4gICAgICAgICAgfVxuICAgICAgICB9XG4gICAgICAgIGlmIChvbkN1cnJlbnRSZXF1ZXN0KSB7XG4gICAgICAgICAgaXNMb2FkaW5nU2V0dGVyKG9yaWdpbmFsU2NvcGUsIGZhbHNlKTtcbiAgICAgICAgfVxuICAgICAgfSwgZnVuY3Rpb24oKSB7XG4gICAgICAgIHJlc2V0TWF0Y2hlcygpO1xuICAgICAgICBpc0xvYWRpbmdTZXR0ZXIob3JpZ2luYWxTY29wZSwgZmFsc2UpO1xuICAgICAgICBpc05vUmVzdWx0c1NldHRlcihvcmlnaW5hbFNjb3BlLCB0cnVlKTtcbiAgICAgIH0pO1xuICAgIH07XG5cbiAgICAvLyBiaW5kIGV2ZW50cyBvbmx5IGlmIGFwcGVuZFRvQm9keSBwYXJhbXMgZXhpc3QgLSBwZXJmb3JtYW5jZSBmZWF0dXJlXG4gICAgaWYgKGFwcGVuZFRvQm9keSkge1xuICAgICAgYW5ndWxhci5lbGVtZW50KCR3aW5kb3cpLmJpbmQoJ3Jlc2l6ZScsIGZpcmVSZWNhbGN1bGF0aW5nKTtcbiAgICAgICRkb2N1bWVudC5maW5kKCdib2R5JykuYmluZCgnc2Nyb2xsJywgZmlyZVJlY2FsY3VsYXRpbmcpO1xuICAgIH1cblxuICAgIC8vIERlY2xhcmUgdGhlIHRpbWVvdXQgcHJvbWlzZSB2YXIgb3V0c2lkZSB0aGUgZnVuY3Rpb24gc2NvcGUgc28gdGhhdCBzdGFja2VkIGNhbGxzIGNhbiBiZSBjYW5jZWxsZWQgbGF0ZXJcbiAgICB2YXIgdGltZW91dEV2ZW50UHJvbWlzZTtcblxuICAgIC8vIERlZmF1bHQgcHJvZ3Jlc3MgdHlwZVxuICAgIHNjb3BlLm1vdmVJblByb2dyZXNzID0gZmFsc2U7XG5cbiAgICBmdW5jdGlvbiBmaXJlUmVjYWxjdWxhdGluZygpIHtcbiAgICAgIGlmICghc2NvcGUubW92ZUluUHJvZ3Jlc3MpIHtcbiAgICAgICAgc2NvcGUubW92ZUluUHJvZ3Jlc3MgPSB0cnVlO1xuICAgICAgICBzY29wZS4kZGlnZXN0KCk7XG4gICAgICB9XG5cbiAgICAgIC8vIENhbmNlbCBwcmV2aW91cyB0aW1lb3V0XG4gICAgICBpZiAodGltZW91dEV2ZW50UHJvbWlzZSkge1xuICAgICAgICAkdGltZW91dC5jYW5jZWwodGltZW91dEV2ZW50UHJvbWlzZSk7XG4gICAgICB9XG5cbiAgICAgIC8vIERlYm91bmNlZCBleGVjdXRpbmcgcmVjYWxjdWxhdGUgYWZ0ZXIgZXZlbnRzIGZpcmVkXG4gICAgICB0aW1lb3V0RXZlbnRQcm9taXNlID0gJHRpbWVvdXQoZnVuY3Rpb24oKSB7XG4gICAgICAgIC8vIGlmIHBvcHVwIGlzIHZpc2libGVcbiAgICAgICAgaWYgKHNjb3BlLm1hdGNoZXMubGVuZ3RoKSB7XG4gICAgICAgICAgcmVjYWxjdWxhdGVQb3NpdGlvbigpO1xuICAgICAgICB9XG5cbiAgICAgICAgc2NvcGUubW92ZUluUHJvZ3Jlc3MgPSBmYWxzZTtcbiAgICAgIH0sIGV2ZW50RGVib3VuY2VUaW1lKTtcbiAgICB9XG5cbiAgICAvLyByZWNhbGN1bGF0ZSBhY3R1YWwgcG9zaXRpb24gYW5kIHNldCBuZXcgdmFsdWVzIHRvIHNjb3BlXG4gICAgLy8gYWZ0ZXIgZGlnZXN0IGxvb3AgaXMgcG9wdXAgaW4gcmlnaHQgcG9zaXRpb25cbiAgICBmdW5jdGlvbiByZWNhbGN1bGF0ZVBvc2l0aW9uKCkge1xuICAgICAgc2NvcGUucG9zaXRpb24gPSBhcHBlbmRUb0JvZHkgPyAkcG9zaXRpb24ub2Zmc2V0KGVsZW1lbnQpIDogJHBvc2l0aW9uLnBvc2l0aW9uKGVsZW1lbnQpO1xuICAgICAgc2NvcGUucG9zaXRpb24udG9wICs9IGVsZW1lbnQucHJvcCgnb2Zmc2V0SGVpZ2h0Jyk7XG4gICAgfVxuXG4gICAgLy93ZSBuZWVkIHRvIHByb3BhZ2F0ZSB1c2VyJ3MgcXVlcnkgc28gd2UgY2FuIGhpZ2xpZ2h0IG1hdGNoZXNcbiAgICBzY29wZS5xdWVyeSA9IHVuZGVmaW5lZDtcblxuICAgIC8vRGVjbGFyZSB0aGUgdGltZW91dCBwcm9taXNlIHZhciBvdXRzaWRlIHRoZSBmdW5jdGlvbiBzY29wZSBzbyB0aGF0IHN0YWNrZWQgY2FsbHMgY2FuIGJlIGNhbmNlbGxlZCBsYXRlclxuICAgIHZhciB0aW1lb3V0UHJvbWlzZTtcblxuICAgIHZhciBzY2hlZHVsZVNlYXJjaFdpdGhUaW1lb3V0ID0gZnVuY3Rpb24oaW5wdXRWYWx1ZSkge1xuICAgICAgdGltZW91dFByb21pc2UgPSAkdGltZW91dChmdW5jdGlvbigpIHtcbiAgICAgICAgZ2V0TWF0Y2hlc0FzeW5jKGlucHV0VmFsdWUpO1xuICAgICAgfSwgd2FpdFRpbWUpO1xuICAgIH07XG5cbiAgICB2YXIgY2FuY2VsUHJldmlvdXNUaW1lb3V0ID0gZnVuY3Rpb24oKSB7XG4gICAgICBpZiAodGltZW91dFByb21pc2UpIHtcbiAgICAgICAgJHRpbWVvdXQuY2FuY2VsKHRpbWVvdXRQcm9taXNlKTtcbiAgICAgIH1cbiAgICB9O1xuXG4gICAgcmVzZXRNYXRjaGVzKCk7XG5cbiAgICBzY29wZS5zZWxlY3QgPSBmdW5jdGlvbihhY3RpdmVJZHgpIHtcbiAgICAgIC8vY2FsbGVkIGZyb20gd2l0aGluIHRoZSAkZGlnZXN0KCkgY3ljbGVcbiAgICAgIHZhciBsb2NhbHMgPSB7fTtcbiAgICAgIHZhciBtb2RlbCwgaXRlbTtcblxuICAgICAgc2VsZWN0ZWQgPSB0cnVlO1xuICAgICAgbG9jYWxzW3BhcnNlclJlc3VsdC5pdGVtTmFtZV0gPSBpdGVtID0gc2NvcGUubWF0Y2hlc1thY3RpdmVJZHhdLm1vZGVsO1xuICAgICAgbW9kZWwgPSBwYXJzZXJSZXN1bHQubW9kZWxNYXBwZXIob3JpZ2luYWxTY29wZSwgbG9jYWxzKTtcbiAgICAgICRzZXRNb2RlbFZhbHVlKG9yaWdpbmFsU2NvcGUsIG1vZGVsKTtcbiAgICAgIG1vZGVsQ3RybC4kc2V0VmFsaWRpdHkoJ2VkaXRhYmxlJywgdHJ1ZSk7XG4gICAgICBtb2RlbEN0cmwuJHNldFZhbGlkaXR5KCdwYXJzZScsIHRydWUpO1xuXG4gICAgICBvblNlbGVjdENhbGxiYWNrKG9yaWdpbmFsU2NvcGUsIHtcbiAgICAgICAgJGl0ZW06IGl0ZW0sXG4gICAgICAgICRtb2RlbDogbW9kZWwsXG4gICAgICAgICRsYWJlbDogcGFyc2VyUmVzdWx0LnZpZXdNYXBwZXIob3JpZ2luYWxTY29wZSwgbG9jYWxzKVxuICAgICAgfSk7XG5cbiAgICAgIHJlc2V0TWF0Y2hlcygpO1xuXG4gICAgICAvL3JldHVybiBmb2N1cyB0byB0aGUgaW5wdXQgZWxlbWVudCBpZiBhIG1hdGNoIHdhcyBzZWxlY3RlZCB2aWEgYSBtb3VzZSBjbGljayBldmVudFxuICAgICAgLy8gdXNlIHRpbWVvdXQgdG8gYXZvaWQgJHJvb3RTY29wZTppbnByb2cgZXJyb3JcbiAgICAgIGlmIChzY29wZS4kZXZhbChhdHRycy50eXBlYWhlYWRGb2N1c09uU2VsZWN0KSAhPT0gZmFsc2UpIHtcbiAgICAgICAgJHRpbWVvdXQoZnVuY3Rpb24oKSB7IGVsZW1lbnRbMF0uZm9jdXMoKTsgfSwgMCwgZmFsc2UpO1xuICAgICAgfVxuICAgIH07XG5cbiAgICAvL2JpbmQga2V5Ym9hcmQgZXZlbnRzOiBhcnJvd3MgdXAoMzgpIC8gZG93big0MCksIGVudGVyKDEzKSBhbmQgdGFiKDkpLCBlc2MoMjcpXG4gICAgZWxlbWVudC5iaW5kKCdrZXlkb3duJywgZnVuY3Rpb24oZXZ0KSB7XG4gICAgICAvL3R5cGVhaGVhZCBpcyBvcGVuIGFuZCBhbiBcImludGVyZXN0aW5nXCIga2V5IHdhcyBwcmVzc2VkXG4gICAgICBpZiAoc2NvcGUubWF0Y2hlcy5sZW5ndGggPT09IDAgfHwgSE9UX0tFWVMuaW5kZXhPZihldnQud2hpY2gpID09PSAtMSkge1xuICAgICAgICByZXR1cm47XG4gICAgICB9XG5cbiAgICAgIC8vIGlmIHRoZXJlJ3Mgbm90aGluZyBzZWxlY3RlZCAoaS5lLiBmb2N1c0ZpcnN0KSBhbmQgZW50ZXIgb3IgdGFiIGlzIGhpdCwgY2xlYXIgdGhlIHJlc3VsdHNcbiAgICAgIGlmIChzY29wZS5hY3RpdmVJZHggPT09IC0xICYmIChldnQud2hpY2ggPT09IDkgfHwgZXZ0LndoaWNoID09PSAxMykpIHtcbiAgICAgICAgcmVzZXRNYXRjaGVzKCk7XG4gICAgICAgIHNjb3BlLiRkaWdlc3QoKTtcbiAgICAgICAgcmV0dXJuO1xuICAgICAgfVxuXG4gICAgICBldnQucHJldmVudERlZmF1bHQoKTtcblxuICAgICAgaWYgKGV2dC53aGljaCA9PT0gNDApIHtcbiAgICAgICAgc2NvcGUuYWN0aXZlSWR4ID0gKHNjb3BlLmFjdGl2ZUlkeCArIDEpICUgc2NvcGUubWF0Y2hlcy5sZW5ndGg7XG4gICAgICAgIHNjb3BlLiRkaWdlc3QoKTtcbiAgICAgIH0gZWxzZSBpZiAoZXZ0LndoaWNoID09PSAzOCkge1xuICAgICAgICBzY29wZS5hY3RpdmVJZHggPSAoc2NvcGUuYWN0aXZlSWR4ID4gMCA/IHNjb3BlLmFjdGl2ZUlkeCA6IHNjb3BlLm1hdGNoZXMubGVuZ3RoKSAtIDE7XG4gICAgICAgIHNjb3BlLiRkaWdlc3QoKTtcbiAgICAgIH0gZWxzZSBpZiAoZXZ0LndoaWNoID09PSAxMyB8fCBldnQud2hpY2ggPT09IDkpIHtcbiAgICAgICAgc2NvcGUuJGFwcGx5KGZ1bmN0aW9uICgpIHtcbiAgICAgICAgICBzY29wZS5zZWxlY3Qoc2NvcGUuYWN0aXZlSWR4KTtcbiAgICAgICAgfSk7XG4gICAgICB9IGVsc2UgaWYgKGV2dC53aGljaCA9PT0gMjcpIHtcbiAgICAgICAgZXZ0LnN0b3BQcm9wYWdhdGlvbigpO1xuXG4gICAgICAgIHJlc2V0TWF0Y2hlcygpO1xuICAgICAgICBzY29wZS4kZGlnZXN0KCk7XG4gICAgICB9XG4gICAgfSk7XG5cbiAgICBlbGVtZW50LmJpbmQoJ2JsdXInLCBmdW5jdGlvbigpIHtcbiAgICAgIGlmIChpc1NlbGVjdE9uQmx1ciAmJiBzY29wZS5tYXRjaGVzLmxlbmd0aCAmJiBzY29wZS5hY3RpdmVJZHggIT09IC0xICYmICFzZWxlY3RlZCkge1xuICAgICAgICBzZWxlY3RlZCA9IHRydWU7XG4gICAgICAgIHNjb3BlLiRhcHBseShmdW5jdGlvbigpIHtcbiAgICAgICAgICBzY29wZS5zZWxlY3Qoc2NvcGUuYWN0aXZlSWR4KTtcbiAgICAgICAgfSk7XG4gICAgICB9XG4gICAgICBoYXNGb2N1cyA9IGZhbHNlO1xuICAgICAgc2VsZWN0ZWQgPSBmYWxzZTtcbiAgICB9KTtcblxuICAgIC8vIEtlZXAgcmVmZXJlbmNlIHRvIGNsaWNrIGhhbmRsZXIgdG8gdW5iaW5kIGl0LlxuICAgIHZhciBkaXNtaXNzQ2xpY2tIYW5kbGVyID0gZnVuY3Rpb24oZXZ0KSB7XG4gICAgICAvLyBJc3N1ZSAjMzk3M1xuICAgICAgLy8gRmlyZWZveCB0cmVhdHMgcmlnaHQgY2xpY2sgYXMgYSBjbGljayBvbiBkb2N1bWVudFxuICAgICAgaWYgKGVsZW1lbnRbMF0gIT09IGV2dC50YXJnZXQgJiYgZXZ0LndoaWNoICE9PSAzICYmIHNjb3BlLm1hdGNoZXMubGVuZ3RoICE9PSAwKSB7XG4gICAgICAgIHJlc2V0TWF0Y2hlcygpO1xuICAgICAgICBpZiAoISRyb290U2NvcGUuJCRwaGFzZSkge1xuICAgICAgICAgIHNjb3BlLiRkaWdlc3QoKTtcbiAgICAgICAgfVxuICAgICAgfVxuICAgIH07XG5cbiAgICAkZG9jdW1lbnQuYmluZCgnY2xpY2snLCBkaXNtaXNzQ2xpY2tIYW5kbGVyKTtcblxuICAgIG9yaWdpbmFsU2NvcGUuJG9uKCckZGVzdHJveScsIGZ1bmN0aW9uKCkge1xuICAgICAgJGRvY3VtZW50LnVuYmluZCgnY2xpY2snLCBkaXNtaXNzQ2xpY2tIYW5kbGVyKTtcbiAgICAgIGlmIChhcHBlbmRUb0JvZHkgfHwgYXBwZW5kVG9FbGVtZW50SWQpIHtcbiAgICAgICAgJHBvcHVwLnJlbW92ZSgpO1xuICAgICAgfVxuXG4gICAgICBpZiAoYXBwZW5kVG9Cb2R5KSB7XG4gICAgICAgIGFuZ3VsYXIuZWxlbWVudCgkd2luZG93KS51bmJpbmQoJ3Jlc2l6ZScsIGZpcmVSZWNhbGN1bGF0aW5nKTtcbiAgICAgICAgJGRvY3VtZW50LmZpbmQoJ2JvZHknKS51bmJpbmQoJ3Njcm9sbCcsIGZpcmVSZWNhbGN1bGF0aW5nKTtcbiAgICAgIH1cbiAgICAgIC8vIFByZXZlbnQgalF1ZXJ5IGNhY2hlIG1lbW9yeSBsZWFrXG4gICAgICBwb3BVcEVsLnJlbW92ZSgpO1xuICAgIH0pO1xuXG4gICAgdmFyICRwb3B1cCA9ICRjb21waWxlKHBvcFVwRWwpKHNjb3BlKTtcblxuICAgIGlmIChhcHBlbmRUb0JvZHkpIHtcbiAgICAgICRkb2N1bWVudC5maW5kKCdib2R5JykuYXBwZW5kKCRwb3B1cCk7XG4gICAgfSBlbHNlIGlmIChhcHBlbmRUb0VsZW1lbnRJZCAhPT0gZmFsc2UpIHtcbiAgICAgIGFuZ3VsYXIuZWxlbWVudCgkZG9jdW1lbnRbMF0uZ2V0RWxlbWVudEJ5SWQoYXBwZW5kVG9FbGVtZW50SWQpKS5hcHBlbmQoJHBvcHVwKTtcbiAgICB9IGVsc2Uge1xuICAgICAgZWxlbWVudC5hZnRlcigkcG9wdXApO1xuICAgIH1cblxuICAgIHRoaXMuaW5pdCA9IGZ1bmN0aW9uKF9tb2RlbEN0cmwsIF9uZ01vZGVsT3B0aW9ucykge1xuICAgICAgbW9kZWxDdHJsID0gX21vZGVsQ3RybDtcbiAgICAgIG5nTW9kZWxPcHRpb25zID0gX25nTW9kZWxPcHRpb25zO1xuXG4gICAgICAvL3BsdWcgaW50byAkcGFyc2VycyBwaXBlbGluZSB0byBvcGVuIGEgdHlwZWFoZWFkIG9uIHZpZXcgY2hhbmdlcyBpbml0aWF0ZWQgZnJvbSBET01cbiAgICAgIC8vJHBhcnNlcnMga2ljay1pbiBvbiBhbGwgdGhlIGNoYW5nZXMgY29taW5nIGZyb20gdGhlIHZpZXcgYXMgd2VsbCBhcyBtYW51YWxseSB0cmlnZ2VyZWQgYnkgJHNldFZpZXdWYWx1ZVxuICAgICAgbW9kZWxDdHJsLiRwYXJzZXJzLnVuc2hpZnQoZnVuY3Rpb24oaW5wdXRWYWx1ZSkge1xuICAgICAgICBoYXNGb2N1cyA9IHRydWU7XG5cbiAgICAgICAgaWYgKG1pbkxlbmd0aCA9PT0gMCB8fCBpbnB1dFZhbHVlICYmIGlucHV0VmFsdWUubGVuZ3RoID49IG1pbkxlbmd0aCkge1xuICAgICAgICAgIGlmICh3YWl0VGltZSA+IDApIHtcbiAgICAgICAgICAgIGNhbmNlbFByZXZpb3VzVGltZW91dCgpO1xuICAgICAgICAgICAgc2NoZWR1bGVTZWFyY2hXaXRoVGltZW91dChpbnB1dFZhbHVlKTtcbiAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgZ2V0TWF0Y2hlc0FzeW5jKGlucHV0VmFsdWUpO1xuICAgICAgICAgIH1cbiAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICBpc0xvYWRpbmdTZXR0ZXIob3JpZ2luYWxTY29wZSwgZmFsc2UpO1xuICAgICAgICAgIGNhbmNlbFByZXZpb3VzVGltZW91dCgpO1xuICAgICAgICAgIHJlc2V0TWF0Y2hlcygpO1xuICAgICAgICB9XG5cbiAgICAgICAgaWYgKGlzRWRpdGFibGUpIHtcbiAgICAgICAgICByZXR1cm4gaW5wdXRWYWx1ZTtcbiAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICBpZiAoIWlucHV0VmFsdWUpIHtcbiAgICAgICAgICAgIC8vIFJlc2V0IGluIGNhc2UgdXNlciBoYWQgdHlwZWQgc29tZXRoaW5nIHByZXZpb3VzbHkuXG4gICAgICAgICAgICBtb2RlbEN0cmwuJHNldFZhbGlkaXR5KCdlZGl0YWJsZScsIHRydWUpO1xuICAgICAgICAgICAgcmV0dXJuIG51bGw7XG4gICAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICAgIG1vZGVsQ3RybC4kc2V0VmFsaWRpdHkoJ2VkaXRhYmxlJywgZmFsc2UpO1xuICAgICAgICAgICAgcmV0dXJuIHVuZGVmaW5lZDtcbiAgICAgICAgICB9XG4gICAgICAgIH1cbiAgICAgIH0pO1xuXG4gICAgICBtb2RlbEN0cmwuJGZvcm1hdHRlcnMucHVzaChmdW5jdGlvbihtb2RlbFZhbHVlKSB7XG4gICAgICAgIHZhciBjYW5kaWRhdGVWaWV3VmFsdWUsIGVtcHR5Vmlld1ZhbHVlO1xuICAgICAgICB2YXIgbG9jYWxzID0ge307XG5cbiAgICAgICAgLy8gVGhlIHZhbGlkaXR5IG1heSBiZSBzZXQgdG8gZmFsc2UgdmlhICRwYXJzZXJzIChzZWUgYWJvdmUpIGlmXG4gICAgICAgIC8vIHRoZSBtb2RlbCBpcyByZXN0cmljdGVkIHRvIHNlbGVjdGVkIHZhbHVlcy4gSWYgdGhlIG1vZGVsXG4gICAgICAgIC8vIGlzIHNldCBtYW51YWxseSBpdCBpcyBjb25zaWRlcmVkIHRvIGJlIHZhbGlkLlxuICAgICAgICBpZiAoIWlzRWRpdGFibGUpIHtcbiAgICAgICAgICBtb2RlbEN0cmwuJHNldFZhbGlkaXR5KCdlZGl0YWJsZScsIHRydWUpO1xuICAgICAgICB9XG5cbiAgICAgICAgaWYgKGlucHV0Rm9ybWF0dGVyKSB7XG4gICAgICAgICAgbG9jYWxzLiRtb2RlbCA9IG1vZGVsVmFsdWU7XG4gICAgICAgICAgcmV0dXJuIGlucHV0Rm9ybWF0dGVyKG9yaWdpbmFsU2NvcGUsIGxvY2Fscyk7XG4gICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgLy9pdCBtaWdodCBoYXBwZW4gdGhhdCB3ZSBkb24ndCBoYXZlIGVub3VnaCBpbmZvIHRvIHByb3Blcmx5IHJlbmRlciBpbnB1dCB2YWx1ZVxuICAgICAgICAgIC8vd2UgbmVlZCB0byBjaGVjayBmb3IgdGhpcyBzaXR1YXRpb24gYW5kIHNpbXBseSByZXR1cm4gbW9kZWwgdmFsdWUgaWYgd2UgY2FuJ3QgYXBwbHkgY3VzdG9tIGZvcm1hdHRpbmdcbiAgICAgICAgICBsb2NhbHNbcGFyc2VyUmVzdWx0Lml0ZW1OYW1lXSA9IG1vZGVsVmFsdWU7XG4gICAgICAgICAgY2FuZGlkYXRlVmlld1ZhbHVlID0gcGFyc2VyUmVzdWx0LnZpZXdNYXBwZXIob3JpZ2luYWxTY29wZSwgbG9jYWxzKTtcbiAgICAgICAgICBsb2NhbHNbcGFyc2VyUmVzdWx0Lml0ZW1OYW1lXSA9IHVuZGVmaW5lZDtcbiAgICAgICAgICBlbXB0eVZpZXdWYWx1ZSA9IHBhcnNlclJlc3VsdC52aWV3TWFwcGVyKG9yaWdpbmFsU2NvcGUsIGxvY2Fscyk7XG5cbiAgICAgICAgICByZXR1cm4gY2FuZGlkYXRlVmlld1ZhbHVlICE9PSBlbXB0eVZpZXdWYWx1ZSA/IGNhbmRpZGF0ZVZpZXdWYWx1ZSA6IG1vZGVsVmFsdWU7XG4gICAgICAgIH1cbiAgICAgIH0pO1xuICAgIH07XG4gIH1dKVxuXG4gIC5kaXJlY3RpdmUoJ3VpYlR5cGVhaGVhZCcsIGZ1bmN0aW9uKCkge1xuICAgIHJldHVybiB7XG4gICAgICBjb250cm9sbGVyOiAnVWliVHlwZWFoZWFkQ29udHJvbGxlcicsXG4gICAgICByZXF1aXJlOiBbJ25nTW9kZWwnLCAnXj9uZ01vZGVsT3B0aW9ucycsICd1aWJUeXBlYWhlYWQnXSxcbiAgICAgIGxpbms6IGZ1bmN0aW9uKG9yaWdpbmFsU2NvcGUsIGVsZW1lbnQsIGF0dHJzLCBjdHJscykge1xuICAgICAgICBjdHJsc1syXS5pbml0KGN0cmxzWzBdLCBjdHJsc1sxXSk7XG4gICAgICB9XG4gICAgfTtcbiAgfSlcblxuICAuZGlyZWN0aXZlKCd1aWJUeXBlYWhlYWRQb3B1cCcsIGZ1bmN0aW9uKCkge1xuICAgIHJldHVybiB7XG4gICAgICBzY29wZToge1xuICAgICAgICBtYXRjaGVzOiAnPScsXG4gICAgICAgIHF1ZXJ5OiAnPScsXG4gICAgICAgIGFjdGl2ZTogJz0nLFxuICAgICAgICBwb3NpdGlvbjogJyYnLFxuICAgICAgICBtb3ZlSW5Qcm9ncmVzczogJz0nLFxuICAgICAgICBzZWxlY3Q6ICcmJ1xuICAgICAgfSxcbiAgICAgIHJlcGxhY2U6IHRydWUsXG4gICAgICB0ZW1wbGF0ZVVybDogZnVuY3Rpb24oZWxlbWVudCwgYXR0cnMpIHtcbiAgICAgICAgcmV0dXJuIGF0dHJzLnBvcHVwVGVtcGxhdGVVcmwgfHwgJ3RlbXBsYXRlL3R5cGVhaGVhZC90eXBlYWhlYWQtcG9wdXAuaHRtbCc7XG4gICAgICB9LFxuICAgICAgbGluazogZnVuY3Rpb24oc2NvcGUsIGVsZW1lbnQsIGF0dHJzKSB7XG4gICAgICAgIHNjb3BlLnRlbXBsYXRlVXJsID0gYXR0cnMudGVtcGxhdGVVcmw7XG5cbiAgICAgICAgc2NvcGUuaXNPcGVuID0gZnVuY3Rpb24oKSB7XG4gICAgICAgICAgcmV0dXJuIHNjb3BlLm1hdGNoZXMubGVuZ3RoID4gMDtcbiAgICAgICAgfTtcblxuICAgICAgICBzY29wZS5pc0FjdGl2ZSA9IGZ1bmN0aW9uKG1hdGNoSWR4KSB7XG4gICAgICAgICAgcmV0dXJuIHNjb3BlLmFjdGl2ZSA9PSBtYXRjaElkeDtcbiAgICAgICAgfTtcblxuICAgICAgICBzY29wZS5zZWxlY3RBY3RpdmUgPSBmdW5jdGlvbihtYXRjaElkeCkge1xuICAgICAgICAgIHNjb3BlLmFjdGl2ZSA9IG1hdGNoSWR4O1xuICAgICAgICB9O1xuXG4gICAgICAgIHNjb3BlLnNlbGVjdE1hdGNoID0gZnVuY3Rpb24oYWN0aXZlSWR4KSB7XG4gICAgICAgICAgc2NvcGUuc2VsZWN0KHthY3RpdmVJZHg6YWN0aXZlSWR4fSk7XG4gICAgICAgIH07XG4gICAgICB9XG4gICAgfTtcbiAgfSlcblxuICAuZGlyZWN0aXZlKCd1aWJUeXBlYWhlYWRNYXRjaCcsIFsnJHRlbXBsYXRlUmVxdWVzdCcsICckY29tcGlsZScsICckcGFyc2UnLCBmdW5jdGlvbigkdGVtcGxhdGVSZXF1ZXN0LCAkY29tcGlsZSwgJHBhcnNlKSB7XG4gICAgcmV0dXJuIHtcbiAgICAgIHNjb3BlOiB7XG4gICAgICAgIGluZGV4OiAnPScsXG4gICAgICAgIG1hdGNoOiAnPScsXG4gICAgICAgIHF1ZXJ5OiAnPSdcbiAgICAgIH0sXG4gICAgICBsaW5rOmZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycykge1xuICAgICAgICB2YXIgdHBsVXJsID0gJHBhcnNlKGF0dHJzLnRlbXBsYXRlVXJsKShzY29wZS4kcGFyZW50KSB8fCAndGVtcGxhdGUvdHlwZWFoZWFkL3R5cGVhaGVhZC1tYXRjaC5odG1sJztcbiAgICAgICAgJHRlbXBsYXRlUmVxdWVzdCh0cGxVcmwpLnRoZW4oZnVuY3Rpb24odHBsQ29udGVudCkge1xuICAgICAgICAgICRjb21waWxlKHRwbENvbnRlbnQudHJpbSgpKShzY29wZSwgZnVuY3Rpb24oY2xvbmVkRWxlbWVudCkge1xuICAgICAgICAgICAgZWxlbWVudC5yZXBsYWNlV2l0aChjbG9uZWRFbGVtZW50KTtcbiAgICAgICAgICB9KTtcbiAgICAgICAgfSk7XG4gICAgICB9XG4gICAgfTtcbiAgfV0pXG5cbiAgLmZpbHRlcigndWliVHlwZWFoZWFkSGlnaGxpZ2h0JywgWyckc2NlJywgJyRpbmplY3RvcicsICckbG9nJywgZnVuY3Rpb24oJHNjZSwgJGluamVjdG9yLCAkbG9nKSB7XG4gICAgdmFyIGlzU2FuaXRpemVQcmVzZW50O1xuICAgIGlzU2FuaXRpemVQcmVzZW50ID0gJGluamVjdG9yLmhhcygnJHNhbml0aXplJyk7XG5cbiAgICBmdW5jdGlvbiBlc2NhcGVSZWdleHAocXVlcnlUb0VzY2FwZSkge1xuICAgICAgLy8gUmVnZXg6IGNhcHR1cmUgdGhlIHdob2xlIHF1ZXJ5IHN0cmluZyBhbmQgcmVwbGFjZSBpdCB3aXRoIHRoZSBzdHJpbmcgdGhhdCB3aWxsIGJlIHVzZWQgdG8gbWF0Y2hcbiAgICAgIC8vIHRoZSByZXN1bHRzLCBmb3IgZXhhbXBsZSBpZiB0aGUgY2FwdHVyZSBpcyBcImFcIiB0aGUgcmVzdWx0IHdpbGwgYmUgXFxhXG4gICAgICByZXR1cm4gcXVlcnlUb0VzY2FwZS5yZXBsYWNlKC8oWy4/KiteJFtcXF1cXFxcKCl7fXwtXSkvZywgJ1xcXFwkMScpO1xuICAgIH1cblxuICAgIGZ1bmN0aW9uIGNvbnRhaW5zSHRtbChtYXRjaEl0ZW0pIHtcbiAgICAgIHJldHVybiAvPC4qPi9nLnRlc3QobWF0Y2hJdGVtKTtcbiAgICB9XG5cbiAgICByZXR1cm4gZnVuY3Rpb24obWF0Y2hJdGVtLCBxdWVyeSkge1xuICAgICAgaWYgKCFpc1Nhbml0aXplUHJlc2VudCAmJiBjb250YWluc0h0bWwobWF0Y2hJdGVtKSkge1xuICAgICAgICAkbG9nLndhcm4oJ1Vuc2FmZSB1c2Ugb2YgdHlwZWFoZWFkIHBsZWFzZSB1c2UgbmdTYW5pdGl6ZScpOyAvLyBXYXJuIHRoZSB1c2VyIGFib3V0IHRoZSBkYW5nZXJcbiAgICAgIH1cbiAgICAgIG1hdGNoSXRlbSA9IHF1ZXJ5PyAoJycgKyBtYXRjaEl0ZW0pLnJlcGxhY2UobmV3IFJlZ0V4cChlc2NhcGVSZWdleHAocXVlcnkpLCAnZ2knKSwgJzxzdHJvbmc+JCY8L3N0cm9uZz4nKSA6IG1hdGNoSXRlbTsgLy8gUmVwbGFjZXMgdGhlIGNhcHR1cmUgc3RyaW5nIHdpdGggYSB0aGUgc2FtZSBzdHJpbmcgaW5zaWRlIG9mIGEgXCJzdHJvbmdcIiB0YWdcbiAgICAgIGlmICghaXNTYW5pdGl6ZVByZXNlbnQpIHtcbiAgICAgICAgbWF0Y2hJdGVtID0gJHNjZS50cnVzdEFzSHRtbChtYXRjaEl0ZW0pOyAvLyBJZiAkc2FuaXRpemUgaXMgbm90IHByZXNlbnQgd2UgcGFjayB0aGUgc3RyaW5nIGluIGEgJHNjZSBvYmplY3QgZm9yIHRoZSBuZy1iaW5kLWh0bWwgZGlyZWN0aXZlXG4gICAgICB9XG4gICAgICByZXR1cm4gbWF0Y2hJdGVtO1xuICAgIH07XG4gIH1dKTtcblxuLyogRGVwcmVjYXRlZCB0eXBlYWhlYWQgYmVsb3cgKi9cbiAgXG5hbmd1bGFyLm1vZHVsZSgndWkuYm9vdHN0cmFwLnR5cGVhaGVhZCcpXG4gIC52YWx1ZSgnJHR5cGVhaGVhZFN1cHByZXNzV2FybmluZycsIGZhbHNlKVxuICAuc2VydmljZSgndHlwZWFoZWFkUGFyc2VyJywgWyckcGFyc2UnLCAndWliVHlwZWFoZWFkUGFyc2VyJywgJyRsb2cnLCAnJHR5cGVhaGVhZFN1cHByZXNzV2FybmluZycsIGZ1bmN0aW9uKCRwYXJzZSwgdWliVHlwZWFoZWFkUGFyc2VyLCAkbG9nLCAkdHlwZWFoZWFkU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgaWYgKCEkdHlwZWFoZWFkU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAkbG9nLndhcm4oJ3R5cGVhaGVhZFBhcnNlciBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYlR5cGVhaGVhZFBhcnNlciBpbnN0ZWFkLicpO1xuICAgIH1cblxuICAgIHJldHVybiB1aWJUeXBlYWhlYWRQYXJzZXI7XG4gIH1dKVxuXG4gIC5kaXJlY3RpdmUoJ3R5cGVhaGVhZCcsIFsnJGNvbXBpbGUnLCAnJHBhcnNlJywgJyRxJywgJyR0aW1lb3V0JywgJyRkb2N1bWVudCcsICckd2luZG93JywgJyRyb290U2NvcGUnLCAnJHVpYlBvc2l0aW9uJywgJ3R5cGVhaGVhZFBhcnNlcicsICckbG9nJywgJyR0eXBlYWhlYWRTdXBwcmVzc1dhcm5pbmcnLFxuICAgIGZ1bmN0aW9uKCRjb21waWxlLCAkcGFyc2UsICRxLCAkdGltZW91dCwgJGRvY3VtZW50LCAkd2luZG93LCAkcm9vdFNjb3BlLCAkcG9zaXRpb24sIHR5cGVhaGVhZFBhcnNlciwgJGxvZywgJHR5cGVhaGVhZFN1cHByZXNzV2FybmluZykge1xuICAgIHZhciBIT1RfS0VZUyA9IFs5LCAxMywgMjcsIDM4LCA0MF07XG4gICAgdmFyIGV2ZW50RGVib3VuY2VUaW1lID0gMjAwO1xuICAgIHJldHVybiB7XG4gICAgICByZXF1aXJlOiBbJ25nTW9kZWwnLCAnXj9uZ01vZGVsT3B0aW9ucyddLFxuICAgICAgbGluazogZnVuY3Rpb24ob3JpZ2luYWxTY29wZSwgZWxlbWVudCwgYXR0cnMsIGN0cmxzKSB7XG4gICAgICAgIGlmICghJHR5cGVhaGVhZFN1cHByZXNzV2FybmluZykge1xuICAgICAgICAgICRsb2cud2FybigndHlwZWFoZWFkIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliLXR5cGVhaGVhZCBpbnN0ZWFkLicpO1xuICAgICAgICB9XG4gICAgICAgIHZhciBtb2RlbEN0cmwgPSBjdHJsc1swXTtcbiAgICAgICAgdmFyIG5nTW9kZWxPcHRpb25zID0gY3RybHNbMV07XG4gICAgICAgIC8vU1VQUE9SVEVEIEFUVFJJQlVURVMgKE9QVElPTlMpXG5cbiAgICAgICAgLy9taW5pbWFsIG5vIG9mIGNoYXJhY3RlcnMgdGhhdCBuZWVkcyB0byBiZSBlbnRlcmVkIGJlZm9yZSB0eXBlYWhlYWQga2lja3MtaW5cbiAgICAgICAgdmFyIG1pbkxlbmd0aCA9IG9yaWdpbmFsU2NvcGUuJGV2YWwoYXR0cnMudHlwZWFoZWFkTWluTGVuZ3RoKTtcbiAgICAgICAgaWYgKCFtaW5MZW5ndGggJiYgbWluTGVuZ3RoICE9PSAwKSB7XG4gICAgICAgICAgbWluTGVuZ3RoID0gMTtcbiAgICAgICAgfVxuXG4gICAgICAgIC8vbWluaW1hbCB3YWl0IHRpbWUgYWZ0ZXIgbGFzdCBjaGFyYWN0ZXIgdHlwZWQgYmVmb3JlIHR5cGVhaGVhZCBraWNrcy1pblxuICAgICAgICB2YXIgd2FpdFRpbWUgPSBvcmlnaW5hbFNjb3BlLiRldmFsKGF0dHJzLnR5cGVhaGVhZFdhaXRNcykgfHwgMDtcblxuICAgICAgICAvL3Nob3VsZCBpdCByZXN0cmljdCBtb2RlbCB2YWx1ZXMgdG8gdGhlIG9uZXMgc2VsZWN0ZWQgZnJvbSB0aGUgcG9wdXAgb25seT9cbiAgICAgICAgdmFyIGlzRWRpdGFibGUgPSBvcmlnaW5hbFNjb3BlLiRldmFsKGF0dHJzLnR5cGVhaGVhZEVkaXRhYmxlKSAhPT0gZmFsc2U7XG5cbiAgICAgICAgLy9iaW5kaW5nIHRvIGEgdmFyaWFibGUgdGhhdCBpbmRpY2F0ZXMgaWYgbWF0Y2hlcyBhcmUgYmVpbmcgcmV0cmlldmVkIGFzeW5jaHJvbm91c2x5XG4gICAgICAgIHZhciBpc0xvYWRpbmdTZXR0ZXIgPSAkcGFyc2UoYXR0cnMudHlwZWFoZWFkTG9hZGluZykuYXNzaWduIHx8IGFuZ3VsYXIubm9vcDtcblxuICAgICAgICAvL2EgY2FsbGJhY2sgZXhlY3V0ZWQgd2hlbiBhIG1hdGNoIGlzIHNlbGVjdGVkXG4gICAgICAgIHZhciBvblNlbGVjdENhbGxiYWNrID0gJHBhcnNlKGF0dHJzLnR5cGVhaGVhZE9uU2VsZWN0KTtcblxuICAgICAgICAvL3Nob3VsZCBpdCBzZWxlY3QgaGlnaGxpZ2h0ZWQgcG9wdXAgdmFsdWUgd2hlbiBsb3NpbmcgZm9jdXM/XG4gICAgICAgIHZhciBpc1NlbGVjdE9uQmx1ciA9IGFuZ3VsYXIuaXNEZWZpbmVkKGF0dHJzLnR5cGVhaGVhZFNlbGVjdE9uQmx1cikgPyBvcmlnaW5hbFNjb3BlLiRldmFsKGF0dHJzLnR5cGVhaGVhZFNlbGVjdE9uQmx1cikgOiBmYWxzZTtcblxuICAgICAgICAvL2JpbmRpbmcgdG8gYSB2YXJpYWJsZSB0aGF0IGluZGljYXRlcyBpZiB0aGVyZSB3ZXJlIG5vIHJlc3VsdHMgYWZ0ZXIgdGhlIHF1ZXJ5IGlzIGNvbXBsZXRlZFxuICAgICAgICB2YXIgaXNOb1Jlc3VsdHNTZXR0ZXIgPSAkcGFyc2UoYXR0cnMudHlwZWFoZWFkTm9SZXN1bHRzKS5hc3NpZ24gfHwgYW5ndWxhci5ub29wO1xuXG4gICAgICAgIHZhciBpbnB1dEZvcm1hdHRlciA9IGF0dHJzLnR5cGVhaGVhZElucHV0Rm9ybWF0dGVyID8gJHBhcnNlKGF0dHJzLnR5cGVhaGVhZElucHV0Rm9ybWF0dGVyKSA6IHVuZGVmaW5lZDtcblxuICAgICAgICB2YXIgYXBwZW5kVG9Cb2R5ID0gIGF0dHJzLnR5cGVhaGVhZEFwcGVuZFRvQm9keSA/IG9yaWdpbmFsU2NvcGUuJGV2YWwoYXR0cnMudHlwZWFoZWFkQXBwZW5kVG9Cb2R5KSA6IGZhbHNlO1xuXG4gICAgICAgIHZhciBhcHBlbmRUb0VsZW1lbnRJZCA9ICBhdHRycy50eXBlYWhlYWRBcHBlbmRUb0VsZW1lbnRJZCB8fCBmYWxzZTtcblxuICAgICAgICB2YXIgZm9jdXNGaXJzdCA9IG9yaWdpbmFsU2NvcGUuJGV2YWwoYXR0cnMudHlwZWFoZWFkRm9jdXNGaXJzdCkgIT09IGZhbHNlO1xuXG4gICAgICAgIC8vSWYgaW5wdXQgbWF0Y2hlcyBhbiBpdGVtIG9mIHRoZSBsaXN0IGV4YWN0bHksIHNlbGVjdCBpdCBhdXRvbWF0aWNhbGx5XG4gICAgICAgIHZhciBzZWxlY3RPbkV4YWN0ID0gYXR0cnMudHlwZWFoZWFkU2VsZWN0T25FeGFjdCA/IG9yaWdpbmFsU2NvcGUuJGV2YWwoYXR0cnMudHlwZWFoZWFkU2VsZWN0T25FeGFjdCkgOiBmYWxzZTtcblxuICAgICAgICAvL0lOVEVSTkFMIFZBUklBQkxFU1xuXG4gICAgICAgIC8vbW9kZWwgc2V0dGVyIGV4ZWN1dGVkIHVwb24gbWF0Y2ggc2VsZWN0aW9uXG4gICAgICAgIHZhciBwYXJzZWRNb2RlbCA9ICRwYXJzZShhdHRycy5uZ01vZGVsKTtcbiAgICAgICAgdmFyIGludm9rZU1vZGVsU2V0dGVyID0gJHBhcnNlKGF0dHJzLm5nTW9kZWwgKyAnKCQkJHApJyk7XG4gICAgICAgIHZhciAkc2V0TW9kZWxWYWx1ZSA9IGZ1bmN0aW9uKHNjb3BlLCBuZXdWYWx1ZSkge1xuICAgICAgICAgIGlmIChhbmd1bGFyLmlzRnVuY3Rpb24ocGFyc2VkTW9kZWwob3JpZ2luYWxTY29wZSkpICYmXG4gICAgICAgICAgICBuZ01vZGVsT3B0aW9ucyAmJiBuZ01vZGVsT3B0aW9ucy4kb3B0aW9ucyAmJiBuZ01vZGVsT3B0aW9ucy4kb3B0aW9ucy5nZXR0ZXJTZXR0ZXIpIHtcbiAgICAgICAgICAgIHJldHVybiBpbnZva2VNb2RlbFNldHRlcihzY29wZSwgeyQkJHA6IG5ld1ZhbHVlfSk7XG4gICAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICAgIHJldHVybiBwYXJzZWRNb2RlbC5hc3NpZ24oc2NvcGUsIG5ld1ZhbHVlKTtcbiAgICAgICAgICB9XG4gICAgICAgIH07XG5cbiAgICAgICAgLy9leHByZXNzaW9ucyB1c2VkIGJ5IHR5cGVhaGVhZFxuICAgICAgICB2YXIgcGFyc2VyUmVzdWx0ID0gdHlwZWFoZWFkUGFyc2VyLnBhcnNlKGF0dHJzLnR5cGVhaGVhZCk7XG5cbiAgICAgICAgdmFyIGhhc0ZvY3VzO1xuXG4gICAgICAgIC8vVXNlZCB0byBhdm9pZCBidWcgaW4gaU9TIHdlYnZpZXcgd2hlcmUgaU9TIGtleWJvYXJkIGRvZXMgbm90IGZpcmVcbiAgICAgICAgLy9tb3VzZWRvd24gJiBtb3VzZXVwIGV2ZW50c1xuICAgICAgICAvL0lzc3VlICMzNjk5XG4gICAgICAgIHZhciBzZWxlY3RlZDtcblxuICAgICAgICAvL2NyZWF0ZSBhIGNoaWxkIHNjb3BlIGZvciB0aGUgdHlwZWFoZWFkIGRpcmVjdGl2ZSBzbyB3ZSBhcmUgbm90IHBvbGx1dGluZyBvcmlnaW5hbCBzY29wZVxuICAgICAgICAvL3dpdGggdHlwZWFoZWFkLXNwZWNpZmljIGRhdGEgKG1hdGNoZXMsIHF1ZXJ5IGV0Yy4pXG4gICAgICAgIHZhciBzY29wZSA9IG9yaWdpbmFsU2NvcGUuJG5ldygpO1xuICAgICAgICB2YXIgb2ZmRGVzdHJveSA9IG9yaWdpbmFsU2NvcGUuJG9uKCckZGVzdHJveScsIGZ1bmN0aW9uKCkge1xuXHRcdFx0ICAgIHNjb3BlLiRkZXN0cm95KCk7XG4gICAgICAgIH0pO1xuICAgICAgICBzY29wZS4kb24oJyRkZXN0cm95Jywgb2ZmRGVzdHJveSk7XG5cbiAgICAgICAgLy8gV0FJLUFSSUFcbiAgICAgICAgdmFyIHBvcHVwSWQgPSAndHlwZWFoZWFkLScgKyBzY29wZS4kaWQgKyAnLScgKyBNYXRoLmZsb29yKE1hdGgucmFuZG9tKCkgKiAxMDAwMCk7XG4gICAgICAgIGVsZW1lbnQuYXR0cih7XG4gICAgICAgICAgJ2FyaWEtYXV0b2NvbXBsZXRlJzogJ2xpc3QnLFxuICAgICAgICAgICdhcmlhLWV4cGFuZGVkJzogZmFsc2UsXG4gICAgICAgICAgJ2FyaWEtb3ducyc6IHBvcHVwSWRcbiAgICAgICAgfSk7XG5cbiAgICAgICAgLy9wb3AtdXAgZWxlbWVudCB1c2VkIHRvIGRpc3BsYXkgbWF0Y2hlc1xuICAgICAgICB2YXIgcG9wVXBFbCA9IGFuZ3VsYXIuZWxlbWVudCgnPGRpdiB0eXBlYWhlYWQtcG9wdXA+PC9kaXY+Jyk7XG4gICAgICAgIHBvcFVwRWwuYXR0cih7XG4gICAgICAgICAgaWQ6IHBvcHVwSWQsXG4gICAgICAgICAgbWF0Y2hlczogJ21hdGNoZXMnLFxuICAgICAgICAgIGFjdGl2ZTogJ2FjdGl2ZUlkeCcsXG4gICAgICAgICAgc2VsZWN0OiAnc2VsZWN0KGFjdGl2ZUlkeCknLFxuICAgICAgICAgICdtb3ZlLWluLXByb2dyZXNzJzogJ21vdmVJblByb2dyZXNzJyxcbiAgICAgICAgICBxdWVyeTogJ3F1ZXJ5JyxcbiAgICAgICAgICBwb3NpdGlvbjogJ3Bvc2l0aW9uJ1xuICAgICAgICB9KTtcbiAgICAgICAgLy9jdXN0b20gaXRlbSB0ZW1wbGF0ZVxuICAgICAgICBpZiAoYW5ndWxhci5pc0RlZmluZWQoYXR0cnMudHlwZWFoZWFkVGVtcGxhdGVVcmwpKSB7XG4gICAgICAgICAgcG9wVXBFbC5hdHRyKCd0ZW1wbGF0ZS11cmwnLCBhdHRycy50eXBlYWhlYWRUZW1wbGF0ZVVybCk7XG4gICAgICAgIH1cblxuICAgICAgICBpZiAoYW5ndWxhci5pc0RlZmluZWQoYXR0cnMudHlwZWFoZWFkUG9wdXBUZW1wbGF0ZVVybCkpIHtcbiAgICAgICAgICBwb3BVcEVsLmF0dHIoJ3BvcHVwLXRlbXBsYXRlLXVybCcsIGF0dHJzLnR5cGVhaGVhZFBvcHVwVGVtcGxhdGVVcmwpO1xuICAgICAgICB9XG5cbiAgICAgICAgdmFyIHJlc2V0TWF0Y2hlcyA9IGZ1bmN0aW9uKCkge1xuICAgICAgICAgIHNjb3BlLm1hdGNoZXMgPSBbXTtcbiAgICAgICAgICBzY29wZS5hY3RpdmVJZHggPSAtMTtcbiAgICAgICAgICBlbGVtZW50LmF0dHIoJ2FyaWEtZXhwYW5kZWQnLCBmYWxzZSk7XG4gICAgICAgIH07XG5cbiAgICAgICAgdmFyIGdldE1hdGNoSWQgPSBmdW5jdGlvbihpbmRleCkge1xuICAgICAgICAgIHJldHVybiBwb3B1cElkICsgJy1vcHRpb24tJyArIGluZGV4O1xuICAgICAgICB9O1xuXG4gICAgICAgIC8vIEluZGljYXRlIHRoYXQgdGhlIHNwZWNpZmllZCBtYXRjaCBpcyB0aGUgYWN0aXZlIChwcmUtc2VsZWN0ZWQpIGl0ZW0gaW4gdGhlIGxpc3Qgb3duZWQgYnkgdGhpcyB0eXBlYWhlYWQuXG4gICAgICAgIC8vIFRoaXMgYXR0cmlidXRlIGlzIGFkZGVkIG9yIHJlbW92ZWQgYXV0b21hdGljYWxseSB3aGVuIHRoZSBgYWN0aXZlSWR4YCBjaGFuZ2VzLlxuICAgICAgICBzY29wZS4kd2F0Y2goJ2FjdGl2ZUlkeCcsIGZ1bmN0aW9uKGluZGV4KSB7XG4gICAgICAgICAgaWYgKGluZGV4IDwgMCkge1xuICAgICAgICAgICAgZWxlbWVudC5yZW1vdmVBdHRyKCdhcmlhLWFjdGl2ZWRlc2NlbmRhbnQnKTtcbiAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgZWxlbWVudC5hdHRyKCdhcmlhLWFjdGl2ZWRlc2NlbmRhbnQnLCBnZXRNYXRjaElkKGluZGV4KSk7XG4gICAgICAgICAgfVxuICAgICAgICB9KTtcblxuICAgICAgICB2YXIgaW5wdXRJc0V4YWN0TWF0Y2ggPSBmdW5jdGlvbihpbnB1dFZhbHVlLCBpbmRleCkge1xuICAgICAgICAgIGlmIChzY29wZS5tYXRjaGVzLmxlbmd0aCA+IGluZGV4ICYmIGlucHV0VmFsdWUpIHtcbiAgICAgICAgICAgIHJldHVybiBpbnB1dFZhbHVlLnRvVXBwZXJDYXNlKCkgPT09IHNjb3BlLm1hdGNoZXNbaW5kZXhdLmxhYmVsLnRvVXBwZXJDYXNlKCk7XG4gICAgICAgICAgfVxuXG4gICAgICAgICAgcmV0dXJuIGZhbHNlO1xuICAgICAgICB9O1xuXG4gICAgICAgIHZhciBnZXRNYXRjaGVzQXN5bmMgPSBmdW5jdGlvbihpbnB1dFZhbHVlKSB7XG4gICAgICAgICAgdmFyIGxvY2FscyA9IHskdmlld1ZhbHVlOiBpbnB1dFZhbHVlfTtcbiAgICAgICAgICBpc0xvYWRpbmdTZXR0ZXIob3JpZ2luYWxTY29wZSwgdHJ1ZSk7XG4gICAgICAgICAgaXNOb1Jlc3VsdHNTZXR0ZXIob3JpZ2luYWxTY29wZSwgZmFsc2UpO1xuICAgICAgICAgICRxLndoZW4ocGFyc2VyUmVzdWx0LnNvdXJjZShvcmlnaW5hbFNjb3BlLCBsb2NhbHMpKS50aGVuKGZ1bmN0aW9uKG1hdGNoZXMpIHtcbiAgICAgICAgICAgIC8vaXQgbWlnaHQgaGFwcGVuIHRoYXQgc2V2ZXJhbCBhc3luYyBxdWVyaWVzIHdlcmUgaW4gcHJvZ3Jlc3MgaWYgYSB1c2VyIHdlcmUgdHlwaW5nIGZhc3RcbiAgICAgICAgICAgIC8vYnV0IHdlIGFyZSBpbnRlcmVzdGVkIG9ubHkgaW4gcmVzcG9uc2VzIHRoYXQgY29ycmVzcG9uZCB0byB0aGUgY3VycmVudCB2aWV3IHZhbHVlXG4gICAgICAgICAgICB2YXIgb25DdXJyZW50UmVxdWVzdCA9IChpbnB1dFZhbHVlID09PSBtb2RlbEN0cmwuJHZpZXdWYWx1ZSk7XG4gICAgICAgICAgICBpZiAob25DdXJyZW50UmVxdWVzdCAmJiBoYXNGb2N1cykge1xuICAgICAgICAgICAgICBpZiAobWF0Y2hlcyAmJiBtYXRjaGVzLmxlbmd0aCA+IDApIHtcbiAgICAgICAgICAgICAgICBzY29wZS5hY3RpdmVJZHggPSBmb2N1c0ZpcnN0ID8gMCA6IC0xO1xuICAgICAgICAgICAgICAgIGlzTm9SZXN1bHRzU2V0dGVyKG9yaWdpbmFsU2NvcGUsIGZhbHNlKTtcbiAgICAgICAgICAgICAgICBzY29wZS5tYXRjaGVzLmxlbmd0aCA9IDA7XG5cbiAgICAgICAgICAgICAgICAvL3RyYW5zZm9ybSBsYWJlbHNcbiAgICAgICAgICAgICAgICBmb3IgKHZhciBpID0gMDsgaSA8IG1hdGNoZXMubGVuZ3RoOyBpKyspIHtcbiAgICAgICAgICAgICAgICAgIGxvY2Fsc1twYXJzZXJSZXN1bHQuaXRlbU5hbWVdID0gbWF0Y2hlc1tpXTtcbiAgICAgICAgICAgICAgICAgIHNjb3BlLm1hdGNoZXMucHVzaCh7XG4gICAgICAgICAgICAgICAgICAgIGlkOiBnZXRNYXRjaElkKGkpLFxuICAgICAgICAgICAgICAgICAgICBsYWJlbDogcGFyc2VyUmVzdWx0LnZpZXdNYXBwZXIoc2NvcGUsIGxvY2FscyksXG4gICAgICAgICAgICAgICAgICAgIG1vZGVsOiBtYXRjaGVzW2ldXG4gICAgICAgICAgICAgICAgICB9KTtcbiAgICAgICAgICAgICAgICB9XG5cbiAgICAgICAgICAgICAgICBzY29wZS5xdWVyeSA9IGlucHV0VmFsdWU7XG4gICAgICAgICAgICAgICAgLy9wb3NpdGlvbiBwb3AtdXAgd2l0aCBtYXRjaGVzIC0gd2UgbmVlZCB0byByZS1jYWxjdWxhdGUgaXRzIHBvc2l0aW9uIGVhY2ggdGltZSB3ZSBhcmUgb3BlbmluZyBhIHdpbmRvd1xuICAgICAgICAgICAgICAgIC8vd2l0aCBtYXRjaGVzIGFzIGEgcG9wLXVwIG1pZ2h0IGJlIGFic29sdXRlLXBvc2l0aW9uZWQgYW5kIHBvc2l0aW9uIG9mIGFuIGlucHV0IG1pZ2h0IGhhdmUgY2hhbmdlZCBvbiBhIHBhZ2VcbiAgICAgICAgICAgICAgICAvL2R1ZSB0byBvdGhlciBlbGVtZW50cyBiZWluZyByZW5kZXJlZFxuICAgICAgICAgICAgICAgIHJlY2FsY3VsYXRlUG9zaXRpb24oKTtcblxuICAgICAgICAgICAgICAgIGVsZW1lbnQuYXR0cignYXJpYS1leHBhbmRlZCcsIHRydWUpO1xuXG4gICAgICAgICAgICAgICAgLy9TZWxlY3QgdGhlIHNpbmdsZSByZW1haW5pbmcgb3B0aW9uIGlmIHVzZXIgaW5wdXQgbWF0Y2hlc1xuICAgICAgICAgICAgICAgIGlmIChzZWxlY3RPbkV4YWN0ICYmIHNjb3BlLm1hdGNoZXMubGVuZ3RoID09PSAxICYmIGlucHV0SXNFeGFjdE1hdGNoKGlucHV0VmFsdWUsIDApKSB7XG4gICAgICAgICAgICAgICAgICBzY29wZS5zZWxlY3QoMCk7XG4gICAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgICAgIHJlc2V0TWF0Y2hlcygpO1xuICAgICAgICAgICAgICAgIGlzTm9SZXN1bHRzU2V0dGVyKG9yaWdpbmFsU2NvcGUsIHRydWUpO1xuICAgICAgICAgICAgICB9XG4gICAgICAgICAgICB9XG4gICAgICAgICAgICBpZiAob25DdXJyZW50UmVxdWVzdCkge1xuICAgICAgICAgICAgICBpc0xvYWRpbmdTZXR0ZXIob3JpZ2luYWxTY29wZSwgZmFsc2UpO1xuICAgICAgICAgICAgfVxuICAgICAgICAgIH0sIGZ1bmN0aW9uKCkge1xuICAgICAgICAgICAgcmVzZXRNYXRjaGVzKCk7XG4gICAgICAgICAgICBpc0xvYWRpbmdTZXR0ZXIob3JpZ2luYWxTY29wZSwgZmFsc2UpO1xuICAgICAgICAgICAgaXNOb1Jlc3VsdHNTZXR0ZXIob3JpZ2luYWxTY29wZSwgdHJ1ZSk7XG4gICAgICAgICAgfSk7XG4gICAgICAgIH07XG5cbiAgICAgICAgLy8gYmluZCBldmVudHMgb25seSBpZiBhcHBlbmRUb0JvZHkgcGFyYW1zIGV4aXN0IC0gcGVyZm9ybWFuY2UgZmVhdHVyZVxuICAgICAgICBpZiAoYXBwZW5kVG9Cb2R5KSB7XG4gICAgICAgICAgYW5ndWxhci5lbGVtZW50KCR3aW5kb3cpLmJpbmQoJ3Jlc2l6ZScsIGZpcmVSZWNhbGN1bGF0aW5nKTtcbiAgICAgICAgICAkZG9jdW1lbnQuZmluZCgnYm9keScpLmJpbmQoJ3Njcm9sbCcsIGZpcmVSZWNhbGN1bGF0aW5nKTtcbiAgICAgICAgfVxuXG4gICAgICAgIC8vIERlY2xhcmUgdGhlIHRpbWVvdXQgcHJvbWlzZSB2YXIgb3V0c2lkZSB0aGUgZnVuY3Rpb24gc2NvcGUgc28gdGhhdCBzdGFja2VkIGNhbGxzIGNhbiBiZSBjYW5jZWxsZWQgbGF0ZXJcbiAgICAgICAgdmFyIHRpbWVvdXRFdmVudFByb21pc2U7XG5cbiAgICAgICAgLy8gRGVmYXVsdCBwcm9ncmVzcyB0eXBlXG4gICAgICAgIHNjb3BlLm1vdmVJblByb2dyZXNzID0gZmFsc2U7XG5cbiAgICAgICAgZnVuY3Rpb24gZmlyZVJlY2FsY3VsYXRpbmcoKSB7XG4gICAgICAgICAgaWYgKCFzY29wZS5tb3ZlSW5Qcm9ncmVzcykge1xuICAgICAgICAgICAgc2NvcGUubW92ZUluUHJvZ3Jlc3MgPSB0cnVlO1xuICAgICAgICAgICAgc2NvcGUuJGRpZ2VzdCgpO1xuICAgICAgICAgIH1cblxuICAgICAgICAgIC8vIENhbmNlbCBwcmV2aW91cyB0aW1lb3V0XG4gICAgICAgICAgaWYgKHRpbWVvdXRFdmVudFByb21pc2UpIHtcbiAgICAgICAgICAgICR0aW1lb3V0LmNhbmNlbCh0aW1lb3V0RXZlbnRQcm9taXNlKTtcbiAgICAgICAgICB9XG5cbiAgICAgICAgICAvLyBEZWJvdW5jZWQgZXhlY3V0aW5nIHJlY2FsY3VsYXRlIGFmdGVyIGV2ZW50cyBmaXJlZFxuICAgICAgICAgIHRpbWVvdXRFdmVudFByb21pc2UgPSAkdGltZW91dChmdW5jdGlvbigpIHtcbiAgICAgICAgICAgIC8vIGlmIHBvcHVwIGlzIHZpc2libGVcbiAgICAgICAgICAgIGlmIChzY29wZS5tYXRjaGVzLmxlbmd0aCkge1xuICAgICAgICAgICAgICByZWNhbGN1bGF0ZVBvc2l0aW9uKCk7XG4gICAgICAgICAgICB9XG5cbiAgICAgICAgICAgIHNjb3BlLm1vdmVJblByb2dyZXNzID0gZmFsc2U7XG4gICAgICAgICAgfSwgZXZlbnREZWJvdW5jZVRpbWUpO1xuICAgICAgICB9XG5cbiAgICAgICAgLy8gcmVjYWxjdWxhdGUgYWN0dWFsIHBvc2l0aW9uIGFuZCBzZXQgbmV3IHZhbHVlcyB0byBzY29wZVxuICAgICAgICAvLyBhZnRlciBkaWdlc3QgbG9vcCBpcyBwb3B1cCBpbiByaWdodCBwb3NpdGlvblxuICAgICAgICBmdW5jdGlvbiByZWNhbGN1bGF0ZVBvc2l0aW9uKCkge1xuICAgICAgICAgIHNjb3BlLnBvc2l0aW9uID0gYXBwZW5kVG9Cb2R5ID8gJHBvc2l0aW9uLm9mZnNldChlbGVtZW50KSA6ICRwb3NpdGlvbi5wb3NpdGlvbihlbGVtZW50KTtcbiAgICAgICAgICBzY29wZS5wb3NpdGlvbi50b3AgKz0gZWxlbWVudC5wcm9wKCdvZmZzZXRIZWlnaHQnKTtcbiAgICAgICAgfVxuXG4gICAgICAgIHJlc2V0TWF0Y2hlcygpO1xuXG4gICAgICAgIC8vd2UgbmVlZCB0byBwcm9wYWdhdGUgdXNlcidzIHF1ZXJ5IHNvIHdlIGNhbiBoaWdsaWdodCBtYXRjaGVzXG4gICAgICAgIHNjb3BlLnF1ZXJ5ID0gdW5kZWZpbmVkO1xuXG4gICAgICAgIC8vRGVjbGFyZSB0aGUgdGltZW91dCBwcm9taXNlIHZhciBvdXRzaWRlIHRoZSBmdW5jdGlvbiBzY29wZSBzbyB0aGF0IHN0YWNrZWQgY2FsbHMgY2FuIGJlIGNhbmNlbGxlZCBsYXRlclxuICAgICAgICB2YXIgdGltZW91dFByb21pc2U7XG5cbiAgICAgICAgdmFyIHNjaGVkdWxlU2VhcmNoV2l0aFRpbWVvdXQgPSBmdW5jdGlvbihpbnB1dFZhbHVlKSB7XG4gICAgICAgICAgdGltZW91dFByb21pc2UgPSAkdGltZW91dChmdW5jdGlvbigpIHtcbiAgICAgICAgICAgIGdldE1hdGNoZXNBc3luYyhpbnB1dFZhbHVlKTtcbiAgICAgICAgICB9LCB3YWl0VGltZSk7XG4gICAgICAgIH07XG5cbiAgICAgICAgdmFyIGNhbmNlbFByZXZpb3VzVGltZW91dCA9IGZ1bmN0aW9uKCkge1xuICAgICAgICAgIGlmICh0aW1lb3V0UHJvbWlzZSkge1xuICAgICAgICAgICAgJHRpbWVvdXQuY2FuY2VsKHRpbWVvdXRQcm9taXNlKTtcbiAgICAgICAgICB9XG4gICAgICAgIH07XG5cbiAgICAgICAgLy9wbHVnIGludG8gJHBhcnNlcnMgcGlwZWxpbmUgdG8gb3BlbiBhIHR5cGVhaGVhZCBvbiB2aWV3IGNoYW5nZXMgaW5pdGlhdGVkIGZyb20gRE9NXG4gICAgICAgIC8vJHBhcnNlcnMga2ljay1pbiBvbiBhbGwgdGhlIGNoYW5nZXMgY29taW5nIGZyb20gdGhlIHZpZXcgYXMgd2VsbCBhcyBtYW51YWxseSB0cmlnZ2VyZWQgYnkgJHNldFZpZXdWYWx1ZVxuICAgICAgICBtb2RlbEN0cmwuJHBhcnNlcnMudW5zaGlmdChmdW5jdGlvbihpbnB1dFZhbHVlKSB7XG4gICAgICAgICAgaGFzRm9jdXMgPSB0cnVlO1xuXG4gICAgICAgICAgaWYgKG1pbkxlbmd0aCA9PT0gMCB8fCBpbnB1dFZhbHVlICYmIGlucHV0VmFsdWUubGVuZ3RoID49IG1pbkxlbmd0aCkge1xuICAgICAgICAgICAgaWYgKHdhaXRUaW1lID4gMCkge1xuICAgICAgICAgICAgICBjYW5jZWxQcmV2aW91c1RpbWVvdXQoKTtcbiAgICAgICAgICAgICAgc2NoZWR1bGVTZWFyY2hXaXRoVGltZW91dChpbnB1dFZhbHVlKTtcbiAgICAgICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgICAgIGdldE1hdGNoZXNBc3luYyhpbnB1dFZhbHVlKTtcbiAgICAgICAgICAgIH1cbiAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgaXNMb2FkaW5nU2V0dGVyKG9yaWdpbmFsU2NvcGUsIGZhbHNlKTtcbiAgICAgICAgICAgIGNhbmNlbFByZXZpb3VzVGltZW91dCgpO1xuICAgICAgICAgICAgcmVzZXRNYXRjaGVzKCk7XG4gICAgICAgICAgfVxuXG4gICAgICAgICAgaWYgKGlzRWRpdGFibGUpIHtcbiAgICAgICAgICAgIHJldHVybiBpbnB1dFZhbHVlO1xuICAgICAgICAgIH0gZWxzZSB7XG4gICAgICAgICAgICBpZiAoIWlucHV0VmFsdWUpIHtcbiAgICAgICAgICAgICAgLy8gUmVzZXQgaW4gY2FzZSB1c2VyIGhhZCB0eXBlZCBzb21ldGhpbmcgcHJldmlvdXNseS5cbiAgICAgICAgICAgICAgbW9kZWxDdHJsLiRzZXRWYWxpZGl0eSgnZWRpdGFibGUnLCB0cnVlKTtcbiAgICAgICAgICAgICAgcmV0dXJuIG51bGw7XG4gICAgICAgICAgICB9IGVsc2Uge1xuICAgICAgICAgICAgICBtb2RlbEN0cmwuJHNldFZhbGlkaXR5KCdlZGl0YWJsZScsIGZhbHNlKTtcbiAgICAgICAgICAgICAgcmV0dXJuIHVuZGVmaW5lZDtcbiAgICAgICAgICAgIH1cbiAgICAgICAgICB9XG4gICAgICAgIH0pO1xuXG4gICAgICAgIG1vZGVsQ3RybC4kZm9ybWF0dGVycy5wdXNoKGZ1bmN0aW9uKG1vZGVsVmFsdWUpIHtcbiAgICAgICAgICB2YXIgY2FuZGlkYXRlVmlld1ZhbHVlLCBlbXB0eVZpZXdWYWx1ZTtcbiAgICAgICAgICB2YXIgbG9jYWxzID0ge307XG5cbiAgICAgICAgICAvLyBUaGUgdmFsaWRpdHkgbWF5IGJlIHNldCB0byBmYWxzZSB2aWEgJHBhcnNlcnMgKHNlZSBhYm92ZSkgaWZcbiAgICAgICAgICAvLyB0aGUgbW9kZWwgaXMgcmVzdHJpY3RlZCB0byBzZWxlY3RlZCB2YWx1ZXMuIElmIHRoZSBtb2RlbFxuICAgICAgICAgIC8vIGlzIHNldCBtYW51YWxseSBpdCBpcyBjb25zaWRlcmVkIHRvIGJlIHZhbGlkLlxuICAgICAgICAgIGlmICghaXNFZGl0YWJsZSkge1xuICAgICAgICAgICAgbW9kZWxDdHJsLiRzZXRWYWxpZGl0eSgnZWRpdGFibGUnLCB0cnVlKTtcbiAgICAgICAgICB9XG5cbiAgICAgICAgICBpZiAoaW5wdXRGb3JtYXR0ZXIpIHtcbiAgICAgICAgICAgIGxvY2Fscy4kbW9kZWwgPSBtb2RlbFZhbHVlO1xuICAgICAgICAgICAgcmV0dXJuIGlucHV0Rm9ybWF0dGVyKG9yaWdpbmFsU2NvcGUsIGxvY2Fscyk7XG4gICAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICAgIC8vaXQgbWlnaHQgaGFwcGVuIHRoYXQgd2UgZG9uJ3QgaGF2ZSBlbm91Z2ggaW5mbyB0byBwcm9wZXJseSByZW5kZXIgaW5wdXQgdmFsdWVcbiAgICAgICAgICAgIC8vd2UgbmVlZCB0byBjaGVjayBmb3IgdGhpcyBzaXR1YXRpb24gYW5kIHNpbXBseSByZXR1cm4gbW9kZWwgdmFsdWUgaWYgd2UgY2FuJ3QgYXBwbHkgY3VzdG9tIGZvcm1hdHRpbmdcbiAgICAgICAgICAgIGxvY2Fsc1twYXJzZXJSZXN1bHQuaXRlbU5hbWVdID0gbW9kZWxWYWx1ZTtcbiAgICAgICAgICAgIGNhbmRpZGF0ZVZpZXdWYWx1ZSA9IHBhcnNlclJlc3VsdC52aWV3TWFwcGVyKG9yaWdpbmFsU2NvcGUsIGxvY2Fscyk7XG4gICAgICAgICAgICBsb2NhbHNbcGFyc2VyUmVzdWx0Lml0ZW1OYW1lXSA9IHVuZGVmaW5lZDtcbiAgICAgICAgICAgIGVtcHR5Vmlld1ZhbHVlID0gcGFyc2VyUmVzdWx0LnZpZXdNYXBwZXIob3JpZ2luYWxTY29wZSwgbG9jYWxzKTtcblxuICAgICAgICAgICAgcmV0dXJuIGNhbmRpZGF0ZVZpZXdWYWx1ZSAhPT0gZW1wdHlWaWV3VmFsdWUgPyBjYW5kaWRhdGVWaWV3VmFsdWUgOiBtb2RlbFZhbHVlO1xuICAgICAgICAgIH1cbiAgICAgICAgfSk7XG5cbiAgICAgICAgc2NvcGUuc2VsZWN0ID0gZnVuY3Rpb24oYWN0aXZlSWR4KSB7XG4gICAgICAgICAgLy9jYWxsZWQgZnJvbSB3aXRoaW4gdGhlICRkaWdlc3QoKSBjeWNsZVxuICAgICAgICAgIHZhciBsb2NhbHMgPSB7fTtcbiAgICAgICAgICB2YXIgbW9kZWwsIGl0ZW07XG5cbiAgICAgICAgICBzZWxlY3RlZCA9IHRydWU7XG4gICAgICAgICAgbG9jYWxzW3BhcnNlclJlc3VsdC5pdGVtTmFtZV0gPSBpdGVtID0gc2NvcGUubWF0Y2hlc1thY3RpdmVJZHhdLm1vZGVsO1xuICAgICAgICAgIG1vZGVsID0gcGFyc2VyUmVzdWx0Lm1vZGVsTWFwcGVyKG9yaWdpbmFsU2NvcGUsIGxvY2Fscyk7XG4gICAgICAgICAgJHNldE1vZGVsVmFsdWUob3JpZ2luYWxTY29wZSwgbW9kZWwpO1xuICAgICAgICAgIG1vZGVsQ3RybC4kc2V0VmFsaWRpdHkoJ2VkaXRhYmxlJywgdHJ1ZSk7XG4gICAgICAgICAgbW9kZWxDdHJsLiRzZXRWYWxpZGl0eSgncGFyc2UnLCB0cnVlKTtcblxuICAgICAgICAgIG9uU2VsZWN0Q2FsbGJhY2sob3JpZ2luYWxTY29wZSwge1xuICAgICAgICAgICAgJGl0ZW06IGl0ZW0sXG4gICAgICAgICAgICAkbW9kZWw6IG1vZGVsLFxuICAgICAgICAgICAgJGxhYmVsOiBwYXJzZXJSZXN1bHQudmlld01hcHBlcihvcmlnaW5hbFNjb3BlLCBsb2NhbHMpXG4gICAgICAgICAgfSk7XG5cbiAgICAgICAgICByZXNldE1hdGNoZXMoKTtcblxuICAgICAgICAgIC8vcmV0dXJuIGZvY3VzIHRvIHRoZSBpbnB1dCBlbGVtZW50IGlmIGEgbWF0Y2ggd2FzIHNlbGVjdGVkIHZpYSBhIG1vdXNlIGNsaWNrIGV2ZW50XG4gICAgICAgICAgLy8gdXNlIHRpbWVvdXQgdG8gYXZvaWQgJHJvb3RTY29wZTppbnByb2cgZXJyb3JcbiAgICAgICAgICBpZiAoc2NvcGUuJGV2YWwoYXR0cnMudHlwZWFoZWFkRm9jdXNPblNlbGVjdCkgIT09IGZhbHNlKSB7XG4gICAgICAgICAgICAkdGltZW91dChmdW5jdGlvbigpIHsgZWxlbWVudFswXS5mb2N1cygpOyB9LCAwLCBmYWxzZSk7XG4gICAgICAgICAgfVxuICAgICAgICB9O1xuXG4gICAgICAgIC8vYmluZCBrZXlib2FyZCBldmVudHM6IGFycm93cyB1cCgzOCkgLyBkb3duKDQwKSwgZW50ZXIoMTMpIGFuZCB0YWIoOSksIGVzYygyNylcbiAgICAgICAgZWxlbWVudC5iaW5kKCdrZXlkb3duJywgZnVuY3Rpb24oZXZ0KSB7XG4gICAgICAgICAgLy90eXBlYWhlYWQgaXMgb3BlbiBhbmQgYW4gXCJpbnRlcmVzdGluZ1wiIGtleSB3YXMgcHJlc3NlZFxuICAgICAgICAgIGlmIChzY29wZS5tYXRjaGVzLmxlbmd0aCA9PT0gMCB8fCBIT1RfS0VZUy5pbmRleE9mKGV2dC53aGljaCkgPT09IC0xKSB7XG4gICAgICAgICAgICByZXR1cm47XG4gICAgICAgICAgfVxuXG4gICAgICAgICAgLy8gaWYgdGhlcmUncyBub3RoaW5nIHNlbGVjdGVkIChpLmUuIGZvY3VzRmlyc3QpIGFuZCBlbnRlciBvciB0YWIgaXMgaGl0LCBjbGVhciB0aGUgcmVzdWx0c1xuICAgICAgICAgIGlmIChzY29wZS5hY3RpdmVJZHggPT09IC0xICYmIChldnQud2hpY2ggPT09IDkgfHwgZXZ0LndoaWNoID09PSAxMykpIHtcbiAgICAgICAgICAgIHJlc2V0TWF0Y2hlcygpO1xuICAgICAgICAgICAgc2NvcGUuJGRpZ2VzdCgpO1xuICAgICAgICAgICAgcmV0dXJuO1xuICAgICAgICAgIH1cblxuICAgICAgICAgIGV2dC5wcmV2ZW50RGVmYXVsdCgpO1xuXG4gICAgICAgICAgaWYgKGV2dC53aGljaCA9PT0gNDApIHtcbiAgICAgICAgICAgIHNjb3BlLmFjdGl2ZUlkeCA9IChzY29wZS5hY3RpdmVJZHggKyAxKSAlIHNjb3BlLm1hdGNoZXMubGVuZ3RoO1xuICAgICAgICAgICAgc2NvcGUuJGRpZ2VzdCgpO1xuICAgICAgICAgIH0gZWxzZSBpZiAoZXZ0LndoaWNoID09PSAzOCkge1xuICAgICAgICAgICAgc2NvcGUuYWN0aXZlSWR4ID0gKHNjb3BlLmFjdGl2ZUlkeCA+IDAgPyBzY29wZS5hY3RpdmVJZHggOiBzY29wZS5tYXRjaGVzLmxlbmd0aCkgLSAxO1xuICAgICAgICAgICAgc2NvcGUuJGRpZ2VzdCgpO1xuICAgICAgICAgIH0gZWxzZSBpZiAoZXZ0LndoaWNoID09PSAxMyB8fCBldnQud2hpY2ggPT09IDkpIHtcbiAgICAgICAgICAgIHNjb3BlLiRhcHBseShmdW5jdGlvbiAoKSB7XG4gICAgICAgICAgICAgIHNjb3BlLnNlbGVjdChzY29wZS5hY3RpdmVJZHgpO1xuICAgICAgICAgICAgfSk7XG4gICAgICAgICAgfSBlbHNlIGlmIChldnQud2hpY2ggPT09IDI3KSB7XG4gICAgICAgICAgICBldnQuc3RvcFByb3BhZ2F0aW9uKCk7XG5cbiAgICAgICAgICAgIHJlc2V0TWF0Y2hlcygpO1xuICAgICAgICAgICAgc2NvcGUuJGRpZ2VzdCgpO1xuICAgICAgICAgIH1cbiAgICAgICAgfSk7XG5cbiAgICAgICAgZWxlbWVudC5iaW5kKCdibHVyJywgZnVuY3Rpb24oKSB7XG4gICAgICAgICAgaWYgKGlzU2VsZWN0T25CbHVyICYmIHNjb3BlLm1hdGNoZXMubGVuZ3RoICYmIHNjb3BlLmFjdGl2ZUlkeCAhPT0gLTEgJiYgIXNlbGVjdGVkKSB7XG4gICAgICAgICAgICBzZWxlY3RlZCA9IHRydWU7XG4gICAgICAgICAgICBzY29wZS4kYXBwbHkoZnVuY3Rpb24oKSB7XG4gICAgICAgICAgICAgIHNjb3BlLnNlbGVjdChzY29wZS5hY3RpdmVJZHgpO1xuICAgICAgICAgICAgfSk7XG4gICAgICAgICAgfVxuICAgICAgICAgIGhhc0ZvY3VzID0gZmFsc2U7XG4gICAgICAgICAgc2VsZWN0ZWQgPSBmYWxzZTtcbiAgICAgICAgfSk7XG5cbiAgICAgICAgLy8gS2VlcCByZWZlcmVuY2UgdG8gY2xpY2sgaGFuZGxlciB0byB1bmJpbmQgaXQuXG4gICAgICAgIHZhciBkaXNtaXNzQ2xpY2tIYW5kbGVyID0gZnVuY3Rpb24oZXZ0KSB7XG4gICAgICAgICAgLy8gSXNzdWUgIzM5NzNcbiAgICAgICAgICAvLyBGaXJlZm94IHRyZWF0cyByaWdodCBjbGljayBhcyBhIGNsaWNrIG9uIGRvY3VtZW50XG4gICAgICAgICAgaWYgKGVsZW1lbnRbMF0gIT09IGV2dC50YXJnZXQgJiYgZXZ0LndoaWNoICE9PSAzICYmIHNjb3BlLm1hdGNoZXMubGVuZ3RoICE9PSAwKSB7XG4gICAgICAgICAgICByZXNldE1hdGNoZXMoKTtcbiAgICAgICAgICAgIGlmICghJHJvb3RTY29wZS4kJHBoYXNlKSB7XG4gICAgICAgICAgICAgIHNjb3BlLiRkaWdlc3QoKTtcbiAgICAgICAgICAgIH1cbiAgICAgICAgICB9XG4gICAgICAgIH07XG5cbiAgICAgICAgJGRvY3VtZW50LmJpbmQoJ2NsaWNrJywgZGlzbWlzc0NsaWNrSGFuZGxlcik7XG5cbiAgICAgICAgb3JpZ2luYWxTY29wZS4kb24oJyRkZXN0cm95JywgZnVuY3Rpb24oKSB7XG4gICAgICAgICAgJGRvY3VtZW50LnVuYmluZCgnY2xpY2snLCBkaXNtaXNzQ2xpY2tIYW5kbGVyKTtcbiAgICAgICAgICBpZiAoYXBwZW5kVG9Cb2R5IHx8IGFwcGVuZFRvRWxlbWVudElkKSB7XG4gICAgICAgICAgICAkcG9wdXAucmVtb3ZlKCk7XG4gICAgICAgICAgfVxuXG4gICAgICAgICAgaWYgKGFwcGVuZFRvQm9keSkge1xuICAgICAgICAgICAgYW5ndWxhci5lbGVtZW50KCR3aW5kb3cpLnVuYmluZCgncmVzaXplJywgZmlyZVJlY2FsY3VsYXRpbmcpO1xuICAgICAgICAgICAgJGRvY3VtZW50LmZpbmQoJ2JvZHknKS51bmJpbmQoJ3Njcm9sbCcsIGZpcmVSZWNhbGN1bGF0aW5nKTtcbiAgICAgICAgICB9XG4gICAgICAgICAgLy8gUHJldmVudCBqUXVlcnkgY2FjaGUgbWVtb3J5IGxlYWtcbiAgICAgICAgICBwb3BVcEVsLnJlbW92ZSgpO1xuICAgICAgICB9KTtcblxuICAgICAgICB2YXIgJHBvcHVwID0gJGNvbXBpbGUocG9wVXBFbCkoc2NvcGUpO1xuXG4gICAgICAgIGlmIChhcHBlbmRUb0JvZHkpIHtcbiAgICAgICAgICAkZG9jdW1lbnQuZmluZCgnYm9keScpLmFwcGVuZCgkcG9wdXApO1xuICAgICAgICB9IGVsc2UgaWYgKGFwcGVuZFRvRWxlbWVudElkICE9PSBmYWxzZSkge1xuICAgICAgICAgIGFuZ3VsYXIuZWxlbWVudCgkZG9jdW1lbnRbMF0uZ2V0RWxlbWVudEJ5SWQoYXBwZW5kVG9FbGVtZW50SWQpKS5hcHBlbmQoJHBvcHVwKTtcbiAgICAgICAgfSBlbHNlIHtcbiAgICAgICAgICBlbGVtZW50LmFmdGVyKCRwb3B1cCk7XG4gICAgICAgIH1cbiAgICAgIH1cbiAgICB9O1xuICB9XSlcbiAgXG4gIC5kaXJlY3RpdmUoJ3R5cGVhaGVhZFBvcHVwJywgWyckdHlwZWFoZWFkU3VwcHJlc3NXYXJuaW5nJywgJyRsb2cnLCBmdW5jdGlvbigkdHlwZWFoZWFkU3VwcHJlc3NXYXJuaW5nLCAkbG9nKSB7XG4gICAgcmV0dXJuIHtcbiAgICAgIHNjb3BlOiB7XG4gICAgICAgIG1hdGNoZXM6ICc9JyxcbiAgICAgICAgcXVlcnk6ICc9JyxcbiAgICAgICAgYWN0aXZlOiAnPScsXG4gICAgICAgIHBvc2l0aW9uOiAnJicsXG4gICAgICAgIG1vdmVJblByb2dyZXNzOiAnPScsXG4gICAgICAgIHNlbGVjdDogJyYnXG4gICAgICB9LFxuICAgICAgcmVwbGFjZTogdHJ1ZSxcbiAgICAgIHRlbXBsYXRlVXJsOiBmdW5jdGlvbihlbGVtZW50LCBhdHRycykge1xuICAgICAgICByZXR1cm4gYXR0cnMucG9wdXBUZW1wbGF0ZVVybCB8fCAndGVtcGxhdGUvdHlwZWFoZWFkL3R5cGVhaGVhZC1wb3B1cC5odG1sJztcbiAgICAgIH0sXG4gICAgICBsaW5rOiBmdW5jdGlvbihzY29wZSwgZWxlbWVudCwgYXR0cnMpIHtcbiAgICAgICAgXG4gICAgICAgIGlmICghJHR5cGVhaGVhZFN1cHByZXNzV2FybmluZykge1xuICAgICAgICAgICRsb2cud2FybigndHlwZWFoZWFkLXBvcHVwIGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliLXR5cGVhaGVhZC1wb3B1cCBpbnN0ZWFkLicpO1xuICAgICAgICB9XG4gICAgICAgIHNjb3BlLnRlbXBsYXRlVXJsID0gYXR0cnMudGVtcGxhdGVVcmw7XG5cbiAgICAgICAgc2NvcGUuaXNPcGVuID0gZnVuY3Rpb24oKSB7XG4gICAgICAgICAgcmV0dXJuIHNjb3BlLm1hdGNoZXMubGVuZ3RoID4gMDtcbiAgICAgICAgfTtcblxuICAgICAgICBzY29wZS5pc0FjdGl2ZSA9IGZ1bmN0aW9uKG1hdGNoSWR4KSB7XG4gICAgICAgICAgcmV0dXJuIHNjb3BlLmFjdGl2ZSA9PSBtYXRjaElkeDtcbiAgICAgICAgfTtcblxuICAgICAgICBzY29wZS5zZWxlY3RBY3RpdmUgPSBmdW5jdGlvbihtYXRjaElkeCkge1xuICAgICAgICAgIHNjb3BlLmFjdGl2ZSA9IG1hdGNoSWR4O1xuICAgICAgICB9O1xuXG4gICAgICAgIHNjb3BlLnNlbGVjdE1hdGNoID0gZnVuY3Rpb24oYWN0aXZlSWR4KSB7XG4gICAgICAgICAgc2NvcGUuc2VsZWN0KHthY3RpdmVJZHg6YWN0aXZlSWR4fSk7XG4gICAgICAgIH07XG4gICAgICB9XG4gICAgfTtcbiAgfV0pXG4gIFxuICAuZGlyZWN0aXZlKCd0eXBlYWhlYWRNYXRjaCcsIFsnJHRlbXBsYXRlUmVxdWVzdCcsICckY29tcGlsZScsICckcGFyc2UnLCAnJHR5cGVhaGVhZFN1cHByZXNzV2FybmluZycsICckbG9nJywgZnVuY3Rpb24oJHRlbXBsYXRlUmVxdWVzdCwgJGNvbXBpbGUsICRwYXJzZSwgJHR5cGVhaGVhZFN1cHByZXNzV2FybmluZywgJGxvZykge1xuICAgIHJldHVybiB7XG4gICAgICByZXN0cmljdDogJ0VBJyxcbiAgICAgIHNjb3BlOiB7XG4gICAgICAgIGluZGV4OiAnPScsXG4gICAgICAgIG1hdGNoOiAnPScsXG4gICAgICAgIHF1ZXJ5OiAnPSdcbiAgICAgIH0sXG4gICAgICBsaW5rOmZ1bmN0aW9uKHNjb3BlLCBlbGVtZW50LCBhdHRycykge1xuICAgICAgICBpZiAoISR0eXBlYWhlYWRTdXBwcmVzc1dhcm5pbmcpIHtcbiAgICAgICAgICAkbG9nLndhcm4oJ3R5cGVhaGVhZC1tYXRjaCBpcyBub3cgZGVwcmVjYXRlZC4gVXNlIHVpYi10eXBlYWhlYWQtbWF0Y2ggaW5zdGVhZC4nKTtcbiAgICAgICAgfVxuXG4gICAgICAgIHZhciB0cGxVcmwgPSAkcGFyc2UoYXR0cnMudGVtcGxhdGVVcmwpKHNjb3BlLiRwYXJlbnQpIHx8ICd0ZW1wbGF0ZS90eXBlYWhlYWQvdHlwZWFoZWFkLW1hdGNoLmh0bWwnO1xuICAgICAgICAkdGVtcGxhdGVSZXF1ZXN0KHRwbFVybCkudGhlbihmdW5jdGlvbih0cGxDb250ZW50KSB7XG4gICAgICAgICAgJGNvbXBpbGUodHBsQ29udGVudC50cmltKCkpKHNjb3BlLCBmdW5jdGlvbihjbG9uZWRFbGVtZW50KSB7XG4gICAgICAgICAgICBlbGVtZW50LnJlcGxhY2VXaXRoKGNsb25lZEVsZW1lbnQpO1xuICAgICAgICAgIH0pO1xuICAgICAgICB9KTtcbiAgICAgIH1cbiAgICB9O1xuICB9XSlcbiAgXG4gIC5maWx0ZXIoJ3R5cGVhaGVhZEhpZ2hsaWdodCcsIFsnJHNjZScsICckaW5qZWN0b3InLCAnJGxvZycsICckdHlwZWFoZWFkU3VwcHJlc3NXYXJuaW5nJywgZnVuY3Rpb24oJHNjZSwgJGluamVjdG9yLCAkbG9nLCAkdHlwZWFoZWFkU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgdmFyIGlzU2FuaXRpemVQcmVzZW50O1xuICAgIGlzU2FuaXRpemVQcmVzZW50ID0gJGluamVjdG9yLmhhcygnJHNhbml0aXplJyk7XG5cbiAgICBmdW5jdGlvbiBlc2NhcGVSZWdleHAocXVlcnlUb0VzY2FwZSkge1xuICAgICAgLy8gUmVnZXg6IGNhcHR1cmUgdGhlIHdob2xlIHF1ZXJ5IHN0cmluZyBhbmQgcmVwbGFjZSBpdCB3aXRoIHRoZSBzdHJpbmcgdGhhdCB3aWxsIGJlIHVzZWQgdG8gbWF0Y2hcbiAgICAgIC8vIHRoZSByZXN1bHRzLCBmb3IgZXhhbXBsZSBpZiB0aGUgY2FwdHVyZSBpcyBcImFcIiB0aGUgcmVzdWx0IHdpbGwgYmUgXFxhXG4gICAgICByZXR1cm4gcXVlcnlUb0VzY2FwZS5yZXBsYWNlKC8oWy4/KiteJFtcXF1cXFxcKCl7fXwtXSkvZywgJ1xcXFwkMScpO1xuICAgIH1cblxuICAgIGZ1bmN0aW9uIGNvbnRhaW5zSHRtbChtYXRjaEl0ZW0pIHtcbiAgICAgIHJldHVybiAvPC4qPi9nLnRlc3QobWF0Y2hJdGVtKTtcbiAgICB9XG5cbiAgICByZXR1cm4gZnVuY3Rpb24obWF0Y2hJdGVtLCBxdWVyeSkge1xuICAgICAgaWYgKCEkdHlwZWFoZWFkU3VwcHJlc3NXYXJuaW5nKSB7XG4gICAgICAgICRsb2cud2FybigndHlwZWFoZWFkSGlnaGxpZ2h0IGlzIG5vdyBkZXByZWNhdGVkLiBVc2UgdWliVHlwZWFoZWFkSGlnaGxpZ2h0IGluc3RlYWQuJyk7XG4gICAgICB9XG5cbiAgICAgIGlmICghaXNTYW5pdGl6ZVByZXNlbnQgJiYgY29udGFpbnNIdG1sKG1hdGNoSXRlbSkpIHtcbiAgICAgICAgJGxvZy53YXJuKCdVbnNhZmUgdXNlIG9mIHR5cGVhaGVhZCBwbGVhc2UgdXNlIG5nU2FuaXRpemUnKTsgLy8gV2FybiB0aGUgdXNlciBhYm91dCB0aGUgZGFuZ2VyXG4gICAgICB9XG5cbiAgICAgIG1hdGNoSXRlbSA9IHF1ZXJ5PyAoJycgKyBtYXRjaEl0ZW0pLnJlcGxhY2UobmV3IFJlZ0V4cChlc2NhcGVSZWdleHAocXVlcnkpLCAnZ2knKSwgJzxzdHJvbmc+JCY8L3N0cm9uZz4nKSA6IG1hdGNoSXRlbTsgLy8gUmVwbGFjZXMgdGhlIGNhcHR1cmUgc3RyaW5nIHdpdGggYSB0aGUgc2FtZSBzdHJpbmcgaW5zaWRlIG9mIGEgXCJzdHJvbmdcIiB0YWdcbiAgICAgIGlmICghaXNTYW5pdGl6ZVByZXNlbnQpIHtcbiAgICAgICAgbWF0Y2hJdGVtID0gJHNjZS50cnVzdEFzSHRtbChtYXRjaEl0ZW0pOyAvLyBJZiAkc2FuaXRpemUgaXMgbm90IHByZXNlbnQgd2UgcGFjayB0aGUgc3RyaW5nIGluIGEgJHNjZSBvYmplY3QgZm9yIHRoZSBuZy1iaW5kLWh0bWwgZGlyZWN0aXZlXG4gICAgICB9XG5cbiAgICAgIHJldHVybiBtYXRjaEl0ZW07XG4gICAgfTtcbiAgfV0pO1xuXG5hbmd1bGFyLm1vZHVsZShcInRlbXBsYXRlL2FjY29yZGlvbi9hY2NvcmRpb24tZ3JvdXAuaHRtbFwiLCBbXSkucnVuKFtcIiR0ZW1wbGF0ZUNhY2hlXCIsIGZ1bmN0aW9uKCR0ZW1wbGF0ZUNhY2hlKSB7XG4gICR0ZW1wbGF0ZUNhY2hlLnB1dChcInRlbXBsYXRlL2FjY29yZGlvbi9hY2NvcmRpb24tZ3JvdXAuaHRtbFwiLFxuICAgIFwiPGRpdiBjbGFzcz1cXFwicGFuZWwge3twYW5lbENsYXNzIHx8ICdwYW5lbC1kZWZhdWx0J319XFxcIj5cXG5cIiArXG4gICAgXCIgIDxkaXYgY2xhc3M9XFxcInBhbmVsLWhlYWRpbmdcXFwiIG5nLWtleXByZXNzPVxcXCJ0b2dnbGVPcGVuKCRldmVudClcXFwiPlxcblwiICtcbiAgICBcIiAgICA8aDQgY2xhc3M9XFxcInBhbmVsLXRpdGxlXFxcIj5cXG5cIiArXG4gICAgXCIgICAgICA8YSBocmVmIHRhYmluZGV4PVxcXCIwXFxcIiBjbGFzcz1cXFwiYWNjb3JkaW9uLXRvZ2dsZVxcXCIgbmctY2xpY2s9XFxcInRvZ2dsZU9wZW4oKVxcXCIgdWliLWFjY29yZGlvbi10cmFuc2NsdWRlPVxcXCJoZWFkaW5nXFxcIj48c3BhbiBuZy1jbGFzcz1cXFwieyd0ZXh0LW11dGVkJzogaXNEaXNhYmxlZH1cXFwiPnt7aGVhZGluZ319PC9zcGFuPjwvYT5cXG5cIiArXG4gICAgXCIgICAgPC9oND5cXG5cIiArXG4gICAgXCIgIDwvZGl2PlxcblwiICtcbiAgICBcIiAgPGRpdiBjbGFzcz1cXFwicGFuZWwtY29sbGFwc2UgY29sbGFwc2VcXFwiIHVpYi1jb2xsYXBzZT1cXFwiIWlzT3BlblxcXCI+XFxuXCIgK1xuICAgIFwiXHQgIDxkaXYgY2xhc3M9XFxcInBhbmVsLWJvZHlcXFwiIG5nLXRyYW5zY2x1ZGU+PC9kaXY+XFxuXCIgK1xuICAgIFwiICA8L2Rpdj5cXG5cIiArXG4gICAgXCI8L2Rpdj5cXG5cIiArXG4gICAgXCJcIik7XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKFwidGVtcGxhdGUvYWNjb3JkaW9uL2FjY29yZGlvbi5odG1sXCIsIFtdKS5ydW4oW1wiJHRlbXBsYXRlQ2FjaGVcIiwgZnVuY3Rpb24oJHRlbXBsYXRlQ2FjaGUpIHtcbiAgJHRlbXBsYXRlQ2FjaGUucHV0KFwidGVtcGxhdGUvYWNjb3JkaW9uL2FjY29yZGlvbi5odG1sXCIsXG4gICAgXCI8ZGl2IGNsYXNzPVxcXCJwYW5lbC1ncm91cFxcXCIgbmctdHJhbnNjbHVkZT48L2Rpdj5cIik7XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKFwidGVtcGxhdGUvYWxlcnQvYWxlcnQuaHRtbFwiLCBbXSkucnVuKFtcIiR0ZW1wbGF0ZUNhY2hlXCIsIGZ1bmN0aW9uKCR0ZW1wbGF0ZUNhY2hlKSB7XG4gICR0ZW1wbGF0ZUNhY2hlLnB1dChcInRlbXBsYXRlL2FsZXJ0L2FsZXJ0Lmh0bWxcIixcbiAgICBcIjxkaXYgY2xhc3M9XFxcImFsZXJ0XFxcIiBuZy1jbGFzcz1cXFwiWydhbGVydC0nICsgKHR5cGUgfHwgJ3dhcm5pbmcnKSwgY2xvc2VhYmxlID8gJ2FsZXJ0LWRpc21pc3NpYmxlJyA6IG51bGxdXFxcIiByb2xlPVxcXCJhbGVydFxcXCI+XFxuXCIgK1xuICAgIFwiICAgIDxidXR0b24gbmctc2hvdz1cXFwiY2xvc2VhYmxlXFxcIiB0eXBlPVxcXCJidXR0b25cXFwiIGNsYXNzPVxcXCJjbG9zZVxcXCIgbmctY2xpY2s9XFxcImNsb3NlKHskZXZlbnQ6ICRldmVudH0pXFxcIj5cXG5cIiArXG4gICAgXCIgICAgICAgIDxzcGFuIGFyaWEtaGlkZGVuPVxcXCJ0cnVlXFxcIj4mdGltZXM7PC9zcGFuPlxcblwiICtcbiAgICBcIiAgICAgICAgPHNwYW4gY2xhc3M9XFxcInNyLW9ubHlcXFwiPkNsb3NlPC9zcGFuPlxcblwiICtcbiAgICBcIiAgICA8L2J1dHRvbj5cXG5cIiArXG4gICAgXCIgICAgPGRpdiBuZy10cmFuc2NsdWRlPjwvZGl2PlxcblwiICtcbiAgICBcIjwvZGl2PlxcblwiICtcbiAgICBcIlwiKTtcbn1dKTtcblxuYW5ndWxhci5tb2R1bGUoXCJ0ZW1wbGF0ZS9jYXJvdXNlbC9jYXJvdXNlbC5odG1sXCIsIFtdKS5ydW4oW1wiJHRlbXBsYXRlQ2FjaGVcIiwgZnVuY3Rpb24oJHRlbXBsYXRlQ2FjaGUpIHtcbiAgJHRlbXBsYXRlQ2FjaGUucHV0KFwidGVtcGxhdGUvY2Fyb3VzZWwvY2Fyb3VzZWwuaHRtbFwiLFxuICAgIFwiPGRpdiBuZy1tb3VzZWVudGVyPVxcXCJwYXVzZSgpXFxcIiBuZy1tb3VzZWxlYXZlPVxcXCJwbGF5KClcXFwiIGNsYXNzPVxcXCJjYXJvdXNlbFxcXCIgbmctc3dpcGUtcmlnaHQ9XFxcInByZXYoKVxcXCIgbmctc3dpcGUtbGVmdD1cXFwibmV4dCgpXFxcIj5cXG5cIiArXG4gICAgXCIgIDxkaXYgY2xhc3M9XFxcImNhcm91c2VsLWlubmVyXFxcIiBuZy10cmFuc2NsdWRlPjwvZGl2PlxcblwiICtcbiAgICBcIiAgPGEgcm9sZT1cXFwiYnV0dG9uXFxcIiBocmVmIGNsYXNzPVxcXCJsZWZ0IGNhcm91c2VsLWNvbnRyb2xcXFwiIG5nLWNsaWNrPVxcXCJwcmV2KClcXFwiIG5nLXNob3c9XFxcInNsaWRlcy5sZW5ndGggPiAxXFxcIj5cXG5cIiArXG4gICAgXCIgICAgPHNwYW4gYXJpYS1oaWRkZW49XFxcInRydWVcXFwiIGNsYXNzPVxcXCJnbHlwaGljb24gZ2x5cGhpY29uLWNoZXZyb24tbGVmdFxcXCI+PC9zcGFuPlxcblwiICtcbiAgICBcIiAgICA8c3BhbiBjbGFzcz1cXFwic3Itb25seVxcXCI+cHJldmlvdXM8L3NwYW4+XFxuXCIgK1xuICAgIFwiICA8L2E+XFxuXCIgK1xuICAgIFwiICA8YSByb2xlPVxcXCJidXR0b25cXFwiIGhyZWYgY2xhc3M9XFxcInJpZ2h0IGNhcm91c2VsLWNvbnRyb2xcXFwiIG5nLWNsaWNrPVxcXCJuZXh0KClcXFwiIG5nLXNob3c9XFxcInNsaWRlcy5sZW5ndGggPiAxXFxcIj5cXG5cIiArXG4gICAgXCIgICAgPHNwYW4gYXJpYS1oaWRkZW49XFxcInRydWVcXFwiIGNsYXNzPVxcXCJnbHlwaGljb24gZ2x5cGhpY29uLWNoZXZyb24tcmlnaHRcXFwiPjwvc3Bhbj5cXG5cIiArXG4gICAgXCIgICAgPHNwYW4gY2xhc3M9XFxcInNyLW9ubHlcXFwiPm5leHQ8L3NwYW4+XFxuXCIgK1xuICAgIFwiICA8L2E+XFxuXCIgK1xuICAgIFwiICA8b2wgY2xhc3M9XFxcImNhcm91c2VsLWluZGljYXRvcnNcXFwiIG5nLXNob3c9XFxcInNsaWRlcy5sZW5ndGggPiAxXFxcIj5cXG5cIiArXG4gICAgXCIgICAgPGxpIG5nLXJlcGVhdD1cXFwic2xpZGUgaW4gc2xpZGVzIHwgb3JkZXJCeTppbmRleE9mU2xpZGUgdHJhY2sgYnkgJGluZGV4XFxcIiBuZy1jbGFzcz1cXFwieyBhY3RpdmU6IGlzQWN0aXZlKHNsaWRlKSB9XFxcIiBuZy1jbGljaz1cXFwic2VsZWN0KHNsaWRlKVxcXCI+XFxuXCIgK1xuICAgIFwiICAgICAgPHNwYW4gY2xhc3M9XFxcInNyLW9ubHlcXFwiPnNsaWRlIHt7ICRpbmRleCArIDEgfX0gb2Yge3sgc2xpZGVzLmxlbmd0aCB9fTxzcGFuIG5nLWlmPVxcXCJpc0FjdGl2ZShzbGlkZSlcXFwiPiwgY3VycmVudGx5IGFjdGl2ZTwvc3Bhbj48L3NwYW4+XFxuXCIgK1xuICAgIFwiICAgIDwvbGk+XFxuXCIgK1xuICAgIFwiICA8L29sPlxcblwiICtcbiAgICBcIjwvZGl2PlwiKTtcbn1dKTtcblxuYW5ndWxhci5tb2R1bGUoXCJ0ZW1wbGF0ZS9jYXJvdXNlbC9zbGlkZS5odG1sXCIsIFtdKS5ydW4oW1wiJHRlbXBsYXRlQ2FjaGVcIiwgZnVuY3Rpb24oJHRlbXBsYXRlQ2FjaGUpIHtcbiAgJHRlbXBsYXRlQ2FjaGUucHV0KFwidGVtcGxhdGUvY2Fyb3VzZWwvc2xpZGUuaHRtbFwiLFxuICAgIFwiPGRpdiBuZy1jbGFzcz1cXFwie1xcblwiICtcbiAgICBcIiAgICAnYWN0aXZlJzogYWN0aXZlXFxuXCIgK1xuICAgIFwiICB9XFxcIiBjbGFzcz1cXFwiaXRlbSB0ZXh0LWNlbnRlclxcXCIgbmctdHJhbnNjbHVkZT48L2Rpdj5cXG5cIiArXG4gICAgXCJcIik7XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKFwidGVtcGxhdGUvZGF0ZXBpY2tlci9kYXRlcGlja2VyLmh0bWxcIiwgW10pLnJ1bihbXCIkdGVtcGxhdGVDYWNoZVwiLCBmdW5jdGlvbigkdGVtcGxhdGVDYWNoZSkge1xuICAkdGVtcGxhdGVDYWNoZS5wdXQoXCJ0ZW1wbGF0ZS9kYXRlcGlja2VyL2RhdGVwaWNrZXIuaHRtbFwiLFxuICAgIFwiPGRpdiBuZy1zd2l0Y2g9XFxcImRhdGVwaWNrZXJNb2RlXFxcIiByb2xlPVxcXCJhcHBsaWNhdGlvblxcXCIgbmcta2V5ZG93bj1cXFwia2V5ZG93bigkZXZlbnQpXFxcIj5cXG5cIiArXG4gICAgXCIgIDx1aWItZGF5cGlja2VyIG5nLXN3aXRjaC13aGVuPVxcXCJkYXlcXFwiIHRhYmluZGV4PVxcXCIwXFxcIj48L3VpYi1kYXlwaWNrZXI+XFxuXCIgK1xuICAgIFwiICA8dWliLW1vbnRocGlja2VyIG5nLXN3aXRjaC13aGVuPVxcXCJtb250aFxcXCIgdGFiaW5kZXg9XFxcIjBcXFwiPjwvdWliLW1vbnRocGlja2VyPlxcblwiICtcbiAgICBcIiAgPHVpYi15ZWFycGlja2VyIG5nLXN3aXRjaC13aGVuPVxcXCJ5ZWFyXFxcIiB0YWJpbmRleD1cXFwiMFxcXCI+PC91aWIteWVhcnBpY2tlcj5cXG5cIiArXG4gICAgXCI8L2Rpdj5cIik7XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKFwidGVtcGxhdGUvZGF0ZXBpY2tlci9kYXkuaHRtbFwiLCBbXSkucnVuKFtcIiR0ZW1wbGF0ZUNhY2hlXCIsIGZ1bmN0aW9uKCR0ZW1wbGF0ZUNhY2hlKSB7XG4gICR0ZW1wbGF0ZUNhY2hlLnB1dChcInRlbXBsYXRlL2RhdGVwaWNrZXIvZGF5Lmh0bWxcIixcbiAgICBcIjx0YWJsZSByb2xlPVxcXCJncmlkXFxcIiBhcmlhLWxhYmVsbGVkYnk9XFxcInt7Ojp1bmlxdWVJZH19LXRpdGxlXFxcIiBhcmlhLWFjdGl2ZWRlc2NlbmRhbnQ9XFxcInt7YWN0aXZlRGF0ZUlkfX1cXFwiPlxcblwiICtcbiAgICBcIiAgPHRoZWFkPlxcblwiICtcbiAgICBcIiAgICA8dHI+XFxuXCIgK1xuICAgIFwiICAgICAgPHRoPjxidXR0b24gdHlwZT1cXFwiYnV0dG9uXFxcIiBjbGFzcz1cXFwiYnRuIGJ0bi1kZWZhdWx0IGJ0bi1zbSBwdWxsLWxlZnRcXFwiIG5nLWNsaWNrPVxcXCJtb3ZlKC0xKVxcXCIgdGFiaW5kZXg9XFxcIi0xXFxcIj48aSBjbGFzcz1cXFwiZ2x5cGhpY29uIGdseXBoaWNvbi1jaGV2cm9uLWxlZnRcXFwiPjwvaT48L2J1dHRvbj48L3RoPlxcblwiICtcbiAgICBcIiAgICAgIDx0aCBjb2xzcGFuPVxcXCJ7ezo6NSArIHNob3dXZWVrc319XFxcIj48YnV0dG9uIGlkPVxcXCJ7ezo6dW5pcXVlSWR9fS10aXRsZVxcXCIgcm9sZT1cXFwiaGVhZGluZ1xcXCIgYXJpYS1saXZlPVxcXCJhc3NlcnRpdmVcXFwiIGFyaWEtYXRvbWljPVxcXCJ0cnVlXFxcIiB0eXBlPVxcXCJidXR0b25cXFwiIGNsYXNzPVxcXCJidG4gYnRuLWRlZmF1bHQgYnRuLXNtXFxcIiBuZy1jbGljaz1cXFwidG9nZ2xlTW9kZSgpXFxcIiBuZy1kaXNhYmxlZD1cXFwiZGF0ZXBpY2tlck1vZGUgPT09IG1heE1vZGVcXFwiIHRhYmluZGV4PVxcXCItMVxcXCIgc3R5bGU9XFxcIndpZHRoOjEwMCU7XFxcIj48c3Ryb25nPnt7dGl0bGV9fTwvc3Ryb25nPjwvYnV0dG9uPjwvdGg+XFxuXCIgK1xuICAgIFwiICAgICAgPHRoPjxidXR0b24gdHlwZT1cXFwiYnV0dG9uXFxcIiBjbGFzcz1cXFwiYnRuIGJ0bi1kZWZhdWx0IGJ0bi1zbSBwdWxsLXJpZ2h0XFxcIiBuZy1jbGljaz1cXFwibW92ZSgxKVxcXCIgdGFiaW5kZXg9XFxcIi0xXFxcIj48aSBjbGFzcz1cXFwiZ2x5cGhpY29uIGdseXBoaWNvbi1jaGV2cm9uLXJpZ2h0XFxcIj48L2k+PC9idXR0b24+PC90aD5cXG5cIiArXG4gICAgXCIgICAgPC90cj5cXG5cIiArXG4gICAgXCIgICAgPHRyPlxcblwiICtcbiAgICBcIiAgICAgIDx0aCBuZy1pZj1cXFwic2hvd1dlZWtzXFxcIiBjbGFzcz1cXFwidGV4dC1jZW50ZXJcXFwiPjwvdGg+XFxuXCIgK1xuICAgIFwiICAgICAgPHRoIG5nLXJlcGVhdD1cXFwibGFiZWwgaW4gOjpsYWJlbHMgdHJhY2sgYnkgJGluZGV4XFxcIiBjbGFzcz1cXFwidGV4dC1jZW50ZXJcXFwiPjxzbWFsbCBhcmlhLWxhYmVsPVxcXCJ7ezo6bGFiZWwuZnVsbH19XFxcIj57ezo6bGFiZWwuYWJicn19PC9zbWFsbD48L3RoPlxcblwiICtcbiAgICBcIiAgICA8L3RyPlxcblwiICtcbiAgICBcIiAgPC90aGVhZD5cXG5cIiArXG4gICAgXCIgIDx0Ym9keT5cXG5cIiArXG4gICAgXCIgICAgPHRyIG5nLXJlcGVhdD1cXFwicm93IGluIHJvd3MgdHJhY2sgYnkgJGluZGV4XFxcIj5cXG5cIiArXG4gICAgXCIgICAgICA8dGQgbmctaWY9XFxcInNob3dXZWVrc1xcXCIgY2xhc3M9XFxcInRleHQtY2VudGVyIGg2XFxcIj48ZW0+e3sgd2Vla051bWJlcnNbJGluZGV4XSB9fTwvZW0+PC90ZD5cXG5cIiArXG4gICAgXCIgICAgICA8dGQgbmctcmVwZWF0PVxcXCJkdCBpbiByb3cgdHJhY2sgYnkgZHQuZGF0ZVxcXCIgY2xhc3M9XFxcInRleHQtY2VudGVyXFxcIiByb2xlPVxcXCJncmlkY2VsbFxcXCIgaWQ9XFxcInt7OjpkdC51aWR9fVxcXCIgbmctY2xhc3M9XFxcIjo6ZHQuY3VzdG9tQ2xhc3NcXFwiPlxcblwiICtcbiAgICBcIiAgICAgICAgPGJ1dHRvbiB0eXBlPVxcXCJidXR0b25cXFwiIHN0eWxlPVxcXCJtaW4td2lkdGg6MTAwJTtcXFwiIGNsYXNzPVxcXCJidG4gYnRuLWRlZmF1bHQgYnRuLXNtXFxcIiBuZy1jbGFzcz1cXFwieydidG4taW5mbyc6IGR0LnNlbGVjdGVkLCBhY3RpdmU6IGlzQWN0aXZlKGR0KX1cXFwiIG5nLWNsaWNrPVxcXCJzZWxlY3QoZHQuZGF0ZSlcXFwiIG5nLWRpc2FibGVkPVxcXCJkdC5kaXNhYmxlZFxcXCIgdGFiaW5kZXg9XFxcIi0xXFxcIj48c3BhbiBuZy1jbGFzcz1cXFwiOjp7J3RleHQtbXV0ZWQnOiBkdC5zZWNvbmRhcnksICd0ZXh0LWluZm8nOiBkdC5jdXJyZW50fVxcXCI+e3s6OmR0LmxhYmVsfX08L3NwYW4+PC9idXR0b24+XFxuXCIgK1xuICAgIFwiICAgICAgPC90ZD5cXG5cIiArXG4gICAgXCIgICAgPC90cj5cXG5cIiArXG4gICAgXCIgIDwvdGJvZHk+XFxuXCIgK1xuICAgIFwiPC90YWJsZT5cXG5cIiArXG4gICAgXCJcIik7XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKFwidGVtcGxhdGUvZGF0ZXBpY2tlci9tb250aC5odG1sXCIsIFtdKS5ydW4oW1wiJHRlbXBsYXRlQ2FjaGVcIiwgZnVuY3Rpb24oJHRlbXBsYXRlQ2FjaGUpIHtcbiAgJHRlbXBsYXRlQ2FjaGUucHV0KFwidGVtcGxhdGUvZGF0ZXBpY2tlci9tb250aC5odG1sXCIsXG4gICAgXCI8dGFibGUgcm9sZT1cXFwiZ3JpZFxcXCIgYXJpYS1sYWJlbGxlZGJ5PVxcXCJ7ezo6dW5pcXVlSWR9fS10aXRsZVxcXCIgYXJpYS1hY3RpdmVkZXNjZW5kYW50PVxcXCJ7e2FjdGl2ZURhdGVJZH19XFxcIj5cXG5cIiArXG4gICAgXCIgIDx0aGVhZD5cXG5cIiArXG4gICAgXCIgICAgPHRyPlxcblwiICtcbiAgICBcIiAgICAgIDx0aD48YnV0dG9uIHR5cGU9XFxcImJ1dHRvblxcXCIgY2xhc3M9XFxcImJ0biBidG4tZGVmYXVsdCBidG4tc20gcHVsbC1sZWZ0XFxcIiBuZy1jbGljaz1cXFwibW92ZSgtMSlcXFwiIHRhYmluZGV4PVxcXCItMVxcXCI+PGkgY2xhc3M9XFxcImdseXBoaWNvbiBnbHlwaGljb24tY2hldnJvbi1sZWZ0XFxcIj48L2k+PC9idXR0b24+PC90aD5cXG5cIiArXG4gICAgXCIgICAgICA8dGg+PGJ1dHRvbiBpZD1cXFwie3s6OnVuaXF1ZUlkfX0tdGl0bGVcXFwiIHJvbGU9XFxcImhlYWRpbmdcXFwiIGFyaWEtbGl2ZT1cXFwiYXNzZXJ0aXZlXFxcIiBhcmlhLWF0b21pYz1cXFwidHJ1ZVxcXCIgdHlwZT1cXFwiYnV0dG9uXFxcIiBjbGFzcz1cXFwiYnRuIGJ0bi1kZWZhdWx0IGJ0bi1zbVxcXCIgbmctY2xpY2s9XFxcInRvZ2dsZU1vZGUoKVxcXCIgbmctZGlzYWJsZWQ9XFxcImRhdGVwaWNrZXJNb2RlID09PSBtYXhNb2RlXFxcIiB0YWJpbmRleD1cXFwiLTFcXFwiIHN0eWxlPVxcXCJ3aWR0aDoxMDAlO1xcXCI+PHN0cm9uZz57e3RpdGxlfX08L3N0cm9uZz48L2J1dHRvbj48L3RoPlxcblwiICtcbiAgICBcIiAgICAgIDx0aD48YnV0dG9uIHR5cGU9XFxcImJ1dHRvblxcXCIgY2xhc3M9XFxcImJ0biBidG4tZGVmYXVsdCBidG4tc20gcHVsbC1yaWdodFxcXCIgbmctY2xpY2s9XFxcIm1vdmUoMSlcXFwiIHRhYmluZGV4PVxcXCItMVxcXCI+PGkgY2xhc3M9XFxcImdseXBoaWNvbiBnbHlwaGljb24tY2hldnJvbi1yaWdodFxcXCI+PC9pPjwvYnV0dG9uPjwvdGg+XFxuXCIgK1xuICAgIFwiICAgIDwvdHI+XFxuXCIgK1xuICAgIFwiICA8L3RoZWFkPlxcblwiICtcbiAgICBcIiAgPHRib2R5PlxcblwiICtcbiAgICBcIiAgICA8dHIgbmctcmVwZWF0PVxcXCJyb3cgaW4gcm93cyB0cmFjayBieSAkaW5kZXhcXFwiPlxcblwiICtcbiAgICBcIiAgICAgIDx0ZCBuZy1yZXBlYXQ9XFxcImR0IGluIHJvdyB0cmFjayBieSBkdC5kYXRlXFxcIiBjbGFzcz1cXFwidGV4dC1jZW50ZXJcXFwiIHJvbGU9XFxcImdyaWRjZWxsXFxcIiBpZD1cXFwie3s6OmR0LnVpZH19XFxcIiBuZy1jbGFzcz1cXFwiOjpkdC5jdXN0b21DbGFzc1xcXCI+XFxuXCIgK1xuICAgIFwiICAgICAgICA8YnV0dG9uIHR5cGU9XFxcImJ1dHRvblxcXCIgc3R5bGU9XFxcIm1pbi13aWR0aDoxMDAlO1xcXCIgY2xhc3M9XFxcImJ0biBidG4tZGVmYXVsdFxcXCIgbmctY2xhc3M9XFxcInsnYnRuLWluZm8nOiBkdC5zZWxlY3RlZCwgYWN0aXZlOiBpc0FjdGl2ZShkdCl9XFxcIiBuZy1jbGljaz1cXFwic2VsZWN0KGR0LmRhdGUpXFxcIiBuZy1kaXNhYmxlZD1cXFwiZHQuZGlzYWJsZWRcXFwiIHRhYmluZGV4PVxcXCItMVxcXCI+PHNwYW4gbmctY2xhc3M9XFxcIjo6eyd0ZXh0LWluZm8nOiBkdC5jdXJyZW50fVxcXCI+e3s6OmR0LmxhYmVsfX08L3NwYW4+PC9idXR0b24+XFxuXCIgK1xuICAgIFwiICAgICAgPC90ZD5cXG5cIiArXG4gICAgXCIgICAgPC90cj5cXG5cIiArXG4gICAgXCIgIDwvdGJvZHk+XFxuXCIgK1xuICAgIFwiPC90YWJsZT5cXG5cIiArXG4gICAgXCJcIik7XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKFwidGVtcGxhdGUvZGF0ZXBpY2tlci9wb3B1cC5odG1sXCIsIFtdKS5ydW4oW1wiJHRlbXBsYXRlQ2FjaGVcIiwgZnVuY3Rpb24oJHRlbXBsYXRlQ2FjaGUpIHtcbiAgJHRlbXBsYXRlQ2FjaGUucHV0KFwidGVtcGxhdGUvZGF0ZXBpY2tlci9wb3B1cC5odG1sXCIsXG4gICAgXCI8dWwgY2xhc3M9XFxcImRyb3Bkb3duLW1lbnVcXFwiIGRyb3Bkb3duLW5lc3RlZCBuZy1pZj1cXFwiaXNPcGVuXFxcIiBzdHlsZT1cXFwiZGlzcGxheTogYmxvY2tcXFwiIG5nLXN0eWxlPVxcXCJ7dG9wOiBwb3NpdGlvbi50b3ArJ3B4JywgbGVmdDogcG9zaXRpb24ubGVmdCsncHgnfVxcXCIgbmcta2V5ZG93bj1cXFwia2V5ZG93bigkZXZlbnQpXFxcIiBuZy1jbGljaz1cXFwiJGV2ZW50LnN0b3BQcm9wYWdhdGlvbigpXFxcIj5cXG5cIiArXG4gICAgXCJcdDxsaSBuZy10cmFuc2NsdWRlPjwvbGk+XFxuXCIgK1xuICAgIFwiXHQ8bGkgbmctaWY9XFxcInNob3dCdXR0b25CYXJcXFwiIHN0eWxlPVxcXCJwYWRkaW5nOjEwcHggOXB4IDJweFxcXCI+XFxuXCIgK1xuICAgIFwiXHRcdDxzcGFuIGNsYXNzPVxcXCJidG4tZ3JvdXAgcHVsbC1sZWZ0XFxcIj5cXG5cIiArXG4gICAgXCJcdFx0XHQ8YnV0dG9uIHR5cGU9XFxcImJ1dHRvblxcXCIgY2xhc3M9XFxcImJ0biBidG4tc20gYnRuLWluZm9cXFwiIG5nLWNsaWNrPVxcXCJzZWxlY3QoJ3RvZGF5JylcXFwiIG5nLWRpc2FibGVkPVxcXCJpc0Rpc2FibGVkKCd0b2RheScpXFxcIj57eyBnZXRUZXh0KCdjdXJyZW50JykgfX08L2J1dHRvbj5cXG5cIiArXG4gICAgXCJcdFx0XHQ8YnV0dG9uIHR5cGU9XFxcImJ1dHRvblxcXCIgY2xhc3M9XFxcImJ0biBidG4tc20gYnRuLWRhbmdlclxcXCIgbmctY2xpY2s9XFxcInNlbGVjdChudWxsKVxcXCI+e3sgZ2V0VGV4dCgnY2xlYXInKSB9fTwvYnV0dG9uPlxcblwiICtcbiAgICBcIlx0XHQ8L3NwYW4+XFxuXCIgK1xuICAgIFwiXHRcdDxidXR0b24gdHlwZT1cXFwiYnV0dG9uXFxcIiBjbGFzcz1cXFwiYnRuIGJ0bi1zbSBidG4tc3VjY2VzcyBwdWxsLXJpZ2h0XFxcIiBuZy1jbGljaz1cXFwiY2xvc2UoKVxcXCI+e3sgZ2V0VGV4dCgnY2xvc2UnKSB9fTwvYnV0dG9uPlxcblwiICtcbiAgICBcIlx0PC9saT5cXG5cIiArXG4gICAgXCI8L3VsPlxcblwiICtcbiAgICBcIlwiKTtcbn1dKTtcblxuYW5ndWxhci5tb2R1bGUoXCJ0ZW1wbGF0ZS9kYXRlcGlja2VyL3llYXIuaHRtbFwiLCBbXSkucnVuKFtcIiR0ZW1wbGF0ZUNhY2hlXCIsIGZ1bmN0aW9uKCR0ZW1wbGF0ZUNhY2hlKSB7XG4gICR0ZW1wbGF0ZUNhY2hlLnB1dChcInRlbXBsYXRlL2RhdGVwaWNrZXIveWVhci5odG1sXCIsXG4gICAgXCI8dGFibGUgcm9sZT1cXFwiZ3JpZFxcXCIgYXJpYS1sYWJlbGxlZGJ5PVxcXCJ7ezo6dW5pcXVlSWR9fS10aXRsZVxcXCIgYXJpYS1hY3RpdmVkZXNjZW5kYW50PVxcXCJ7e2FjdGl2ZURhdGVJZH19XFxcIj5cXG5cIiArXG4gICAgXCIgIDx0aGVhZD5cXG5cIiArXG4gICAgXCIgICAgPHRyPlxcblwiICtcbiAgICBcIiAgICAgIDx0aD48YnV0dG9uIHR5cGU9XFxcImJ1dHRvblxcXCIgY2xhc3M9XFxcImJ0biBidG4tZGVmYXVsdCBidG4tc20gcHVsbC1sZWZ0XFxcIiBuZy1jbGljaz1cXFwibW92ZSgtMSlcXFwiIHRhYmluZGV4PVxcXCItMVxcXCI+PGkgY2xhc3M9XFxcImdseXBoaWNvbiBnbHlwaGljb24tY2hldnJvbi1sZWZ0XFxcIj48L2k+PC9idXR0b24+PC90aD5cXG5cIiArXG4gICAgXCIgICAgICA8dGggY29sc3Bhbj1cXFwiM1xcXCI+PGJ1dHRvbiBpZD1cXFwie3s6OnVuaXF1ZUlkfX0tdGl0bGVcXFwiIHJvbGU9XFxcImhlYWRpbmdcXFwiIGFyaWEtbGl2ZT1cXFwiYXNzZXJ0aXZlXFxcIiBhcmlhLWF0b21pYz1cXFwidHJ1ZVxcXCIgdHlwZT1cXFwiYnV0dG9uXFxcIiBjbGFzcz1cXFwiYnRuIGJ0bi1kZWZhdWx0IGJ0bi1zbVxcXCIgbmctY2xpY2s9XFxcInRvZ2dsZU1vZGUoKVxcXCIgbmctZGlzYWJsZWQ9XFxcImRhdGVwaWNrZXJNb2RlID09PSBtYXhNb2RlXFxcIiB0YWJpbmRleD1cXFwiLTFcXFwiIHN0eWxlPVxcXCJ3aWR0aDoxMDAlO1xcXCI+PHN0cm9uZz57e3RpdGxlfX08L3N0cm9uZz48L2J1dHRvbj48L3RoPlxcblwiICtcbiAgICBcIiAgICAgIDx0aD48YnV0dG9uIHR5cGU9XFxcImJ1dHRvblxcXCIgY2xhc3M9XFxcImJ0biBidG4tZGVmYXVsdCBidG4tc20gcHVsbC1yaWdodFxcXCIgbmctY2xpY2s9XFxcIm1vdmUoMSlcXFwiIHRhYmluZGV4PVxcXCItMVxcXCI+PGkgY2xhc3M9XFxcImdseXBoaWNvbiBnbHlwaGljb24tY2hldnJvbi1yaWdodFxcXCI+PC9pPjwvYnV0dG9uPjwvdGg+XFxuXCIgK1xuICAgIFwiICAgIDwvdHI+XFxuXCIgK1xuICAgIFwiICA8L3RoZWFkPlxcblwiICtcbiAgICBcIiAgPHRib2R5PlxcblwiICtcbiAgICBcIiAgICA8dHIgbmctcmVwZWF0PVxcXCJyb3cgaW4gcm93cyB0cmFjayBieSAkaW5kZXhcXFwiPlxcblwiICtcbiAgICBcIiAgICAgIDx0ZCBuZy1yZXBlYXQ9XFxcImR0IGluIHJvdyB0cmFjayBieSBkdC5kYXRlXFxcIiBjbGFzcz1cXFwidGV4dC1jZW50ZXJcXFwiIHJvbGU9XFxcImdyaWRjZWxsXFxcIiBpZD1cXFwie3s6OmR0LnVpZH19XFxcIiBuZy1jbGFzcz1cXFwiOjpkdC5jdXN0b21DbGFzc1xcXCI+XFxuXCIgK1xuICAgIFwiICAgICAgICA8YnV0dG9uIHR5cGU9XFxcImJ1dHRvblxcXCIgc3R5bGU9XFxcIm1pbi13aWR0aDoxMDAlO1xcXCIgY2xhc3M9XFxcImJ0biBidG4tZGVmYXVsdFxcXCIgbmctY2xhc3M9XFxcInsnYnRuLWluZm8nOiBkdC5zZWxlY3RlZCwgYWN0aXZlOiBpc0FjdGl2ZShkdCl9XFxcIiBuZy1jbGljaz1cXFwic2VsZWN0KGR0LmRhdGUpXFxcIiBuZy1kaXNhYmxlZD1cXFwiZHQuZGlzYWJsZWRcXFwiIHRhYmluZGV4PVxcXCItMVxcXCI+PHNwYW4gbmctY2xhc3M9XFxcIjo6eyd0ZXh0LWluZm8nOiBkdC5jdXJyZW50fVxcXCI+e3s6OmR0LmxhYmVsfX08L3NwYW4+PC9idXR0b24+XFxuXCIgK1xuICAgIFwiICAgICAgPC90ZD5cXG5cIiArXG4gICAgXCIgICAgPC90cj5cXG5cIiArXG4gICAgXCIgIDwvdGJvZHk+XFxuXCIgK1xuICAgIFwiPC90YWJsZT5cXG5cIiArXG4gICAgXCJcIik7XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKFwidGVtcGxhdGUvbW9kYWwvYmFja2Ryb3AuaHRtbFwiLCBbXSkucnVuKFtcIiR0ZW1wbGF0ZUNhY2hlXCIsIGZ1bmN0aW9uKCR0ZW1wbGF0ZUNhY2hlKSB7XG4gICR0ZW1wbGF0ZUNhY2hlLnB1dChcInRlbXBsYXRlL21vZGFsL2JhY2tkcm9wLmh0bWxcIixcbiAgICBcIjxkaXYgdWliLW1vZGFsLWFuaW1hdGlvbi1jbGFzcz1cXFwiZmFkZVxcXCJcXG5cIiArXG4gICAgXCIgICAgIG1vZGFsLWluLWNsYXNzPVxcXCJpblxcXCJcXG5cIiArXG4gICAgXCIgICAgIG5nLXN0eWxlPVxcXCJ7J3otaW5kZXgnOiAxMDQwICsgKGluZGV4ICYmIDEgfHwgMCkgKyBpbmRleCoxMH1cXFwiXFxuXCIgK1xuICAgIFwiPjwvZGl2PlxcblwiICtcbiAgICBcIlwiKTtcbn1dKTtcblxuYW5ndWxhci5tb2R1bGUoXCJ0ZW1wbGF0ZS9tb2RhbC93aW5kb3cuaHRtbFwiLCBbXSkucnVuKFtcIiR0ZW1wbGF0ZUNhY2hlXCIsIGZ1bmN0aW9uKCR0ZW1wbGF0ZUNhY2hlKSB7XG4gICR0ZW1wbGF0ZUNhY2hlLnB1dChcInRlbXBsYXRlL21vZGFsL3dpbmRvdy5odG1sXCIsXG4gICAgXCI8ZGl2IG1vZGFsLXJlbmRlcj1cXFwie3skaXNSZW5kZXJlZH19XFxcIiB0YWJpbmRleD1cXFwiLTFcXFwiIHJvbGU9XFxcImRpYWxvZ1xcXCIgY2xhc3M9XFxcIm1vZGFsXFxcIlxcblwiICtcbiAgICBcIiAgICB1aWItbW9kYWwtYW5pbWF0aW9uLWNsYXNzPVxcXCJmYWRlXFxcIlxcblwiICtcbiAgICBcIiAgICBtb2RhbC1pbi1jbGFzcz1cXFwiaW5cXFwiXFxuXCIgK1xuICAgIFwiICAgIG5nLXN0eWxlPVxcXCJ7J3otaW5kZXgnOiAxMDUwICsgaW5kZXgqMTAsIGRpc3BsYXk6ICdibG9jayd9XFxcIj5cXG5cIiArXG4gICAgXCIgICAgPGRpdiBjbGFzcz1cXFwibW9kYWwtZGlhbG9nXFxcIiBuZy1jbGFzcz1cXFwic2l6ZSA/ICdtb2RhbC0nICsgc2l6ZSA6ICcnXFxcIj48ZGl2IGNsYXNzPVxcXCJtb2RhbC1jb250ZW50XFxcIiB1aWItbW9kYWwtdHJhbnNjbHVkZT48L2Rpdj48L2Rpdj5cXG5cIiArXG4gICAgXCI8L2Rpdj5cXG5cIiArXG4gICAgXCJcIik7XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKFwidGVtcGxhdGUvcGFnaW5hdGlvbi9wYWdlci5odG1sXCIsIFtdKS5ydW4oW1wiJHRlbXBsYXRlQ2FjaGVcIiwgZnVuY3Rpb24oJHRlbXBsYXRlQ2FjaGUpIHtcbiAgJHRlbXBsYXRlQ2FjaGUucHV0KFwidGVtcGxhdGUvcGFnaW5hdGlvbi9wYWdlci5odG1sXCIsXG4gICAgXCI8dWwgY2xhc3M9XFxcInBhZ2VyXFxcIj5cXG5cIiArXG4gICAgXCIgIDxsaSBuZy1jbGFzcz1cXFwie2Rpc2FibGVkOiBub1ByZXZpb3VzKCl8fG5nRGlzYWJsZWQsIHByZXZpb3VzOiBhbGlnbn1cXFwiPjxhIGhyZWYgbmctY2xpY2s9XFxcInNlbGVjdFBhZ2UocGFnZSAtIDEsICRldmVudClcXFwiPnt7OjpnZXRUZXh0KCdwcmV2aW91cycpfX08L2E+PC9saT5cXG5cIiArXG4gICAgXCIgIDxsaSBuZy1jbGFzcz1cXFwie2Rpc2FibGVkOiBub05leHQoKXx8bmdEaXNhYmxlZCwgbmV4dDogYWxpZ259XFxcIj48YSBocmVmIG5nLWNsaWNrPVxcXCJzZWxlY3RQYWdlKHBhZ2UgKyAxLCAkZXZlbnQpXFxcIj57ezo6Z2V0VGV4dCgnbmV4dCcpfX08L2E+PC9saT5cXG5cIiArXG4gICAgXCI8L3VsPlxcblwiICtcbiAgICBcIlwiKTtcbn1dKTtcblxuYW5ndWxhci5tb2R1bGUoXCJ0ZW1wbGF0ZS9wYWdpbmF0aW9uL3BhZ2luYXRpb24uaHRtbFwiLCBbXSkucnVuKFtcIiR0ZW1wbGF0ZUNhY2hlXCIsIGZ1bmN0aW9uKCR0ZW1wbGF0ZUNhY2hlKSB7XG4gICR0ZW1wbGF0ZUNhY2hlLnB1dChcInRlbXBsYXRlL3BhZ2luYXRpb24vcGFnaW5hdGlvbi5odG1sXCIsXG4gICAgXCI8dWwgY2xhc3M9XFxcInBhZ2luYXRpb25cXFwiPlxcblwiICtcbiAgICBcIiAgPGxpIG5nLWlmPVxcXCI6OmJvdW5kYXJ5TGlua3NcXFwiIG5nLWNsYXNzPVxcXCJ7ZGlzYWJsZWQ6IG5vUHJldmlvdXMoKXx8bmdEaXNhYmxlZH1cXFwiIGNsYXNzPVxcXCJwYWdpbmF0aW9uLWZpcnN0XFxcIj48YSBocmVmIG5nLWNsaWNrPVxcXCJzZWxlY3RQYWdlKDEsICRldmVudClcXFwiPnt7OjpnZXRUZXh0KCdmaXJzdCcpfX08L2E+PC9saT5cXG5cIiArXG4gICAgXCIgIDxsaSBuZy1pZj1cXFwiOjpkaXJlY3Rpb25MaW5rc1xcXCIgbmctY2xhc3M9XFxcIntkaXNhYmxlZDogbm9QcmV2aW91cygpfHxuZ0Rpc2FibGVkfVxcXCIgY2xhc3M9XFxcInBhZ2luYXRpb24tcHJldlxcXCI+PGEgaHJlZiBuZy1jbGljaz1cXFwic2VsZWN0UGFnZShwYWdlIC0gMSwgJGV2ZW50KVxcXCI+e3s6OmdldFRleHQoJ3ByZXZpb3VzJyl9fTwvYT48L2xpPlxcblwiICtcbiAgICBcIiAgPGxpIG5nLXJlcGVhdD1cXFwicGFnZSBpbiBwYWdlcyB0cmFjayBieSAkaW5kZXhcXFwiIG5nLWNsYXNzPVxcXCJ7YWN0aXZlOiBwYWdlLmFjdGl2ZSxkaXNhYmxlZDogbmdEaXNhYmxlZCYmIXBhZ2UuYWN0aXZlfVxcXCIgY2xhc3M9XFxcInBhZ2luYXRpb24tcGFnZVxcXCI+PGEgaHJlZiBuZy1jbGljaz1cXFwic2VsZWN0UGFnZShwYWdlLm51bWJlciwgJGV2ZW50KVxcXCI+e3twYWdlLnRleHR9fTwvYT48L2xpPlxcblwiICtcbiAgICBcIiAgPGxpIG5nLWlmPVxcXCI6OmRpcmVjdGlvbkxpbmtzXFxcIiBuZy1jbGFzcz1cXFwie2Rpc2FibGVkOiBub05leHQoKXx8bmdEaXNhYmxlZH1cXFwiIGNsYXNzPVxcXCJwYWdpbmF0aW9uLW5leHRcXFwiPjxhIGhyZWYgbmctY2xpY2s9XFxcInNlbGVjdFBhZ2UocGFnZSArIDEsICRldmVudClcXFwiPnt7OjpnZXRUZXh0KCduZXh0Jyl9fTwvYT48L2xpPlxcblwiICtcbiAgICBcIiAgPGxpIG5nLWlmPVxcXCI6OmJvdW5kYXJ5TGlua3NcXFwiIG5nLWNsYXNzPVxcXCJ7ZGlzYWJsZWQ6IG5vTmV4dCgpfHxuZ0Rpc2FibGVkfVxcXCIgY2xhc3M9XFxcInBhZ2luYXRpb24tbGFzdFxcXCI+PGEgaHJlZiBuZy1jbGljaz1cXFwic2VsZWN0UGFnZSh0b3RhbFBhZ2VzLCAkZXZlbnQpXFxcIj57ezo6Z2V0VGV4dCgnbGFzdCcpfX08L2E+PC9saT5cXG5cIiArXG4gICAgXCI8L3VsPlxcblwiICtcbiAgICBcIlwiKTtcbn1dKTtcblxuYW5ndWxhci5tb2R1bGUoXCJ0ZW1wbGF0ZS90b29sdGlwL3Rvb2x0aXAtaHRtbC1wb3B1cC5odG1sXCIsIFtdKS5ydW4oW1wiJHRlbXBsYXRlQ2FjaGVcIiwgZnVuY3Rpb24oJHRlbXBsYXRlQ2FjaGUpIHtcbiAgJHRlbXBsYXRlQ2FjaGUucHV0KFwidGVtcGxhdGUvdG9vbHRpcC90b29sdGlwLWh0bWwtcG9wdXAuaHRtbFwiLFxuICAgIFwiPGRpdlxcblwiICtcbiAgICBcIiAgdG9vbHRpcC1hbmltYXRpb24tY2xhc3M9XFxcImZhZGVcXFwiXFxuXCIgK1xuICAgIFwiICB1aWItdG9vbHRpcC1jbGFzc2VzXFxuXCIgK1xuICAgIFwiICBuZy1jbGFzcz1cXFwieyBpbjogaXNPcGVuKCkgfVxcXCI+XFxuXCIgK1xuICAgIFwiICA8ZGl2IGNsYXNzPVxcXCJ0b29sdGlwLWFycm93XFxcIj48L2Rpdj5cXG5cIiArXG4gICAgXCIgIDxkaXYgY2xhc3M9XFxcInRvb2x0aXAtaW5uZXJcXFwiIG5nLWJpbmQtaHRtbD1cXFwiY29udGVudEV4cCgpXFxcIj48L2Rpdj5cXG5cIiArXG4gICAgXCI8L2Rpdj5cXG5cIiArXG4gICAgXCJcIik7XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKFwidGVtcGxhdGUvdG9vbHRpcC90b29sdGlwLXBvcHVwLmh0bWxcIiwgW10pLnJ1bihbXCIkdGVtcGxhdGVDYWNoZVwiLCBmdW5jdGlvbigkdGVtcGxhdGVDYWNoZSkge1xuICAkdGVtcGxhdGVDYWNoZS5wdXQoXCJ0ZW1wbGF0ZS90b29sdGlwL3Rvb2x0aXAtcG9wdXAuaHRtbFwiLFxuICAgIFwiPGRpdlxcblwiICtcbiAgICBcIiAgdG9vbHRpcC1hbmltYXRpb24tY2xhc3M9XFxcImZhZGVcXFwiXFxuXCIgK1xuICAgIFwiICB1aWItdG9vbHRpcC1jbGFzc2VzXFxuXCIgK1xuICAgIFwiICBuZy1jbGFzcz1cXFwieyBpbjogaXNPcGVuKCkgfVxcXCI+XFxuXCIgK1xuICAgIFwiICA8ZGl2IGNsYXNzPVxcXCJ0b29sdGlwLWFycm93XFxcIj48L2Rpdj5cXG5cIiArXG4gICAgXCIgIDxkaXYgY2xhc3M9XFxcInRvb2x0aXAtaW5uZXJcXFwiIG5nLWJpbmQ9XFxcImNvbnRlbnRcXFwiPjwvZGl2PlxcblwiICtcbiAgICBcIjwvZGl2PlxcblwiICtcbiAgICBcIlwiKTtcbn1dKTtcblxuYW5ndWxhci5tb2R1bGUoXCJ0ZW1wbGF0ZS90b29sdGlwL3Rvb2x0aXAtdGVtcGxhdGUtcG9wdXAuaHRtbFwiLCBbXSkucnVuKFtcIiR0ZW1wbGF0ZUNhY2hlXCIsIGZ1bmN0aW9uKCR0ZW1wbGF0ZUNhY2hlKSB7XG4gICR0ZW1wbGF0ZUNhY2hlLnB1dChcInRlbXBsYXRlL3Rvb2x0aXAvdG9vbHRpcC10ZW1wbGF0ZS1wb3B1cC5odG1sXCIsXG4gICAgXCI8ZGl2XFxuXCIgK1xuICAgIFwiICB0b29sdGlwLWFuaW1hdGlvbi1jbGFzcz1cXFwiZmFkZVxcXCJcXG5cIiArXG4gICAgXCIgIHVpYi10b29sdGlwLWNsYXNzZXNcXG5cIiArXG4gICAgXCIgIG5nLWNsYXNzPVxcXCJ7IGluOiBpc09wZW4oKSB9XFxcIj5cXG5cIiArXG4gICAgXCIgIDxkaXYgY2xhc3M9XFxcInRvb2x0aXAtYXJyb3dcXFwiPjwvZGl2PlxcblwiICtcbiAgICBcIiAgPGRpdiBjbGFzcz1cXFwidG9vbHRpcC1pbm5lclxcXCJcXG5cIiArXG4gICAgXCIgICAgdWliLXRvb2x0aXAtdGVtcGxhdGUtdHJhbnNjbHVkZT1cXFwiY29udGVudEV4cCgpXFxcIlxcblwiICtcbiAgICBcIiAgICB0b29sdGlwLXRlbXBsYXRlLXRyYW5zY2x1ZGUtc2NvcGU9XFxcIm9yaWdpblNjb3BlKClcXFwiPjwvZGl2PlxcblwiICtcbiAgICBcIjwvZGl2PlxcblwiICtcbiAgICBcIlwiKTtcbn1dKTtcblxuYW5ndWxhci5tb2R1bGUoXCJ0ZW1wbGF0ZS9wb3BvdmVyL3BvcG92ZXItaHRtbC5odG1sXCIsIFtdKS5ydW4oW1wiJHRlbXBsYXRlQ2FjaGVcIiwgZnVuY3Rpb24oJHRlbXBsYXRlQ2FjaGUpIHtcbiAgJHRlbXBsYXRlQ2FjaGUucHV0KFwidGVtcGxhdGUvcG9wb3Zlci9wb3BvdmVyLWh0bWwuaHRtbFwiLFxuICAgIFwiPGRpdiB0b29sdGlwLWFuaW1hdGlvbi1jbGFzcz1cXFwiZmFkZVxcXCJcXG5cIiArXG4gICAgXCIgIHVpYi10b29sdGlwLWNsYXNzZXNcXG5cIiArXG4gICAgXCIgIG5nLWNsYXNzPVxcXCJ7IGluOiBpc09wZW4oKSB9XFxcIj5cXG5cIiArXG4gICAgXCIgIDxkaXYgY2xhc3M9XFxcImFycm93XFxcIj48L2Rpdj5cXG5cIiArXG4gICAgXCJcXG5cIiArXG4gICAgXCIgIDxkaXYgY2xhc3M9XFxcInBvcG92ZXItaW5uZXJcXFwiPlxcblwiICtcbiAgICBcIiAgICAgIDxoMyBjbGFzcz1cXFwicG9wb3Zlci10aXRsZVxcXCIgbmctYmluZD1cXFwidGl0bGVcXFwiIG5nLWlmPVxcXCJ0aXRsZVxcXCI+PC9oMz5cXG5cIiArXG4gICAgXCIgICAgICA8ZGl2IGNsYXNzPVxcXCJwb3BvdmVyLWNvbnRlbnRcXFwiIG5nLWJpbmQtaHRtbD1cXFwiY29udGVudEV4cCgpXFxcIj48L2Rpdj5cXG5cIiArXG4gICAgXCIgIDwvZGl2PlxcblwiICtcbiAgICBcIjwvZGl2PlxcblwiICtcbiAgICBcIlwiKTtcbn1dKTtcblxuYW5ndWxhci5tb2R1bGUoXCJ0ZW1wbGF0ZS9wb3BvdmVyL3BvcG92ZXItdGVtcGxhdGUuaHRtbFwiLCBbXSkucnVuKFtcIiR0ZW1wbGF0ZUNhY2hlXCIsIGZ1bmN0aW9uKCR0ZW1wbGF0ZUNhY2hlKSB7XG4gICR0ZW1wbGF0ZUNhY2hlLnB1dChcInRlbXBsYXRlL3BvcG92ZXIvcG9wb3Zlci10ZW1wbGF0ZS5odG1sXCIsXG4gICAgXCI8ZGl2IHRvb2x0aXAtYW5pbWF0aW9uLWNsYXNzPVxcXCJmYWRlXFxcIlxcblwiICtcbiAgICBcIiAgdWliLXRvb2x0aXAtY2xhc3Nlc1xcblwiICtcbiAgICBcIiAgbmctY2xhc3M9XFxcInsgaW46IGlzT3BlbigpIH1cXFwiPlxcblwiICtcbiAgICBcIiAgPGRpdiBjbGFzcz1cXFwiYXJyb3dcXFwiPjwvZGl2PlxcblwiICtcbiAgICBcIlxcblwiICtcbiAgICBcIiAgPGRpdiBjbGFzcz1cXFwicG9wb3Zlci1pbm5lclxcXCI+XFxuXCIgK1xuICAgIFwiICAgICAgPGgzIGNsYXNzPVxcXCJwb3BvdmVyLXRpdGxlXFxcIiBuZy1iaW5kPVxcXCJ0aXRsZVxcXCIgbmctaWY9XFxcInRpdGxlXFxcIj48L2gzPlxcblwiICtcbiAgICBcIiAgICAgIDxkaXYgY2xhc3M9XFxcInBvcG92ZXItY29udGVudFxcXCJcXG5cIiArXG4gICAgXCIgICAgICAgIHVpYi10b29sdGlwLXRlbXBsYXRlLXRyYW5zY2x1ZGU9XFxcImNvbnRlbnRFeHAoKVxcXCJcXG5cIiArXG4gICAgXCIgICAgICAgIHRvb2x0aXAtdGVtcGxhdGUtdHJhbnNjbHVkZS1zY29wZT1cXFwib3JpZ2luU2NvcGUoKVxcXCI+PC9kaXY+XFxuXCIgK1xuICAgIFwiICA8L2Rpdj5cXG5cIiArXG4gICAgXCI8L2Rpdj5cXG5cIiArXG4gICAgXCJcIik7XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKFwidGVtcGxhdGUvcG9wb3Zlci9wb3BvdmVyLmh0bWxcIiwgW10pLnJ1bihbXCIkdGVtcGxhdGVDYWNoZVwiLCBmdW5jdGlvbigkdGVtcGxhdGVDYWNoZSkge1xuICAkdGVtcGxhdGVDYWNoZS5wdXQoXCJ0ZW1wbGF0ZS9wb3BvdmVyL3BvcG92ZXIuaHRtbFwiLFxuICAgIFwiPGRpdiB0b29sdGlwLWFuaW1hdGlvbi1jbGFzcz1cXFwiZmFkZVxcXCJcXG5cIiArXG4gICAgXCIgIHVpYi10b29sdGlwLWNsYXNzZXNcXG5cIiArXG4gICAgXCIgIG5nLWNsYXNzPVxcXCJ7IGluOiBpc09wZW4oKSB9XFxcIj5cXG5cIiArXG4gICAgXCIgIDxkaXYgY2xhc3M9XFxcImFycm93XFxcIj48L2Rpdj5cXG5cIiArXG4gICAgXCJcXG5cIiArXG4gICAgXCIgIDxkaXYgY2xhc3M9XFxcInBvcG92ZXItaW5uZXJcXFwiPlxcblwiICtcbiAgICBcIiAgICAgIDxoMyBjbGFzcz1cXFwicG9wb3Zlci10aXRsZVxcXCIgbmctYmluZD1cXFwidGl0bGVcXFwiIG5nLWlmPVxcXCJ0aXRsZVxcXCI+PC9oMz5cXG5cIiArXG4gICAgXCIgICAgICA8ZGl2IGNsYXNzPVxcXCJwb3BvdmVyLWNvbnRlbnRcXFwiIG5nLWJpbmQ9XFxcImNvbnRlbnRcXFwiPjwvZGl2PlxcblwiICtcbiAgICBcIiAgPC9kaXY+XFxuXCIgK1xuICAgIFwiPC9kaXY+XFxuXCIgK1xuICAgIFwiXCIpO1xufV0pO1xuXG5hbmd1bGFyLm1vZHVsZShcInRlbXBsYXRlL3Byb2dyZXNzYmFyL2Jhci5odG1sXCIsIFtdKS5ydW4oW1wiJHRlbXBsYXRlQ2FjaGVcIiwgZnVuY3Rpb24oJHRlbXBsYXRlQ2FjaGUpIHtcbiAgJHRlbXBsYXRlQ2FjaGUucHV0KFwidGVtcGxhdGUvcHJvZ3Jlc3NiYXIvYmFyLmh0bWxcIixcbiAgICBcIjxkaXYgY2xhc3M9XFxcInByb2dyZXNzLWJhclxcXCIgbmctY2xhc3M9XFxcInR5cGUgJiYgJ3Byb2dyZXNzLWJhci0nICsgdHlwZVxcXCIgcm9sZT1cXFwicHJvZ3Jlc3NiYXJcXFwiIGFyaWEtdmFsdWVub3c9XFxcInt7dmFsdWV9fVxcXCIgYXJpYS12YWx1ZW1pbj1cXFwiMFxcXCIgYXJpYS12YWx1ZW1heD1cXFwie3ttYXh9fVxcXCIgbmctc3R5bGU9XFxcInt3aWR0aDogKHBlcmNlbnQgPCAxMDAgPyBwZXJjZW50IDogMTAwKSArICclJ31cXFwiIGFyaWEtdmFsdWV0ZXh0PVxcXCJ7e3BlcmNlbnQgfCBudW1iZXI6MH19JVxcXCIgYXJpYS1sYWJlbGxlZGJ5PVxcXCJ7ezo6dGl0bGV9fVxcXCIgc3R5bGU9XFxcIm1pbi13aWR0aDogMDtcXFwiIG5nLXRyYW5zY2x1ZGU+PC9kaXY+XFxuXCIgK1xuICAgIFwiXCIpO1xufV0pO1xuXG5hbmd1bGFyLm1vZHVsZShcInRlbXBsYXRlL3Byb2dyZXNzYmFyL3Byb2dyZXNzLmh0bWxcIiwgW10pLnJ1bihbXCIkdGVtcGxhdGVDYWNoZVwiLCBmdW5jdGlvbigkdGVtcGxhdGVDYWNoZSkge1xuICAkdGVtcGxhdGVDYWNoZS5wdXQoXCJ0ZW1wbGF0ZS9wcm9ncmVzc2Jhci9wcm9ncmVzcy5odG1sXCIsXG4gICAgXCI8ZGl2IGNsYXNzPVxcXCJwcm9ncmVzc1xcXCIgbmctdHJhbnNjbHVkZSBhcmlhLWxhYmVsbGVkYnk9XFxcInt7Ojp0aXRsZX19XFxcIj48L2Rpdj5cIik7XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKFwidGVtcGxhdGUvcHJvZ3Jlc3NiYXIvcHJvZ3Jlc3NiYXIuaHRtbFwiLCBbXSkucnVuKFtcIiR0ZW1wbGF0ZUNhY2hlXCIsIGZ1bmN0aW9uKCR0ZW1wbGF0ZUNhY2hlKSB7XG4gICR0ZW1wbGF0ZUNhY2hlLnB1dChcInRlbXBsYXRlL3Byb2dyZXNzYmFyL3Byb2dyZXNzYmFyLmh0bWxcIixcbiAgICBcIjxkaXYgY2xhc3M9XFxcInByb2dyZXNzXFxcIj5cXG5cIiArXG4gICAgXCIgIDxkaXYgY2xhc3M9XFxcInByb2dyZXNzLWJhclxcXCIgbmctY2xhc3M9XFxcInR5cGUgJiYgJ3Byb2dyZXNzLWJhci0nICsgdHlwZVxcXCIgcm9sZT1cXFwicHJvZ3Jlc3NiYXJcXFwiIGFyaWEtdmFsdWVub3c9XFxcInt7dmFsdWV9fVxcXCIgYXJpYS12YWx1ZW1pbj1cXFwiMFxcXCIgYXJpYS12YWx1ZW1heD1cXFwie3ttYXh9fVxcXCIgbmctc3R5bGU9XFxcInt3aWR0aDogKHBlcmNlbnQgPCAxMDAgPyBwZXJjZW50IDogMTAwKSArICclJ31cXFwiIGFyaWEtdmFsdWV0ZXh0PVxcXCJ7e3BlcmNlbnQgfCBudW1iZXI6MH19JVxcXCIgYXJpYS1sYWJlbGxlZGJ5PVxcXCJ7ezo6dGl0bGV9fVxcXCIgc3R5bGU9XFxcIm1pbi13aWR0aDogMDtcXFwiIG5nLXRyYW5zY2x1ZGU+PC9kaXY+XFxuXCIgK1xuICAgIFwiPC9kaXY+XFxuXCIgK1xuICAgIFwiXCIpO1xufV0pO1xuXG5hbmd1bGFyLm1vZHVsZShcInRlbXBsYXRlL3JhdGluZy9yYXRpbmcuaHRtbFwiLCBbXSkucnVuKFtcIiR0ZW1wbGF0ZUNhY2hlXCIsIGZ1bmN0aW9uKCR0ZW1wbGF0ZUNhY2hlKSB7XG4gICR0ZW1wbGF0ZUNhY2hlLnB1dChcInRlbXBsYXRlL3JhdGluZy9yYXRpbmcuaHRtbFwiLFxuICAgIFwiPHNwYW4gbmctbW91c2VsZWF2ZT1cXFwicmVzZXQoKVxcXCIgbmcta2V5ZG93bj1cXFwib25LZXlkb3duKCRldmVudClcXFwiIHRhYmluZGV4PVxcXCIwXFxcIiByb2xlPVxcXCJzbGlkZXJcXFwiIGFyaWEtdmFsdWVtaW49XFxcIjBcXFwiIGFyaWEtdmFsdWVtYXg9XFxcInt7cmFuZ2UubGVuZ3RofX1cXFwiIGFyaWEtdmFsdWVub3c9XFxcInt7dmFsdWV9fVxcXCI+XFxuXCIgK1xuICAgIFwiICAgIDxzcGFuIG5nLXJlcGVhdC1zdGFydD1cXFwiciBpbiByYW5nZSB0cmFjayBieSAkaW5kZXhcXFwiIGNsYXNzPVxcXCJzci1vbmx5XFxcIj4oe3sgJGluZGV4IDwgdmFsdWUgPyAnKicgOiAnICcgfX0pPC9zcGFuPlxcblwiICtcbiAgICBcIiAgICA8aSBuZy1yZXBlYXQtZW5kIG5nLW1vdXNlZW50ZXI9XFxcImVudGVyKCRpbmRleCArIDEpXFxcIiBuZy1jbGljaz1cXFwicmF0ZSgkaW5kZXggKyAxKVxcXCIgY2xhc3M9XFxcImdseXBoaWNvblxcXCIgbmctY2xhc3M9XFxcIiRpbmRleCA8IHZhbHVlICYmIChyLnN0YXRlT24gfHwgJ2dseXBoaWNvbi1zdGFyJykgfHwgKHIuc3RhdGVPZmYgfHwgJ2dseXBoaWNvbi1zdGFyLWVtcHR5JylcXFwiIG5nLWF0dHItdGl0bGU9XFxcInt7ci50aXRsZX19XFxcIiBhcmlhLXZhbHVldGV4dD1cXFwie3tyLnRpdGxlfX1cXFwiPjwvaT5cXG5cIiArXG4gICAgXCI8L3NwYW4+XFxuXCIgK1xuICAgIFwiXCIpO1xufV0pO1xuXG5hbmd1bGFyLm1vZHVsZShcInRlbXBsYXRlL3RhYnMvdGFiLmh0bWxcIiwgW10pLnJ1bihbXCIkdGVtcGxhdGVDYWNoZVwiLCBmdW5jdGlvbigkdGVtcGxhdGVDYWNoZSkge1xuICAkdGVtcGxhdGVDYWNoZS5wdXQoXCJ0ZW1wbGF0ZS90YWJzL3RhYi5odG1sXCIsXG4gICAgXCI8bGkgbmctY2xhc3M9XFxcInthY3RpdmU6IGFjdGl2ZSwgZGlzYWJsZWQ6IGRpc2FibGVkfVxcXCI+XFxuXCIgK1xuICAgIFwiICA8YSBocmVmIG5nLWNsaWNrPVxcXCJzZWxlY3QoKVxcXCIgdWliLXRhYi1oZWFkaW5nLXRyYW5zY2x1ZGU+e3toZWFkaW5nfX08L2E+XFxuXCIgK1xuICAgIFwiPC9saT5cXG5cIiArXG4gICAgXCJcIik7XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKFwidGVtcGxhdGUvdGFicy90YWJzZXQuaHRtbFwiLCBbXSkucnVuKFtcIiR0ZW1wbGF0ZUNhY2hlXCIsIGZ1bmN0aW9uKCR0ZW1wbGF0ZUNhY2hlKSB7XG4gICR0ZW1wbGF0ZUNhY2hlLnB1dChcInRlbXBsYXRlL3RhYnMvdGFic2V0Lmh0bWxcIixcbiAgICBcIjxkaXY+XFxuXCIgK1xuICAgIFwiICA8dWwgY2xhc3M9XFxcIm5hdiBuYXYte3t0eXBlIHx8ICd0YWJzJ319XFxcIiBuZy1jbGFzcz1cXFwieyduYXYtc3RhY2tlZCc6IHZlcnRpY2FsLCAnbmF2LWp1c3RpZmllZCc6IGp1c3RpZmllZH1cXFwiIG5nLXRyYW5zY2x1ZGU+PC91bD5cXG5cIiArXG4gICAgXCIgIDxkaXYgY2xhc3M9XFxcInRhYi1jb250ZW50XFxcIj5cXG5cIiArXG4gICAgXCIgICAgPGRpdiBjbGFzcz1cXFwidGFiLXBhbmVcXFwiIFxcblwiICtcbiAgICBcIiAgICAgICAgIG5nLXJlcGVhdD1cXFwidGFiIGluIHRhYnNcXFwiIFxcblwiICtcbiAgICBcIiAgICAgICAgIG5nLWNsYXNzPVxcXCJ7YWN0aXZlOiB0YWIuYWN0aXZlfVxcXCJcXG5cIiArXG4gICAgXCIgICAgICAgICB1aWItdGFiLWNvbnRlbnQtdHJhbnNjbHVkZT1cXFwidGFiXFxcIj5cXG5cIiArXG4gICAgXCIgICAgPC9kaXY+XFxuXCIgK1xuICAgIFwiICA8L2Rpdj5cXG5cIiArXG4gICAgXCI8L2Rpdj5cXG5cIiArXG4gICAgXCJcIik7XG59XSk7XG5cbmFuZ3VsYXIubW9kdWxlKFwidGVtcGxhdGUvdGltZXBpY2tlci90aW1lcGlja2VyLmh0bWxcIiwgW10pLnJ1bihbXCIkdGVtcGxhdGVDYWNoZVwiLCBmdW5jdGlvbigkdGVtcGxhdGVDYWNoZSkge1xuICAkdGVtcGxhdGVDYWNoZS5wdXQoXCJ0ZW1wbGF0ZS90aW1lcGlja2VyL3RpbWVwaWNrZXIuaHRtbFwiLFxuICAgIFwiPHRhYmxlPlxcblwiICtcbiAgICBcIiAgPHRib2R5PlxcblwiICtcbiAgICBcIiAgICA8dHIgY2xhc3M9XFxcInRleHQtY2VudGVyXFxcIiBuZy1zaG93PVxcXCI6OnNob3dTcGlubmVyc1xcXCI+XFxuXCIgK1xuICAgIFwiICAgICAgPHRkPjxhIG5nLWNsaWNrPVxcXCJpbmNyZW1lbnRIb3VycygpXFxcIiBuZy1jbGFzcz1cXFwie2Rpc2FibGVkOiBub0luY3JlbWVudEhvdXJzKCl9XFxcIiBjbGFzcz1cXFwiYnRuIGJ0bi1saW5rXFxcIiBuZy1kaXNhYmxlZD1cXFwibm9JbmNyZW1lbnRIb3VycygpXFxcIiB0YWJpbmRleD1cXFwie3s6OnRhYmluZGV4fX1cXFwiPjxzcGFuIGNsYXNzPVxcXCJnbHlwaGljb24gZ2x5cGhpY29uLWNoZXZyb24tdXBcXFwiPjwvc3Bhbj48L2E+PC90ZD5cXG5cIiArXG4gICAgXCIgICAgICA8dGQ+Jm5ic3A7PC90ZD5cXG5cIiArXG4gICAgXCIgICAgICA8dGQ+PGEgbmctY2xpY2s9XFxcImluY3JlbWVudE1pbnV0ZXMoKVxcXCIgbmctY2xhc3M9XFxcIntkaXNhYmxlZDogbm9JbmNyZW1lbnRNaW51dGVzKCl9XFxcIiBjbGFzcz1cXFwiYnRuIGJ0bi1saW5rXFxcIiBuZy1kaXNhYmxlZD1cXFwibm9JbmNyZW1lbnRNaW51dGVzKClcXFwiIHRhYmluZGV4PVxcXCJ7ezo6dGFiaW5kZXh9fVxcXCI+PHNwYW4gY2xhc3M9XFxcImdseXBoaWNvbiBnbHlwaGljb24tY2hldnJvbi11cFxcXCI+PC9zcGFuPjwvYT48L3RkPlxcblwiICtcbiAgICBcIiAgICAgIDx0ZCBuZy1zaG93PVxcXCJzaG93TWVyaWRpYW5cXFwiPjwvdGQ+XFxuXCIgK1xuICAgIFwiICAgIDwvdHI+XFxuXCIgK1xuICAgIFwiICAgIDx0cj5cXG5cIiArXG4gICAgXCIgICAgICA8dGQgY2xhc3M9XFxcImZvcm0tZ3JvdXBcXFwiIG5nLWNsYXNzPVxcXCJ7J2hhcy1lcnJvcic6IGludmFsaWRIb3Vyc31cXFwiPlxcblwiICtcbiAgICBcIiAgICAgICAgPGlucHV0IHN0eWxlPVxcXCJ3aWR0aDo1MHB4O1xcXCIgdHlwZT1cXFwidGV4dFxcXCIgbmctbW9kZWw9XFxcImhvdXJzXFxcIiBuZy1jaGFuZ2U9XFxcInVwZGF0ZUhvdXJzKClcXFwiIGNsYXNzPVxcXCJmb3JtLWNvbnRyb2wgdGV4dC1jZW50ZXJcXFwiIG5nLXJlYWRvbmx5PVxcXCI6OnJlYWRvbmx5SW5wdXRcXFwiIG1heGxlbmd0aD1cXFwiMlxcXCIgdGFiaW5kZXg9XFxcInt7Ojp0YWJpbmRleH19XFxcIj5cXG5cIiArXG4gICAgXCIgICAgICA8L3RkPlxcblwiICtcbiAgICBcIiAgICAgIDx0ZD46PC90ZD5cXG5cIiArXG4gICAgXCIgICAgICA8dGQgY2xhc3M9XFxcImZvcm0tZ3JvdXBcXFwiIG5nLWNsYXNzPVxcXCJ7J2hhcy1lcnJvcic6IGludmFsaWRNaW51dGVzfVxcXCI+XFxuXCIgK1xuICAgIFwiICAgICAgICA8aW5wdXQgc3R5bGU9XFxcIndpZHRoOjUwcHg7XFxcIiB0eXBlPVxcXCJ0ZXh0XFxcIiBuZy1tb2RlbD1cXFwibWludXRlc1xcXCIgbmctY2hhbmdlPVxcXCJ1cGRhdGVNaW51dGVzKClcXFwiIGNsYXNzPVxcXCJmb3JtLWNvbnRyb2wgdGV4dC1jZW50ZXJcXFwiIG5nLXJlYWRvbmx5PVxcXCI6OnJlYWRvbmx5SW5wdXRcXFwiIG1heGxlbmd0aD1cXFwiMlxcXCIgdGFiaW5kZXg9XFxcInt7Ojp0YWJpbmRleH19XFxcIj5cXG5cIiArXG4gICAgXCIgICAgICA8L3RkPlxcblwiICtcbiAgICBcIiAgICAgIDx0ZCBuZy1zaG93PVxcXCJzaG93TWVyaWRpYW5cXFwiPjxidXR0b24gdHlwZT1cXFwiYnV0dG9uXFxcIiBuZy1jbGFzcz1cXFwie2Rpc2FibGVkOiBub1RvZ2dsZU1lcmlkaWFuKCl9XFxcIiBjbGFzcz1cXFwiYnRuIGJ0bi1kZWZhdWx0IHRleHQtY2VudGVyXFxcIiBuZy1jbGljaz1cXFwidG9nZ2xlTWVyaWRpYW4oKVxcXCIgbmctZGlzYWJsZWQ9XFxcIm5vVG9nZ2xlTWVyaWRpYW4oKVxcXCIgdGFiaW5kZXg9XFxcInt7Ojp0YWJpbmRleH19XFxcIj57e21lcmlkaWFufX08L2J1dHRvbj48L3RkPlxcblwiICtcbiAgICBcIiAgICA8L3RyPlxcblwiICtcbiAgICBcIiAgICA8dHIgY2xhc3M9XFxcInRleHQtY2VudGVyXFxcIiBuZy1zaG93PVxcXCI6OnNob3dTcGlubmVyc1xcXCI+XFxuXCIgK1xuICAgIFwiICAgICAgPHRkPjxhIG5nLWNsaWNrPVxcXCJkZWNyZW1lbnRIb3VycygpXFxcIiBuZy1jbGFzcz1cXFwie2Rpc2FibGVkOiBub0RlY3JlbWVudEhvdXJzKCl9XFxcIiBjbGFzcz1cXFwiYnRuIGJ0bi1saW5rXFxcIiBuZy1kaXNhYmxlZD1cXFwibm9EZWNyZW1lbnRIb3VycygpXFxcIiB0YWJpbmRleD1cXFwie3s6OnRhYmluZGV4fX1cXFwiPjxzcGFuIGNsYXNzPVxcXCJnbHlwaGljb24gZ2x5cGhpY29uLWNoZXZyb24tZG93blxcXCI+PC9zcGFuPjwvYT48L3RkPlxcblwiICtcbiAgICBcIiAgICAgIDx0ZD4mbmJzcDs8L3RkPlxcblwiICtcbiAgICBcIiAgICAgIDx0ZD48YSBuZy1jbGljaz1cXFwiZGVjcmVtZW50TWludXRlcygpXFxcIiBuZy1jbGFzcz1cXFwie2Rpc2FibGVkOiBub0RlY3JlbWVudE1pbnV0ZXMoKX1cXFwiIGNsYXNzPVxcXCJidG4gYnRuLWxpbmtcXFwiIG5nLWRpc2FibGVkPVxcXCJub0RlY3JlbWVudE1pbnV0ZXMoKVxcXCIgdGFiaW5kZXg9XFxcInt7Ojp0YWJpbmRleH19XFxcIj48c3BhbiBjbGFzcz1cXFwiZ2x5cGhpY29uIGdseXBoaWNvbi1jaGV2cm9uLWRvd25cXFwiPjwvc3Bhbj48L2E+PC90ZD5cXG5cIiArXG4gICAgXCIgICAgICA8dGQgbmctc2hvdz1cXFwic2hvd01lcmlkaWFuXFxcIj48L3RkPlxcblwiICtcbiAgICBcIiAgICA8L3RyPlxcblwiICtcbiAgICBcIiAgPC90Ym9keT5cXG5cIiArXG4gICAgXCI8L3RhYmxlPlxcblwiICtcbiAgICBcIlwiKTtcbn1dKTtcblxuYW5ndWxhci5tb2R1bGUoXCJ0ZW1wbGF0ZS90eXBlYWhlYWQvdHlwZWFoZWFkLW1hdGNoLmh0bWxcIiwgW10pLnJ1bihbXCIkdGVtcGxhdGVDYWNoZVwiLCBmdW5jdGlvbigkdGVtcGxhdGVDYWNoZSkge1xuICAkdGVtcGxhdGVDYWNoZS5wdXQoXCJ0ZW1wbGF0ZS90eXBlYWhlYWQvdHlwZWFoZWFkLW1hdGNoLmh0bWxcIixcbiAgICBcIjxhIGhyZWYgdGFiaW5kZXg9XFxcIi0xXFxcIiBuZy1iaW5kLWh0bWw9XFxcIm1hdGNoLmxhYmVsIHwgdWliVHlwZWFoZWFkSGlnaGxpZ2h0OnF1ZXJ5XFxcIj48L2E+XFxuXCIgK1xuICAgIFwiXCIpO1xufV0pO1xuXG5hbmd1bGFyLm1vZHVsZShcInRlbXBsYXRlL3R5cGVhaGVhZC90eXBlYWhlYWQtcG9wdXAuaHRtbFwiLCBbXSkucnVuKFtcIiR0ZW1wbGF0ZUNhY2hlXCIsIGZ1bmN0aW9uKCR0ZW1wbGF0ZUNhY2hlKSB7XG4gICR0ZW1wbGF0ZUNhY2hlLnB1dChcInRlbXBsYXRlL3R5cGVhaGVhZC90eXBlYWhlYWQtcG9wdXAuaHRtbFwiLFxuICAgIFwiPHVsIGNsYXNzPVxcXCJkcm9wZG93bi1tZW51XFxcIiBuZy1zaG93PVxcXCJpc09wZW4oKSAmJiAhbW92ZUluUHJvZ3Jlc3NcXFwiIG5nLXN0eWxlPVxcXCJ7dG9wOiBwb3NpdGlvbigpLnRvcCsncHgnLCBsZWZ0OiBwb3NpdGlvbigpLmxlZnQrJ3B4J31cXFwiIHN0eWxlPVxcXCJkaXNwbGF5OiBibG9jaztcXFwiIHJvbGU9XFxcImxpc3Rib3hcXFwiIGFyaWEtaGlkZGVuPVxcXCJ7eyFpc09wZW4oKX19XFxcIj5cXG5cIiArXG4gICAgXCIgICAgPGxpIG5nLXJlcGVhdD1cXFwibWF0Y2ggaW4gbWF0Y2hlcyB0cmFjayBieSAkaW5kZXhcXFwiIG5nLWNsYXNzPVxcXCJ7YWN0aXZlOiBpc0FjdGl2ZSgkaW5kZXgpIH1cXFwiIG5nLW1vdXNlZW50ZXI9XFxcInNlbGVjdEFjdGl2ZSgkaW5kZXgpXFxcIiBuZy1jbGljaz1cXFwic2VsZWN0TWF0Y2goJGluZGV4KVxcXCIgcm9sZT1cXFwib3B0aW9uXFxcIiBpZD1cXFwie3s6Om1hdGNoLmlkfX1cXFwiPlxcblwiICtcbiAgICBcIiAgICAgICAgPGRpdiB1aWItdHlwZWFoZWFkLW1hdGNoIGluZGV4PVxcXCIkaW5kZXhcXFwiIG1hdGNoPVxcXCJtYXRjaFxcXCIgcXVlcnk9XFxcInF1ZXJ5XFxcIiB0ZW1wbGF0ZS11cmw9XFxcInRlbXBsYXRlVXJsXFxcIj48L2Rpdj5cXG5cIiArXG4gICAgXCIgICAgPC9saT5cXG5cIiArXG4gICAgXCI8L3VsPlxcblwiICtcbiAgICBcIlwiKTtcbn1dKTtcbiFhbmd1bGFyLiQkY3NwKCkgJiYgYW5ndWxhci5lbGVtZW50KGRvY3VtZW50KS5maW5kKCdoZWFkJykucHJlcGVuZCgnPHN0eWxlIHR5cGU9XCJ0ZXh0L2Nzc1wiPi5uZy1hbmltYXRlLml0ZW06bm90KC5sZWZ0KTpub3QoLnJpZ2h0KXstd2Via2l0LXRyYW5zaXRpb246MHMgZWFzZS1pbi1vdXQgbGVmdDt0cmFuc2l0aW9uOjBzIGVhc2UtaW4tb3V0IGxlZnR9PC9zdHlsZT4nKTtcblxuXG4vKioqKioqKioqKioqKioqKipcbiAqKiBXRUJQQUNLIEZPT1RFUlxuICoqIC9Vc2Vycy94dWdlL3Byb2plY3QvcXVhbnNoaS11aS9+L2FuZ3VsYXItdWktYm9vdHN0cmFwL3VpLWJvb3RzdHJhcC10cGxzLmpzXG4gKiogbW9kdWxlIGlkID0gMTg2XG4gKiogbW9kdWxlIGNodW5rcyA9IDAgMSAyIDNcbiAqKi8iLCJyZXF1aXJlKCcuL2FuZ3VsYXItc2FuaXRpemUnKTtcbm1vZHVsZS5leHBvcnRzID0gJ25nU2FuaXRpemUnO1xuXG5cblxuLyoqKioqKioqKioqKioqKioqXG4gKiogV0VCUEFDSyBGT09URVJcbiAqKiAvVXNlcnMveHVnZS9wcm9qZWN0L3F1YW5zaGktdWkvfi9hbmd1bGFyLXNhbml0aXplL2luZGV4LmpzXG4gKiogbW9kdWxlIGlkID0gMTg3XG4gKiogbW9kdWxlIGNodW5rcyA9IDAgMSAyIDMgNSA2IDdcbiAqKi8iLCIvKipcbiAqIEBsaWNlbnNlIEFuZ3VsYXJKUyB2MS41LjNcbiAqIChjKSAyMDEwLTIwMTYgR29vZ2xlLCBJbmMuIGh0dHA6Ly9hbmd1bGFyanMub3JnXG4gKiBMaWNlbnNlOiBNSVRcbiAqL1xuKGZ1bmN0aW9uKHdpbmRvdywgYW5ndWxhciwgdW5kZWZpbmVkKSB7J3VzZSBzdHJpY3QnO1xuXG4vKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqXG4gKiAgICAgQW55IGNvbW1pdHMgdG8gdGhpcyBmaWxlIHNob3VsZCBiZSByZXZpZXdlZCB3aXRoIHNlY3VyaXR5IGluIG1pbmQuICAqXG4gKiAgIENoYW5nZXMgdG8gdGhpcyBmaWxlIGNhbiBwb3RlbnRpYWxseSBjcmVhdGUgc2VjdXJpdHkgdnVsbmVyYWJpbGl0aWVzLiAqXG4gKiAgICAgICAgICBBbiBhcHByb3ZhbCBmcm9tIDIgQ29yZSBtZW1iZXJzIHdpdGggaGlzdG9yeSBvZiBtb2RpZnlpbmcgICAgICAqXG4gKiAgICAgICAgICAgICAgICAgICAgICAgICB0aGlzIGZpbGUgaXMgcmVxdWlyZWQuICAgICAgICAgICAgICAgICAgICAgICAgICAqXG4gKiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAqXG4gKiAgRG9lcyB0aGUgY2hhbmdlIHNvbWVob3cgYWxsb3cgZm9yIGFyYml0cmFyeSBqYXZhc2NyaXB0IHRvIGJlIGV4ZWN1dGVkPyAqXG4gKiAgICBPciBhbGxvd3MgZm9yIHNvbWVvbmUgdG8gY2hhbmdlIHRoZSBwcm90b3R5cGUgb2YgYnVpbHQtaW4gb2JqZWN0cz8gICAqXG4gKiAgICAgT3IgZ2l2ZXMgdW5kZXNpcmVkIGFjY2VzcyB0byB2YXJpYWJsZXMgbGlrZXMgZG9jdW1lbnQgb3Igd2luZG93PyAgICAqXG4gKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqICogKiAqL1xuXG52YXIgJHNhbml0aXplTWluRXJyID0gYW5ndWxhci4kJG1pbkVycignJHNhbml0aXplJyk7XG5cbi8qKlxuICogQG5nZG9jIG1vZHVsZVxuICogQG5hbWUgbmdTYW5pdGl6ZVxuICogQGRlc2NyaXB0aW9uXG4gKlxuICogIyBuZ1Nhbml0aXplXG4gKlxuICogVGhlIGBuZ1Nhbml0aXplYCBtb2R1bGUgcHJvdmlkZXMgZnVuY3Rpb25hbGl0eSB0byBzYW5pdGl6ZSBIVE1MLlxuICpcbiAqXG4gKiA8ZGl2IGRvYy1tb2R1bGUtY29tcG9uZW50cz1cIm5nU2FuaXRpemVcIj48L2Rpdj5cbiAqXG4gKiBTZWUge0BsaW5rIG5nU2FuaXRpemUuJHNhbml0aXplIGAkc2FuaXRpemVgfSBmb3IgdXNhZ2UuXG4gKi9cblxuLyoqXG4gKiBAbmdkb2Mgc2VydmljZVxuICogQG5hbWUgJHNhbml0aXplXG4gKiBAa2luZCBmdW5jdGlvblxuICpcbiAqIEBkZXNjcmlwdGlvblxuICogICBTYW5pdGl6ZXMgYW4gaHRtbCBzdHJpbmcgYnkgc3RyaXBwaW5nIGFsbCBwb3RlbnRpYWxseSBkYW5nZXJvdXMgdG9rZW5zLlxuICpcbiAqICAgVGhlIGlucHV0IGlzIHNhbml0aXplZCBieSBwYXJzaW5nIHRoZSBIVE1MIGludG8gdG9rZW5zLiBBbGwgc2FmZSB0b2tlbnMgKGZyb20gYSB3aGl0ZWxpc3QpIGFyZVxuICogICB0aGVuIHNlcmlhbGl6ZWQgYmFjayB0byBwcm9wZXJseSBlc2NhcGVkIGh0bWwgc3RyaW5nLiBUaGlzIG1lYW5zIHRoYXQgbm8gdW5zYWZlIGlucHV0IGNhbiBtYWtlXG4gKiAgIGl0IGludG8gdGhlIHJldHVybmVkIHN0cmluZy5cbiAqXG4gKiAgIFRoZSB3aGl0ZWxpc3QgZm9yIFVSTCBzYW5pdGl6YXRpb24gb2YgYXR0cmlidXRlIHZhbHVlcyBpcyBjb25maWd1cmVkIHVzaW5nIHRoZSBmdW5jdGlvbnNcbiAqICAgYGFIcmVmU2FuaXRpemF0aW9uV2hpdGVsaXN0YCBhbmQgYGltZ1NyY1Nhbml0aXphdGlvbldoaXRlbGlzdGAgb2Yge0BsaW5rIG5nLiRjb21waWxlUHJvdmlkZXJcbiAqICAgYCRjb21waWxlUHJvdmlkZXJgfS5cbiAqXG4gKiAgIFRoZSBpbnB1dCBtYXkgYWxzbyBjb250YWluIFNWRyBtYXJrdXAgaWYgdGhpcyBpcyBlbmFibGVkIHZpYSB7QGxpbmsgJHNhbml0aXplUHJvdmlkZXJ9LlxuICpcbiAqIEBwYXJhbSB7c3RyaW5nfSBodG1sIEhUTUwgaW5wdXQuXG4gKiBAcmV0dXJucyB7c3RyaW5nfSBTYW5pdGl6ZWQgSFRNTC5cbiAqXG4gKiBAZXhhbXBsZVxuICAgPGV4YW1wbGUgbW9kdWxlPVwic2FuaXRpemVFeGFtcGxlXCIgZGVwcz1cImFuZ3VsYXItc2FuaXRpemUuanNcIj5cbiAgIDxmaWxlIG5hbWU9XCJpbmRleC5odG1sXCI+XG4gICAgIDxzY3JpcHQ+XG4gICAgICAgICBhbmd1bGFyLm1vZHVsZSgnc2FuaXRpemVFeGFtcGxlJywgWyduZ1Nhbml0aXplJ10pXG4gICAgICAgICAgIC5jb250cm9sbGVyKCdFeGFtcGxlQ29udHJvbGxlcicsIFsnJHNjb3BlJywgJyRzY2UnLCBmdW5jdGlvbigkc2NvcGUsICRzY2UpIHtcbiAgICAgICAgICAgICAkc2NvcGUuc25pcHBldCA9XG4gICAgICAgICAgICAgICAnPHAgc3R5bGU9XCJjb2xvcjpibHVlXCI+YW4gaHRtbFxcbicgK1xuICAgICAgICAgICAgICAgJzxlbSBvbm1vdXNlb3Zlcj1cInRoaXMudGV4dENvbnRlbnQ9XFwnUFdOM0QhXFwnXCI+Y2xpY2sgaGVyZTwvZW0+XFxuJyArXG4gICAgICAgICAgICAgICAnc25pcHBldDwvcD4nO1xuICAgICAgICAgICAgICRzY29wZS5kZWxpYmVyYXRlbHlUcnVzdERhbmdlcm91c1NuaXBwZXQgPSBmdW5jdGlvbigpIHtcbiAgICAgICAgICAgICAgIHJldHVybiAkc2NlLnRydXN0QXNIdG1sKCRzY29wZS5zbmlwcGV0KTtcbiAgICAgICAgICAgICB9O1xuICAgICAgICAgICB9XSk7XG4gICAgIDwvc2NyaXB0PlxuICAgICA8ZGl2IG5nLWNvbnRyb2xsZXI9XCJFeGFtcGxlQ29udHJvbGxlclwiPlxuICAgICAgICBTbmlwcGV0OiA8dGV4dGFyZWEgbmctbW9kZWw9XCJzbmlwcGV0XCIgY29scz1cIjYwXCIgcm93cz1cIjNcIj48L3RleHRhcmVhPlxuICAgICAgIDx0YWJsZT5cbiAgICAgICAgIDx0cj5cbiAgICAgICAgICAgPHRkPkRpcmVjdGl2ZTwvdGQ+XG4gICAgICAgICAgIDx0ZD5Ib3c8L3RkPlxuICAgICAgICAgICA8dGQ+U291cmNlPC90ZD5cbiAgICAgICAgICAgPHRkPlJlbmRlcmVkPC90ZD5cbiAgICAgICAgIDwvdHI+XG4gICAgICAgICA8dHIgaWQ9XCJiaW5kLWh0bWwtd2l0aC1zYW5pdGl6ZVwiPlxuICAgICAgICAgICA8dGQ+bmctYmluZC1odG1sPC90ZD5cbiAgICAgICAgICAgPHRkPkF1dG9tYXRpY2FsbHkgdXNlcyAkc2FuaXRpemU8L3RkPlxuICAgICAgICAgICA8dGQ+PHByZT4mbHQ7ZGl2IG5nLWJpbmQtaHRtbD1cInNuaXBwZXRcIiZndDs8YnIvPiZsdDsvZGl2Jmd0OzwvcHJlPjwvdGQ+XG4gICAgICAgICAgIDx0ZD48ZGl2IG5nLWJpbmQtaHRtbD1cInNuaXBwZXRcIj48L2Rpdj48L3RkPlxuICAgICAgICAgPC90cj5cbiAgICAgICAgIDx0ciBpZD1cImJpbmQtaHRtbC13aXRoLXRydXN0XCI+XG4gICAgICAgICAgIDx0ZD5uZy1iaW5kLWh0bWw8L3RkPlxuICAgICAgICAgICA8dGQ+QnlwYXNzICRzYW5pdGl6ZSBieSBleHBsaWNpdGx5IHRydXN0aW5nIHRoZSBkYW5nZXJvdXMgdmFsdWU8L3RkPlxuICAgICAgICAgICA8dGQ+XG4gICAgICAgICAgIDxwcmU+Jmx0O2RpdiBuZy1iaW5kLWh0bWw9XCJkZWxpYmVyYXRlbHlUcnVzdERhbmdlcm91c1NuaXBwZXQoKVwiJmd0O1xuJmx0Oy9kaXYmZ3Q7PC9wcmU+XG4gICAgICAgICAgIDwvdGQ+XG4gICAgICAgICAgIDx0ZD48ZGl2IG5nLWJpbmQtaHRtbD1cImRlbGliZXJhdGVseVRydXN0RGFuZ2Vyb3VzU25pcHBldCgpXCI+PC9kaXY+PC90ZD5cbiAgICAgICAgIDwvdHI+XG4gICAgICAgICA8dHIgaWQ9XCJiaW5kLWRlZmF1bHRcIj5cbiAgICAgICAgICAgPHRkPm5nLWJpbmQ8L3RkPlxuICAgICAgICAgICA8dGQ+QXV0b21hdGljYWxseSBlc2NhcGVzPC90ZD5cbiAgICAgICAgICAgPHRkPjxwcmU+Jmx0O2RpdiBuZy1iaW5kPVwic25pcHBldFwiJmd0Ozxici8+Jmx0Oy9kaXYmZ3Q7PC9wcmU+PC90ZD5cbiAgICAgICAgICAgPHRkPjxkaXYgbmctYmluZD1cInNuaXBwZXRcIj48L2Rpdj48L3RkPlxuICAgICAgICAgPC90cj5cbiAgICAgICA8L3RhYmxlPlxuICAgICAgIDwvZGl2PlxuICAgPC9maWxlPlxuICAgPGZpbGUgbmFtZT1cInByb3RyYWN0b3IuanNcIiB0eXBlPVwicHJvdHJhY3RvclwiPlxuICAgICBpdCgnc2hvdWxkIHNhbml0aXplIHRoZSBodG1sIHNuaXBwZXQgYnkgZGVmYXVsdCcsIGZ1bmN0aW9uKCkge1xuICAgICAgIGV4cGVjdChlbGVtZW50KGJ5LmNzcygnI2JpbmQtaHRtbC13aXRoLXNhbml0aXplIGRpdicpKS5nZXRJbm5lckh0bWwoKSkuXG4gICAgICAgICB0b0JlKCc8cD5hbiBodG1sXFxuPGVtPmNsaWNrIGhlcmU8L2VtPlxcbnNuaXBwZXQ8L3A+Jyk7XG4gICAgIH0pO1xuXG4gICAgIGl0KCdzaG91bGQgaW5saW5lIHJhdyBzbmlwcGV0IGlmIGJvdW5kIHRvIGEgdHJ1c3RlZCB2YWx1ZScsIGZ1bmN0aW9uKCkge1xuICAgICAgIGV4cGVjdChlbGVtZW50KGJ5LmNzcygnI2JpbmQtaHRtbC13aXRoLXRydXN0IGRpdicpKS5nZXRJbm5lckh0bWwoKSkuXG4gICAgICAgICB0b0JlKFwiPHAgc3R5bGU9XFxcImNvbG9yOmJsdWVcXFwiPmFuIGh0bWxcXG5cIiArXG4gICAgICAgICAgICAgIFwiPGVtIG9ubW91c2VvdmVyPVxcXCJ0aGlzLnRleHRDb250ZW50PSdQV04zRCEnXFxcIj5jbGljayBoZXJlPC9lbT5cXG5cIiArXG4gICAgICAgICAgICAgIFwic25pcHBldDwvcD5cIik7XG4gICAgIH0pO1xuXG4gICAgIGl0KCdzaG91bGQgZXNjYXBlIHNuaXBwZXQgd2l0aG91dCBhbnkgZmlsdGVyJywgZnVuY3Rpb24oKSB7XG4gICAgICAgZXhwZWN0KGVsZW1lbnQoYnkuY3NzKCcjYmluZC1kZWZhdWx0IGRpdicpKS5nZXRJbm5lckh0bWwoKSkuXG4gICAgICAgICB0b0JlKFwiJmx0O3Agc3R5bGU9XFxcImNvbG9yOmJsdWVcXFwiJmd0O2FuIGh0bWxcXG5cIiArXG4gICAgICAgICAgICAgIFwiJmx0O2VtIG9ubW91c2VvdmVyPVxcXCJ0aGlzLnRleHRDb250ZW50PSdQV04zRCEnXFxcIiZndDtjbGljayBoZXJlJmx0Oy9lbSZndDtcXG5cIiArXG4gICAgICAgICAgICAgIFwic25pcHBldCZsdDsvcCZndDtcIik7XG4gICAgIH0pO1xuXG4gICAgIGl0KCdzaG91bGQgdXBkYXRlJywgZnVuY3Rpb24oKSB7XG4gICAgICAgZWxlbWVudChieS5tb2RlbCgnc25pcHBldCcpKS5jbGVhcigpO1xuICAgICAgIGVsZW1lbnQoYnkubW9kZWwoJ3NuaXBwZXQnKSkuc2VuZEtleXMoJ25ldyA8YiBvbmNsaWNrPVwiYWxlcnQoMSlcIj50ZXh0PC9iPicpO1xuICAgICAgIGV4cGVjdChlbGVtZW50KGJ5LmNzcygnI2JpbmQtaHRtbC13aXRoLXNhbml0aXplIGRpdicpKS5nZXRJbm5lckh0bWwoKSkuXG4gICAgICAgICB0b0JlKCduZXcgPGI+dGV4dDwvYj4nKTtcbiAgICAgICBleHBlY3QoZWxlbWVudChieS5jc3MoJyNiaW5kLWh0bWwtd2l0aC10cnVzdCBkaXYnKSkuZ2V0SW5uZXJIdG1sKCkpLnRvQmUoXG4gICAgICAgICAnbmV3IDxiIG9uY2xpY2s9XCJhbGVydCgxKVwiPnRleHQ8L2I+Jyk7XG4gICAgICAgZXhwZWN0KGVsZW1lbnQoYnkuY3NzKCcjYmluZC1kZWZhdWx0IGRpdicpKS5nZXRJbm5lckh0bWwoKSkudG9CZShcbiAgICAgICAgIFwibmV3ICZsdDtiIG9uY2xpY2s9XFxcImFsZXJ0KDEpXFxcIiZndDt0ZXh0Jmx0Oy9iJmd0O1wiKTtcbiAgICAgfSk7XG4gICA8L2ZpbGU+XG4gICA8L2V4YW1wbGU+XG4gKi9cblxuXG4vKipcbiAqIEBuZ2RvYyBwcm92aWRlclxuICogQG5hbWUgJHNhbml0aXplUHJvdmlkZXJcbiAqXG4gKiBAZGVzY3JpcHRpb25cbiAqIENyZWF0ZXMgYW5kIGNvbmZpZ3VyZXMge0BsaW5rICRzYW5pdGl6ZX0gaW5zdGFuY2UuXG4gKi9cbmZ1bmN0aW9uICRTYW5pdGl6ZVByb3ZpZGVyKCkge1xuICB2YXIgc3ZnRW5hYmxlZCA9IGZhbHNlO1xuXG4gIHRoaXMuJGdldCA9IFsnJCRzYW5pdGl6ZVVyaScsIGZ1bmN0aW9uKCQkc2FuaXRpemVVcmkpIHtcbiAgICBpZiAoc3ZnRW5hYmxlZCkge1xuICAgICAgYW5ndWxhci5leHRlbmQodmFsaWRFbGVtZW50cywgc3ZnRWxlbWVudHMpO1xuICAgIH1cbiAgICByZXR1cm4gZnVuY3Rpb24oaHRtbCkge1xuICAgICAgdmFyIGJ1ZiA9IFtdO1xuICAgICAgaHRtbFBhcnNlcihodG1sLCBodG1sU2FuaXRpemVXcml0ZXIoYnVmLCBmdW5jdGlvbih1cmksIGlzSW1hZ2UpIHtcbiAgICAgICAgcmV0dXJuICEvXnVuc2FmZTovLnRlc3QoJCRzYW5pdGl6ZVVyaSh1cmksIGlzSW1hZ2UpKTtcbiAgICAgIH0pKTtcbiAgICAgIHJldHVybiBidWYuam9pbignJyk7XG4gICAgfTtcbiAgfV07XG5cblxuICAvKipcbiAgICogQG5nZG9jIG1ldGhvZFxuICAgKiBAbmFtZSAkc2FuaXRpemVQcm92aWRlciNlbmFibGVTdmdcbiAgICogQGtpbmQgZnVuY3Rpb25cbiAgICpcbiAgICogQGRlc2NyaXB0aW9uXG4gICAqIEVuYWJsZXMgYSBzdWJzZXQgb2Ygc3ZnIHRvIGJlIHN1cHBvcnRlZCBieSB0aGUgc2FuaXRpemVyLlxuICAgKlxuICAgKiA8ZGl2IGNsYXNzPVwiYWxlcnQgYWxlcnQtd2FybmluZ1wiPlxuICAgKiAgIDxwPkJ5IGVuYWJsaW5nIHRoaXMgc2V0dGluZyB3aXRob3V0IHRha2luZyBvdGhlciBwcmVjYXV0aW9ucywgeW91IG1pZ2h0IGV4cG9zZSB5b3VyXG4gICAqICAgYXBwbGljYXRpb24gdG8gY2xpY2staGlqYWNraW5nIGF0dGFja3MuIEluIHRoZXNlIGF0dGFja3MsIHNhbml0aXplZCBzdmcgZWxlbWVudHMgY291bGQgYmUgcG9zaXRpb25lZFxuICAgKiAgIG91dHNpZGUgb2YgdGhlIGNvbnRhaW5pbmcgZWxlbWVudCBhbmQgYmUgcmVuZGVyZWQgb3ZlciBvdGhlciBlbGVtZW50cyBvbiB0aGUgcGFnZSAoZS5nLiBhIGxvZ2luXG4gICAqICAgbGluaykuIFN1Y2ggYmVoYXZpb3IgY2FuIHRoZW4gcmVzdWx0IGluIHBoaXNoaW5nIGluY2lkZW50cy48L3A+XG4gICAqXG4gICAqICAgPHA+VG8gcHJvdGVjdCBhZ2FpbnN0IHRoZXNlLCBleHBsaWNpdGx5IHNldHVwIGBvdmVyZmxvdzogaGlkZGVuYCBjc3MgcnVsZSBmb3IgYWxsIHBvdGVudGlhbCBzdmdcbiAgICogICB0YWdzIHdpdGhpbiB0aGUgc2FuaXRpemVkIGNvbnRlbnQ6PC9wPlxuICAgKlxuICAgKiAgIDxicj5cbiAgICpcbiAgICogICA8cHJlPjxjb2RlPlxuICAgKiAgIC5yb290T2ZUaGVJbmNsdWRlZENvbnRlbnQgc3ZnIHtcbiAgICogICAgIG92ZXJmbG93OiBoaWRkZW4gIWltcG9ydGFudDtcbiAgICogICB9XG4gICAqICAgPC9jb2RlPjwvcHJlPlxuICAgKiA8L2Rpdj5cbiAgICpcbiAgICogQHBhcmFtIHtib29sZWFuPX0gcmVnZXhwIE5ldyByZWdleHAgdG8gd2hpdGVsaXN0IHVybHMgd2l0aC5cbiAgICogQHJldHVybnMge2Jvb2xlYW58bmcuJHNhbml0aXplUHJvdmlkZXJ9IFJldHVybnMgdGhlIGN1cnJlbnRseSBjb25maWd1cmVkIHZhbHVlIGlmIGNhbGxlZFxuICAgKiAgICB3aXRob3V0IGFuIGFyZ3VtZW50IG9yIHNlbGYgZm9yIGNoYWluaW5nIG90aGVyd2lzZS5cbiAgICovXG4gIHRoaXMuZW5hYmxlU3ZnID0gZnVuY3Rpb24oZW5hYmxlU3ZnKSB7XG4gICAgaWYgKGFuZ3VsYXIuaXNEZWZpbmVkKGVuYWJsZVN2ZykpIHtcbiAgICAgIHN2Z0VuYWJsZWQgPSBlbmFibGVTdmc7XG4gICAgICByZXR1cm4gdGhpcztcbiAgICB9IGVsc2Uge1xuICAgICAgcmV0dXJuIHN2Z0VuYWJsZWQ7XG4gICAgfVxuICB9O1xufVxuXG5mdW5jdGlvbiBzYW5pdGl6ZVRleHQoY2hhcnMpIHtcbiAgdmFyIGJ1ZiA9IFtdO1xuICB2YXIgd3JpdGVyID0gaHRtbFNhbml0aXplV3JpdGVyKGJ1ZiwgYW5ndWxhci5ub29wKTtcbiAgd3JpdGVyLmNoYXJzKGNoYXJzKTtcbiAgcmV0dXJuIGJ1Zi5qb2luKCcnKTtcbn1cblxuXG4vLyBSZWd1bGFyIEV4cHJlc3Npb25zIGZvciBwYXJzaW5nIHRhZ3MgYW5kIGF0dHJpYnV0ZXNcbnZhciBTVVJST0dBVEVfUEFJUl9SRUdFWFAgPSAvW1xcdUQ4MDAtXFx1REJGRl1bXFx1REMwMC1cXHVERkZGXS9nLFxuICAvLyBNYXRjaCBldmVyeXRoaW5nIG91dHNpZGUgb2Ygbm9ybWFsIGNoYXJzIGFuZCBcIiAocXVvdGUgY2hhcmFjdGVyKVxuICBOT05fQUxQSEFOVU1FUklDX1JFR0VYUCA9IC8oW15cXCMtfiB8IV0pL2c7XG5cblxuLy8gR29vZCBzb3VyY2Ugb2YgaW5mbyBhYm91dCBlbGVtZW50cyBhbmQgYXR0cmlidXRlc1xuLy8gaHR0cDovL2Rldi53My5vcmcvaHRtbDUvc3BlYy9PdmVydmlldy5odG1sI3NlbWFudGljc1xuLy8gaHR0cDovL3NpbW9uLmh0bWw1Lm9yZy9odG1sLWVsZW1lbnRzXG5cbi8vIFNhZmUgVm9pZCBFbGVtZW50cyAtIEhUTUw1XG4vLyBodHRwOi8vZGV2LnczLm9yZy9odG1sNS9zcGVjL092ZXJ2aWV3Lmh0bWwjdm9pZC1lbGVtZW50c1xudmFyIHZvaWRFbGVtZW50cyA9IHRvTWFwKFwiYXJlYSxicixjb2wsaHIsaW1nLHdiclwiKTtcblxuLy8gRWxlbWVudHMgdGhhdCB5b3UgY2FuLCBpbnRlbnRpb25hbGx5LCBsZWF2ZSBvcGVuIChhbmQgd2hpY2ggY2xvc2UgdGhlbXNlbHZlcylcbi8vIGh0dHA6Ly9kZXYudzMub3JnL2h0bWw1L3NwZWMvT3ZlcnZpZXcuaHRtbCNvcHRpb25hbC10YWdzXG52YXIgb3B0aW9uYWxFbmRUYWdCbG9ja0VsZW1lbnRzID0gdG9NYXAoXCJjb2xncm91cCxkZCxkdCxsaSxwLHRib2R5LHRkLHRmb290LHRoLHRoZWFkLHRyXCIpLFxuICAgIG9wdGlvbmFsRW5kVGFnSW5saW5lRWxlbWVudHMgPSB0b01hcChcInJwLHJ0XCIpLFxuICAgIG9wdGlvbmFsRW5kVGFnRWxlbWVudHMgPSBhbmd1bGFyLmV4dGVuZCh7fSxcbiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgb3B0aW9uYWxFbmRUYWdJbmxpbmVFbGVtZW50cyxcbiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgb3B0aW9uYWxFbmRUYWdCbG9ja0VsZW1lbnRzKTtcblxuLy8gU2FmZSBCbG9jayBFbGVtZW50cyAtIEhUTUw1XG52YXIgYmxvY2tFbGVtZW50cyA9IGFuZ3VsYXIuZXh0ZW5kKHt9LCBvcHRpb25hbEVuZFRhZ0Jsb2NrRWxlbWVudHMsIHRvTWFwKFwiYWRkcmVzcyxhcnRpY2xlLFwiICtcbiAgICAgICAgXCJhc2lkZSxibG9ja3F1b3RlLGNhcHRpb24sY2VudGVyLGRlbCxkaXIsZGl2LGRsLGZpZ3VyZSxmaWdjYXB0aW9uLGZvb3RlcixoMSxoMixoMyxoNCxoNSxcIiArXG4gICAgICAgIFwiaDYsaGVhZGVyLGhncm91cCxocixpbnMsbWFwLG1lbnUsbmF2LG9sLHByZSxzZWN0aW9uLHRhYmxlLHVsXCIpKTtcblxuLy8gSW5saW5lIEVsZW1lbnRzIC0gSFRNTDVcbnZhciBpbmxpbmVFbGVtZW50cyA9IGFuZ3VsYXIuZXh0ZW5kKHt9LCBvcHRpb25hbEVuZFRhZ0lubGluZUVsZW1lbnRzLCB0b01hcChcImEsYWJicixhY3JvbnltLGIsXCIgK1xuICAgICAgICBcImJkaSxiZG8sYmlnLGJyLGNpdGUsY29kZSxkZWwsZGZuLGVtLGZvbnQsaSxpbWcsaW5zLGtiZCxsYWJlbCxtYXAsbWFyayxxLHJ1YnkscnAscnQscyxcIiArXG4gICAgICAgIFwic2FtcCxzbWFsbCxzcGFuLHN0cmlrZSxzdHJvbmcsc3ViLHN1cCx0aW1lLHR0LHUsdmFyXCIpKTtcblxuLy8gU1ZHIEVsZW1lbnRzXG4vLyBodHRwczovL3dpa2kud2hhdHdnLm9yZy93aWtpL1Nhbml0aXphdGlvbl9ydWxlcyNzdmdfRWxlbWVudHNcbi8vIE5vdGU6IHRoZSBlbGVtZW50cyBhbmltYXRlLGFuaW1hdGVDb2xvcixhbmltYXRlTW90aW9uLGFuaW1hdGVUcmFuc2Zvcm0sc2V0IGFyZSBpbnRlbnRpb25hbGx5IG9taXR0ZWQuXG4vLyBUaGV5IGNhbiBwb3RlbnRpYWxseSBhbGxvdyBmb3IgYXJiaXRyYXJ5IGphdmFzY3JpcHQgdG8gYmUgZXhlY3V0ZWQuIFNlZSAjMTEyOTBcbnZhciBzdmdFbGVtZW50cyA9IHRvTWFwKFwiY2lyY2xlLGRlZnMsZGVzYyxlbGxpcHNlLGZvbnQtZmFjZSxmb250LWZhY2UtbmFtZSxmb250LWZhY2Utc3JjLGcsZ2x5cGgsXCIgK1xuICAgICAgICBcImhrZXJuLGltYWdlLGxpbmVhckdyYWRpZW50LGxpbmUsbWFya2VyLG1ldGFkYXRhLG1pc3NpbmctZ2x5cGgsbXBhdGgscGF0aCxwb2x5Z29uLHBvbHlsaW5lLFwiICtcbiAgICAgICAgXCJyYWRpYWxHcmFkaWVudCxyZWN0LHN0b3Asc3ZnLHN3aXRjaCx0ZXh0LHRpdGxlLHRzcGFuXCIpO1xuXG4vLyBCbG9ja2VkIEVsZW1lbnRzICh3aWxsIGJlIHN0cmlwcGVkKVxudmFyIGJsb2NrZWRFbGVtZW50cyA9IHRvTWFwKFwic2NyaXB0LHN0eWxlXCIpO1xuXG52YXIgdmFsaWRFbGVtZW50cyA9IGFuZ3VsYXIuZXh0ZW5kKHt9LFxuICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICB2b2lkRWxlbWVudHMsXG4gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIGJsb2NrRWxlbWVudHMsXG4gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIGlubGluZUVsZW1lbnRzLFxuICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICBvcHRpb25hbEVuZFRhZ0VsZW1lbnRzKTtcblxuLy9BdHRyaWJ1dGVzIHRoYXQgaGF2ZSBocmVmIGFuZCBoZW5jZSBuZWVkIHRvIGJlIHNhbml0aXplZFxudmFyIHVyaUF0dHJzID0gdG9NYXAoXCJiYWNrZ3JvdW5kLGNpdGUsaHJlZixsb25nZGVzYyxzcmMseGxpbms6aHJlZlwiKTtcblxudmFyIGh0bWxBdHRycyA9IHRvTWFwKCdhYmJyLGFsaWduLGFsdCxheGlzLGJnY29sb3IsYm9yZGVyLGNlbGxwYWRkaW5nLGNlbGxzcGFjaW5nLGNsYXNzLGNsZWFyLCcgK1xuICAgICdjb2xvcixjb2xzLGNvbHNwYW4sY29tcGFjdCxjb29yZHMsZGlyLGZhY2UsaGVhZGVycyxoZWlnaHQsaHJlZmxhbmcsaHNwYWNlLCcgK1xuICAgICdpc21hcCxsYW5nLGxhbmd1YWdlLG5vaHJlZixub3dyYXAscmVsLHJldixyb3dzLHJvd3NwYW4scnVsZXMsJyArXG4gICAgJ3Njb3BlLHNjcm9sbGluZyxzaGFwZSxzaXplLHNwYW4sc3RhcnQsc3VtbWFyeSx0YWJpbmRleCx0YXJnZXQsdGl0bGUsdHlwZSwnICtcbiAgICAndmFsaWduLHZhbHVlLHZzcGFjZSx3aWR0aCcpO1xuXG4vLyBTVkcgYXR0cmlidXRlcyAod2l0aG91dCBcImlkXCIgYW5kIFwibmFtZVwiIGF0dHJpYnV0ZXMpXG4vLyBodHRwczovL3dpa2kud2hhdHdnLm9yZy93aWtpL1Nhbml0aXphdGlvbl9ydWxlcyNzdmdfQXR0cmlidXRlc1xudmFyIHN2Z0F0dHJzID0gdG9NYXAoJ2FjY2VudC1oZWlnaHQsYWNjdW11bGF0ZSxhZGRpdGl2ZSxhbHBoYWJldGljLGFyYWJpYy1mb3JtLGFzY2VudCwnICtcbiAgICAnYmFzZVByb2ZpbGUsYmJveCxiZWdpbixieSxjYWxjTW9kZSxjYXAtaGVpZ2h0LGNsYXNzLGNvbG9yLGNvbG9yLXJlbmRlcmluZyxjb250ZW50LCcgK1xuICAgICdjeCxjeSxkLGR4LGR5LGRlc2NlbnQsZGlzcGxheSxkdXIsZW5kLGZpbGwsZmlsbC1ydWxlLGZvbnQtZmFtaWx5LGZvbnQtc2l6ZSxmb250LXN0cmV0Y2gsJyArXG4gICAgJ2ZvbnQtc3R5bGUsZm9udC12YXJpYW50LGZvbnQtd2VpZ2h0LGZyb20sZngsZnksZzEsZzIsZ2x5cGgtbmFtZSxncmFkaWVudFVuaXRzLGhhbmdpbmcsJyArXG4gICAgJ2hlaWdodCxob3Jpei1hZHYteCxob3Jpei1vcmlnaW4teCxpZGVvZ3JhcGhpYyxrLGtleVBvaW50cyxrZXlTcGxpbmVzLGtleVRpbWVzLGxhbmcsJyArXG4gICAgJ21hcmtlci1lbmQsbWFya2VyLW1pZCxtYXJrZXItc3RhcnQsbWFya2VySGVpZ2h0LG1hcmtlclVuaXRzLG1hcmtlcldpZHRoLG1hdGhlbWF0aWNhbCwnICtcbiAgICAnbWF4LG1pbixvZmZzZXQsb3BhY2l0eSxvcmllbnQsb3JpZ2luLG92ZXJsaW5lLXBvc2l0aW9uLG92ZXJsaW5lLXRoaWNrbmVzcyxwYW5vc2UtMSwnICtcbiAgICAncGF0aCxwYXRoTGVuZ3RoLHBvaW50cyxwcmVzZXJ2ZUFzcGVjdFJhdGlvLHIscmVmWCxyZWZZLHJlcGVhdENvdW50LHJlcGVhdER1ciwnICtcbiAgICAncmVxdWlyZWRFeHRlbnNpb25zLHJlcXVpcmVkRmVhdHVyZXMscmVzdGFydCxyb3RhdGUscngscnksc2xvcGUsc3RlbWgsc3RlbXYsc3RvcC1jb2xvciwnICtcbiAgICAnc3RvcC1vcGFjaXR5LHN0cmlrZXRocm91Z2gtcG9zaXRpb24sc3RyaWtldGhyb3VnaC10aGlja25lc3Msc3Ryb2tlLHN0cm9rZS1kYXNoYXJyYXksJyArXG4gICAgJ3N0cm9rZS1kYXNob2Zmc2V0LHN0cm9rZS1saW5lY2FwLHN0cm9rZS1saW5lam9pbixzdHJva2UtbWl0ZXJsaW1pdCxzdHJva2Utb3BhY2l0eSwnICtcbiAgICAnc3Ryb2tlLXdpZHRoLHN5c3RlbUxhbmd1YWdlLHRhcmdldCx0ZXh0LWFuY2hvcix0byx0cmFuc2Zvcm0sdHlwZSx1MSx1Mix1bmRlcmxpbmUtcG9zaXRpb24sJyArXG4gICAgJ3VuZGVybGluZS10aGlja25lc3MsdW5pY29kZSx1bmljb2RlLXJhbmdlLHVuaXRzLXBlci1lbSx2YWx1ZXMsdmVyc2lvbix2aWV3Qm94LHZpc2liaWxpdHksJyArXG4gICAgJ3dpZHRoLHdpZHRocyx4LHgtaGVpZ2h0LHgxLHgyLHhsaW5rOmFjdHVhdGUseGxpbms6YXJjcm9sZSx4bGluazpyb2xlLHhsaW5rOnNob3cseGxpbms6dGl0bGUsJyArXG4gICAgJ3hsaW5rOnR5cGUseG1sOmJhc2UseG1sOmxhbmcseG1sOnNwYWNlLHhtbG5zLHhtbG5zOnhsaW5rLHkseTEseTIsem9vbUFuZFBhbicsIHRydWUpO1xuXG52YXIgdmFsaWRBdHRycyA9IGFuZ3VsYXIuZXh0ZW5kKHt9LFxuICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICB1cmlBdHRycyxcbiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgc3ZnQXR0cnMsXG4gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIGh0bWxBdHRycyk7XG5cbmZ1bmN0aW9uIHRvTWFwKHN0ciwgbG93ZXJjYXNlS2V5cykge1xuICB2YXIgb2JqID0ge30sIGl0ZW1zID0gc3RyLnNwbGl0KCcsJyksIGk7XG4gIGZvciAoaSA9IDA7IGkgPCBpdGVtcy5sZW5ndGg7IGkrKykge1xuICAgIG9ialtsb3dlcmNhc2VLZXlzID8gYW5ndWxhci5sb3dlcmNhc2UoaXRlbXNbaV0pIDogaXRlbXNbaV1dID0gdHJ1ZTtcbiAgfVxuICByZXR1cm4gb2JqO1xufVxuXG52YXIgaW5lcnRCb2R5RWxlbWVudDtcbihmdW5jdGlvbih3aW5kb3cpIHtcbiAgdmFyIGRvYztcbiAgaWYgKHdpbmRvdy5kb2N1bWVudCAmJiB3aW5kb3cuZG9jdW1lbnQuaW1wbGVtZW50YXRpb24pIHtcbiAgICBkb2MgPSB3aW5kb3cuZG9jdW1lbnQuaW1wbGVtZW50YXRpb24uY3JlYXRlSFRNTERvY3VtZW50KFwiaW5lcnRcIik7XG4gIH0gZWxzZSB7XG4gICAgdGhyb3cgJHNhbml0aXplTWluRXJyKCdub2luZXJ0JywgXCJDYW4ndCBjcmVhdGUgYW4gaW5lcnQgaHRtbCBkb2N1bWVudFwiKTtcbiAgfVxuICB2YXIgZG9jRWxlbWVudCA9IGRvYy5kb2N1bWVudEVsZW1lbnQgfHwgZG9jLmdldERvY3VtZW50RWxlbWVudCgpO1xuICB2YXIgYm9keUVsZW1lbnRzID0gZG9jRWxlbWVudC5nZXRFbGVtZW50c0J5VGFnTmFtZSgnYm9keScpO1xuXG4gIC8vIHVzdWFsbHkgdGhlcmUgc2hvdWxkIGJlIG9ubHkgb25lIGJvZHkgZWxlbWVudCBpbiB0aGUgZG9jdW1lbnQsIGJ1dCBJRSBkb2Vzbid0IGhhdmUgYW55LCBzbyB3ZSBuZWVkIHRvIGNyZWF0ZSBvbmVcbiAgaWYgKGJvZHlFbGVtZW50cy5sZW5ndGggPT09IDEpIHtcbiAgICBpbmVydEJvZHlFbGVtZW50ID0gYm9keUVsZW1lbnRzWzBdO1xuICB9IGVsc2Uge1xuICAgIHZhciBodG1sID0gZG9jLmNyZWF0ZUVsZW1lbnQoJ2h0bWwnKTtcbiAgICBpbmVydEJvZHlFbGVtZW50ID0gZG9jLmNyZWF0ZUVsZW1lbnQoJ2JvZHknKTtcbiAgICBodG1sLmFwcGVuZENoaWxkKGluZXJ0Qm9keUVsZW1lbnQpO1xuICAgIGRvYy5hcHBlbmRDaGlsZChodG1sKTtcbiAgfVxufSkod2luZG93KTtcblxuLyoqXG4gKiBAZXhhbXBsZVxuICogaHRtbFBhcnNlcihodG1sU3RyaW5nLCB7XG4gKiAgICAgc3RhcnQ6IGZ1bmN0aW9uKHRhZywgYXR0cnMpIHt9LFxuICogICAgIGVuZDogZnVuY3Rpb24odGFnKSB7fSxcbiAqICAgICBjaGFyczogZnVuY3Rpb24odGV4dCkge30sXG4gKiAgICAgY29tbWVudDogZnVuY3Rpb24odGV4dCkge31cbiAqIH0pO1xuICpcbiAqIEBwYXJhbSB7c3RyaW5nfSBodG1sIHN0cmluZ1xuICogQHBhcmFtIHtvYmplY3R9IGhhbmRsZXJcbiAqL1xuZnVuY3Rpb24gaHRtbFBhcnNlcihodG1sLCBoYW5kbGVyKSB7XG4gIGlmIChodG1sID09PSBudWxsIHx8IGh0bWwgPT09IHVuZGVmaW5lZCkge1xuICAgIGh0bWwgPSAnJztcbiAgfSBlbHNlIGlmICh0eXBlb2YgaHRtbCAhPT0gJ3N0cmluZycpIHtcbiAgICBodG1sID0gJycgKyBodG1sO1xuICB9XG4gIGluZXJ0Qm9keUVsZW1lbnQuaW5uZXJIVE1MID0gaHRtbDtcblxuICAvL21YU1MgcHJvdGVjdGlvblxuICB2YXIgbVhTU0F0dGVtcHRzID0gNTtcbiAgZG8ge1xuICAgIGlmIChtWFNTQXR0ZW1wdHMgPT09IDApIHtcbiAgICAgIHRocm93ICRzYW5pdGl6ZU1pbkVycigndWlucHV0JywgXCJGYWlsZWQgdG8gc2FuaXRpemUgaHRtbCBiZWNhdXNlIHRoZSBpbnB1dCBpcyB1bnN0YWJsZVwiKTtcbiAgICB9XG4gICAgbVhTU0F0dGVtcHRzLS07XG5cbiAgICAvLyBzdHJpcCBjdXN0b20tbmFtZXNwYWNlZCBhdHRyaWJ1dGVzIG9uIElFPD0xMVxuICAgIGlmIChkb2N1bWVudC5kb2N1bWVudE1vZGUgPD0gMTEpIHtcbiAgICAgIHN0cmlwQ3VzdG9tTnNBdHRycyhpbmVydEJvZHlFbGVtZW50KTtcbiAgICB9XG4gICAgaHRtbCA9IGluZXJ0Qm9keUVsZW1lbnQuaW5uZXJIVE1MOyAvL3RyaWdnZXIgbVhTU1xuICAgIGluZXJ0Qm9keUVsZW1lbnQuaW5uZXJIVE1MID0gaHRtbDtcbiAgfSB3aGlsZSAoaHRtbCAhPT0gaW5lcnRCb2R5RWxlbWVudC5pbm5lckhUTUwpO1xuXG4gIHZhciBub2RlID0gaW5lcnRCb2R5RWxlbWVudC5maXJzdENoaWxkO1xuICB3aGlsZSAobm9kZSkge1xuICAgIHN3aXRjaCAobm9kZS5ub2RlVHlwZSkge1xuICAgICAgY2FzZSAxOiAvLyBFTEVNRU5UX05PREVcbiAgICAgICAgaGFuZGxlci5zdGFydChub2RlLm5vZGVOYW1lLnRvTG93ZXJDYXNlKCksIGF0dHJUb01hcChub2RlLmF0dHJpYnV0ZXMpKTtcbiAgICAgICAgYnJlYWs7XG4gICAgICBjYXNlIDM6IC8vIFRFWFQgTk9ERVxuICAgICAgICBoYW5kbGVyLmNoYXJzKG5vZGUudGV4dENvbnRlbnQpO1xuICAgICAgICBicmVhaztcbiAgICB9XG5cbiAgICB2YXIgbmV4dE5vZGU7XG4gICAgaWYgKCEobmV4dE5vZGUgPSBub2RlLmZpcnN0Q2hpbGQpKSB7XG4gICAgICBpZiAobm9kZS5ub2RlVHlwZSA9PSAxKSB7XG4gICAgICAgIGhhbmRsZXIuZW5kKG5vZGUubm9kZU5hbWUudG9Mb3dlckNhc2UoKSk7XG4gICAgICB9XG4gICAgICBuZXh0Tm9kZSA9IG5vZGUubmV4dFNpYmxpbmc7XG4gICAgICBpZiAoIW5leHROb2RlKSB7XG4gICAgICAgIHdoaWxlIChuZXh0Tm9kZSA9PSBudWxsKSB7XG4gICAgICAgICAgbm9kZSA9IG5vZGUucGFyZW50Tm9kZTtcbiAgICAgICAgICBpZiAobm9kZSA9PT0gaW5lcnRCb2R5RWxlbWVudCkgYnJlYWs7XG4gICAgICAgICAgbmV4dE5vZGUgPSBub2RlLm5leHRTaWJsaW5nO1xuICAgICAgICAgIGlmIChub2RlLm5vZGVUeXBlID09IDEpIHtcbiAgICAgICAgICAgIGhhbmRsZXIuZW5kKG5vZGUubm9kZU5hbWUudG9Mb3dlckNhc2UoKSk7XG4gICAgICAgICAgfVxuICAgICAgICB9XG4gICAgICB9XG4gICAgfVxuICAgIG5vZGUgPSBuZXh0Tm9kZTtcbiAgfVxuXG4gIHdoaWxlIChub2RlID0gaW5lcnRCb2R5RWxlbWVudC5maXJzdENoaWxkKSB7XG4gICAgaW5lcnRCb2R5RWxlbWVudC5yZW1vdmVDaGlsZChub2RlKTtcbiAgfVxufVxuXG5mdW5jdGlvbiBhdHRyVG9NYXAoYXR0cnMpIHtcbiAgdmFyIG1hcCA9IHt9O1xuICBmb3IgKHZhciBpID0gMCwgaWkgPSBhdHRycy5sZW5ndGg7IGkgPCBpaTsgaSsrKSB7XG4gICAgdmFyIGF0dHIgPSBhdHRyc1tpXTtcbiAgICBtYXBbYXR0ci5uYW1lXSA9IGF0dHIudmFsdWU7XG4gIH1cbiAgcmV0dXJuIG1hcDtcbn1cblxuXG4vKipcbiAqIEVzY2FwZXMgYWxsIHBvdGVudGlhbGx5IGRhbmdlcm91cyBjaGFyYWN0ZXJzLCBzbyB0aGF0IHRoZVxuICogcmVzdWx0aW5nIHN0cmluZyBjYW4gYmUgc2FmZWx5IGluc2VydGVkIGludG8gYXR0cmlidXRlIG9yXG4gKiBlbGVtZW50IHRleHQuXG4gKiBAcGFyYW0gdmFsdWVcbiAqIEByZXR1cm5zIHtzdHJpbmd9IGVzY2FwZWQgdGV4dFxuICovXG5mdW5jdGlvbiBlbmNvZGVFbnRpdGllcyh2YWx1ZSkge1xuICByZXR1cm4gdmFsdWUuXG4gICAgcmVwbGFjZSgvJi9nLCAnJmFtcDsnKS5cbiAgICByZXBsYWNlKFNVUlJPR0FURV9QQUlSX1JFR0VYUCwgZnVuY3Rpb24odmFsdWUpIHtcbiAgICAgIHZhciBoaSA9IHZhbHVlLmNoYXJDb2RlQXQoMCk7XG4gICAgICB2YXIgbG93ID0gdmFsdWUuY2hhckNvZGVBdCgxKTtcbiAgICAgIHJldHVybiAnJiMnICsgKCgoaGkgLSAweEQ4MDApICogMHg0MDApICsgKGxvdyAtIDB4REMwMCkgKyAweDEwMDAwKSArICc7JztcbiAgICB9KS5cbiAgICByZXBsYWNlKE5PTl9BTFBIQU5VTUVSSUNfUkVHRVhQLCBmdW5jdGlvbih2YWx1ZSkge1xuICAgICAgcmV0dXJuICcmIycgKyB2YWx1ZS5jaGFyQ29kZUF0KDApICsgJzsnO1xuICAgIH0pLlxuICAgIHJlcGxhY2UoLzwvZywgJyZsdDsnKS5cbiAgICByZXBsYWNlKC8+L2csICcmZ3Q7Jyk7XG59XG5cbi8qKlxuICogY3JlYXRlIGFuIEhUTUwvWE1MIHdyaXRlciB3aGljaCB3cml0ZXMgdG8gYnVmZmVyXG4gKiBAcGFyYW0ge0FycmF5fSBidWYgdXNlIGJ1Zi5qb2luKCcnKSB0byBnZXQgb3V0IHNhbml0aXplZCBodG1sIHN0cmluZ1xuICogQHJldHVybnMge29iamVjdH0gaW4gdGhlIGZvcm0gb2Yge1xuICogICAgIHN0YXJ0OiBmdW5jdGlvbih0YWcsIGF0dHJzKSB7fSxcbiAqICAgICBlbmQ6IGZ1bmN0aW9uKHRhZykge30sXG4gKiAgICAgY2hhcnM6IGZ1bmN0aW9uKHRleHQpIHt9LFxuICogICAgIGNvbW1lbnQ6IGZ1bmN0aW9uKHRleHQpIHt9XG4gKiB9XG4gKi9cbmZ1bmN0aW9uIGh0bWxTYW5pdGl6ZVdyaXRlcihidWYsIHVyaVZhbGlkYXRvcikge1xuICB2YXIgaWdub3JlQ3VycmVudEVsZW1lbnQgPSBmYWxzZTtcbiAgdmFyIG91dCA9IGFuZ3VsYXIuYmluZChidWYsIGJ1Zi5wdXNoKTtcbiAgcmV0dXJuIHtcbiAgICBzdGFydDogZnVuY3Rpb24odGFnLCBhdHRycykge1xuICAgICAgdGFnID0gYW5ndWxhci5sb3dlcmNhc2UodGFnKTtcbiAgICAgIGlmICghaWdub3JlQ3VycmVudEVsZW1lbnQgJiYgYmxvY2tlZEVsZW1lbnRzW3RhZ10pIHtcbiAgICAgICAgaWdub3JlQ3VycmVudEVsZW1lbnQgPSB0YWc7XG4gICAgICB9XG4gICAgICBpZiAoIWlnbm9yZUN1cnJlbnRFbGVtZW50ICYmIHZhbGlkRWxlbWVudHNbdGFnXSA9PT0gdHJ1ZSkge1xuICAgICAgICBvdXQoJzwnKTtcbiAgICAgICAgb3V0KHRhZyk7XG4gICAgICAgIGFuZ3VsYXIuZm9yRWFjaChhdHRycywgZnVuY3Rpb24odmFsdWUsIGtleSkge1xuICAgICAgICAgIHZhciBsa2V5PWFuZ3VsYXIubG93ZXJjYXNlKGtleSk7XG4gICAgICAgICAgdmFyIGlzSW1hZ2UgPSAodGFnID09PSAnaW1nJyAmJiBsa2V5ID09PSAnc3JjJykgfHwgKGxrZXkgPT09ICdiYWNrZ3JvdW5kJyk7XG4gICAgICAgICAgaWYgKHZhbGlkQXR0cnNbbGtleV0gPT09IHRydWUgJiZcbiAgICAgICAgICAgICh1cmlBdHRyc1tsa2V5XSAhPT0gdHJ1ZSB8fCB1cmlWYWxpZGF0b3IodmFsdWUsIGlzSW1hZ2UpKSkge1xuICAgICAgICAgICAgb3V0KCcgJyk7XG4gICAgICAgICAgICBvdXQoa2V5KTtcbiAgICAgICAgICAgIG91dCgnPVwiJyk7XG4gICAgICAgICAgICBvdXQoZW5jb2RlRW50aXRpZXModmFsdWUpKTtcbiAgICAgICAgICAgIG91dCgnXCInKTtcbiAgICAgICAgICB9XG4gICAgICAgIH0pO1xuICAgICAgICBvdXQoJz4nKTtcbiAgICAgIH1cbiAgICB9LFxuICAgIGVuZDogZnVuY3Rpb24odGFnKSB7XG4gICAgICB0YWcgPSBhbmd1bGFyLmxvd2VyY2FzZSh0YWcpO1xuICAgICAgaWYgKCFpZ25vcmVDdXJyZW50RWxlbWVudCAmJiB2YWxpZEVsZW1lbnRzW3RhZ10gPT09IHRydWUgJiYgdm9pZEVsZW1lbnRzW3RhZ10gIT09IHRydWUpIHtcbiAgICAgICAgb3V0KCc8LycpO1xuICAgICAgICBvdXQodGFnKTtcbiAgICAgICAgb3V0KCc+Jyk7XG4gICAgICB9XG4gICAgICBpZiAodGFnID09IGlnbm9yZUN1cnJlbnRFbGVtZW50KSB7XG4gICAgICAgIGlnbm9yZUN1cnJlbnRFbGVtZW50ID0gZmFsc2U7XG4gICAgICB9XG4gICAgfSxcbiAgICBjaGFyczogZnVuY3Rpb24oY2hhcnMpIHtcbiAgICAgIGlmICghaWdub3JlQ3VycmVudEVsZW1lbnQpIHtcbiAgICAgICAgb3V0KGVuY29kZUVudGl0aWVzKGNoYXJzKSk7XG4gICAgICB9XG4gICAgfVxuICB9O1xufVxuXG5cbi8qKlxuICogV2hlbiBJRTktMTEgY29tZXMgYWNyb3NzIGFuIHVua25vd24gbmFtZXNwYWNlZCBhdHRyaWJ1dGUgZS5nLiAneGxpbms6Zm9vJyBpdCBhZGRzICd4bWxuczpuczEnIGF0dHJpYnV0ZSB0byBkZWNsYXJlXG4gKiBuczEgbmFtZXNwYWNlIGFuZCBwcmVmaXhlcyB0aGUgYXR0cmlidXRlIHdpdGggJ25zMScgKGUuZy4gJ25zMTp4bGluazpmb28nKS4gVGhpcyBpcyB1bmRlc2lyYWJsZSBzaW5jZSB3ZSBkb24ndCB3YW50XG4gKiB0byBhbGxvdyBhbnkgb2YgdGhlc2UgY3VzdG9tIGF0dHJpYnV0ZXMuIFRoaXMgbWV0aG9kIHN0cmlwcyB0aGVtIGFsbC5cbiAqXG4gKiBAcGFyYW0gbm9kZSBSb290IGVsZW1lbnQgdG8gcHJvY2Vzc1xuICovXG5mdW5jdGlvbiBzdHJpcEN1c3RvbU5zQXR0cnMobm9kZSkge1xuICBpZiAobm9kZS5ub2RlVHlwZSA9PT0gTm9kZS5FTEVNRU5UX05PREUpIHtcbiAgICB2YXIgYXR0cnMgPSBub2RlLmF0dHJpYnV0ZXM7XG4gICAgZm9yICh2YXIgaSA9IDAsIGwgPSBhdHRycy5sZW5ndGg7IGkgPCBsOyBpKyspIHtcbiAgICAgIHZhciBhdHRyTm9kZSA9IGF0dHJzW2ldO1xuICAgICAgdmFyIGF0dHJOYW1lID0gYXR0ck5vZGUubmFtZS50b0xvd2VyQ2FzZSgpO1xuICAgICAgaWYgKGF0dHJOYW1lID09PSAneG1sbnM6bnMxJyB8fCBhdHRyTmFtZS5pbmRleE9mKCduczE6JykgPT09IDApIHtcbiAgICAgICAgbm9kZS5yZW1vdmVBdHRyaWJ1dGVOb2RlKGF0dHJOb2RlKTtcbiAgICAgICAgaS0tO1xuICAgICAgICBsLS07XG4gICAgICB9XG4gICAgfVxuICB9XG5cbiAgdmFyIG5leHROb2RlID0gbm9kZS5maXJzdENoaWxkO1xuICBpZiAobmV4dE5vZGUpIHtcbiAgICBzdHJpcEN1c3RvbU5zQXR0cnMobmV4dE5vZGUpO1xuICB9XG5cbiAgbmV4dE5vZGUgPSBub2RlLm5leHRTaWJsaW5nO1xuICBpZiAobmV4dE5vZGUpIHtcbiAgICBzdHJpcEN1c3RvbU5zQXR0cnMobmV4dE5vZGUpO1xuICB9XG59XG5cblxuXG4vLyBkZWZpbmUgbmdTYW5pdGl6ZSBtb2R1bGUgYW5kIHJlZ2lzdGVyICRzYW5pdGl6ZSBzZXJ2aWNlXG5hbmd1bGFyLm1vZHVsZSgnbmdTYW5pdGl6ZScsIFtdKS5wcm92aWRlcignJHNhbml0aXplJywgJFNhbml0aXplUHJvdmlkZXIpO1xuXG4vKiBnbG9iYWwgc2FuaXRpemVUZXh0OiBmYWxzZSAqL1xuXG4vKipcbiAqIEBuZ2RvYyBmaWx0ZXJcbiAqIEBuYW1lIGxpbmt5XG4gKiBAa2luZCBmdW5jdGlvblxuICpcbiAqIEBkZXNjcmlwdGlvblxuICogRmluZHMgbGlua3MgaW4gdGV4dCBpbnB1dCBhbmQgdHVybnMgdGhlbSBpbnRvIGh0bWwgbGlua3MuIFN1cHBvcnRzIGBodHRwL2h0dHBzL2Z0cC9tYWlsdG9gIGFuZFxuICogcGxhaW4gZW1haWwgYWRkcmVzcyBsaW5rcy5cbiAqXG4gKiBSZXF1aXJlcyB0aGUge0BsaW5rIG5nU2FuaXRpemUgYG5nU2FuaXRpemVgfSBtb2R1bGUgdG8gYmUgaW5zdGFsbGVkLlxuICpcbiAqIEBwYXJhbSB7c3RyaW5nfSB0ZXh0IElucHV0IHRleHQuXG4gKiBAcGFyYW0ge3N0cmluZ30gdGFyZ2V0IFdpbmRvdyAoYF9ibGFua3xfc2VsZnxfcGFyZW50fF90b3BgKSBvciBuYW1lZCBmcmFtZSB0byBvcGVuIGxpbmtzIGluLlxuICogQHBhcmFtIHtvYmplY3R8ZnVuY3Rpb24odXJsKX0gW2F0dHJpYnV0ZXNdIEFkZCBjdXN0b20gYXR0cmlidXRlcyB0byB0aGUgbGluayBlbGVtZW50LlxuICpcbiAqICAgIENhbiBiZSBvbmUgb2Y6XG4gKlxuICogICAgLSBgb2JqZWN0YDogQSBtYXAgb2YgYXR0cmlidXRlc1xuICogICAgLSBgZnVuY3Rpb25gOiBUYWtlcyB0aGUgdXJsIGFzIGEgcGFyYW1ldGVyIGFuZCByZXR1cm5zIGEgbWFwIG9mIGF0dHJpYnV0ZXNcbiAqXG4gKiAgICBJZiB0aGUgbWFwIG9mIGF0dHJpYnV0ZXMgY29udGFpbnMgYSB2YWx1ZSBmb3IgYHRhcmdldGAsIGl0IG92ZXJyaWRlcyB0aGUgdmFsdWUgb2ZcbiAqICAgIHRoZSB0YXJnZXQgcGFyYW1ldGVyLlxuICpcbiAqXG4gKiBAcmV0dXJucyB7c3RyaW5nfSBIdG1sLWxpbmtpZmllZCBhbmQge0BsaW5rICRzYW5pdGl6ZSBzYW5pdGl6ZWR9IHRleHQuXG4gKlxuICogQHVzYWdlXG4gICA8c3BhbiBuZy1iaW5kLWh0bWw9XCJsaW5reV9leHByZXNzaW9uIHwgbGlua3lcIj48L3NwYW4+XG4gKlxuICogQGV4YW1wbGVcbiAgIDxleGFtcGxlIG1vZHVsZT1cImxpbmt5RXhhbXBsZVwiIGRlcHM9XCJhbmd1bGFyLXNhbml0aXplLmpzXCI+XG4gICAgIDxmaWxlIG5hbWU9XCJpbmRleC5odG1sXCI+XG4gICAgICAgPGRpdiBuZy1jb250cm9sbGVyPVwiRXhhbXBsZUNvbnRyb2xsZXJcIj5cbiAgICAgICBTbmlwcGV0OiA8dGV4dGFyZWEgbmctbW9kZWw9XCJzbmlwcGV0XCIgY29scz1cIjYwXCIgcm93cz1cIjNcIj48L3RleHRhcmVhPlxuICAgICAgIDx0YWJsZT5cbiAgICAgICAgIDx0cj5cbiAgICAgICAgICAgPHRoPkZpbHRlcjwvdGg+XG4gICAgICAgICAgIDx0aD5Tb3VyY2U8L3RoPlxuICAgICAgICAgICA8dGg+UmVuZGVyZWQ8L3RoPlxuICAgICAgICAgPC90cj5cbiAgICAgICAgIDx0ciBpZD1cImxpbmt5LWZpbHRlclwiPlxuICAgICAgICAgICA8dGQ+bGlua3kgZmlsdGVyPC90ZD5cbiAgICAgICAgICAgPHRkPlxuICAgICAgICAgICAgIDxwcmU+Jmx0O2RpdiBuZy1iaW5kLWh0bWw9XCJzbmlwcGV0IHwgbGlua3lcIiZndDs8YnI+Jmx0Oy9kaXYmZ3Q7PC9wcmU+XG4gICAgICAgICAgIDwvdGQ+XG4gICAgICAgICAgIDx0ZD5cbiAgICAgICAgICAgICA8ZGl2IG5nLWJpbmQtaHRtbD1cInNuaXBwZXQgfCBsaW5reVwiPjwvZGl2PlxuICAgICAgICAgICA8L3RkPlxuICAgICAgICAgPC90cj5cbiAgICAgICAgIDx0ciBpZD1cImxpbmt5LXRhcmdldFwiPlxuICAgICAgICAgIDx0ZD5saW5reSB0YXJnZXQ8L3RkPlxuICAgICAgICAgIDx0ZD5cbiAgICAgICAgICAgIDxwcmU+Jmx0O2RpdiBuZy1iaW5kLWh0bWw9XCJzbmlwcGV0V2l0aFNpbmdsZVVSTCB8IGxpbmt5OidfYmxhbmsnXCImZ3Q7PGJyPiZsdDsvZGl2Jmd0OzwvcHJlPlxuICAgICAgICAgIDwvdGQ+XG4gICAgICAgICAgPHRkPlxuICAgICAgICAgICAgPGRpdiBuZy1iaW5kLWh0bWw9XCJzbmlwcGV0V2l0aFNpbmdsZVVSTCB8IGxpbmt5OidfYmxhbmsnXCI+PC9kaXY+XG4gICAgICAgICAgPC90ZD5cbiAgICAgICAgIDwvdHI+XG4gICAgICAgICA8dHIgaWQ9XCJsaW5reS1jdXN0b20tYXR0cmlidXRlc1wiPlxuICAgICAgICAgIDx0ZD5saW5reSBjdXN0b20gYXR0cmlidXRlczwvdGQ+XG4gICAgICAgICAgPHRkPlxuICAgICAgICAgICAgPHByZT4mbHQ7ZGl2IG5nLWJpbmQtaHRtbD1cInNuaXBwZXRXaXRoU2luZ2xlVVJMIHwgbGlua3k6J19zZWxmJzp7cmVsOiAnbm9mb2xsb3cnfVwiJmd0Ozxicj4mbHQ7L2RpdiZndDs8L3ByZT5cbiAgICAgICAgICA8L3RkPlxuICAgICAgICAgIDx0ZD5cbiAgICAgICAgICAgIDxkaXYgbmctYmluZC1odG1sPVwic25pcHBldFdpdGhTaW5nbGVVUkwgfCBsaW5reTonX3NlbGYnOntyZWw6ICdub2ZvbGxvdyd9XCI+PC9kaXY+XG4gICAgICAgICAgPC90ZD5cbiAgICAgICAgIDwvdHI+XG4gICAgICAgICA8dHIgaWQ9XCJlc2NhcGVkLWh0bWxcIj5cbiAgICAgICAgICAgPHRkPm5vIGZpbHRlcjwvdGQ+XG4gICAgICAgICAgIDx0ZD48cHJlPiZsdDtkaXYgbmctYmluZD1cInNuaXBwZXRcIiZndDs8YnI+Jmx0Oy9kaXYmZ3Q7PC9wcmU+PC90ZD5cbiAgICAgICAgICAgPHRkPjxkaXYgbmctYmluZD1cInNuaXBwZXRcIj48L2Rpdj48L3RkPlxuICAgICAgICAgPC90cj5cbiAgICAgICA8L3RhYmxlPlxuICAgICA8L2ZpbGU+XG4gICAgIDxmaWxlIG5hbWU9XCJzY3JpcHQuanNcIj5cbiAgICAgICBhbmd1bGFyLm1vZHVsZSgnbGlua3lFeGFtcGxlJywgWyduZ1Nhbml0aXplJ10pXG4gICAgICAgICAuY29udHJvbGxlcignRXhhbXBsZUNvbnRyb2xsZXInLCBbJyRzY29wZScsIGZ1bmN0aW9uKCRzY29wZSkge1xuICAgICAgICAgICAkc2NvcGUuc25pcHBldCA9XG4gICAgICAgICAgICAgJ1ByZXR0eSB0ZXh0IHdpdGggc29tZSBsaW5rczpcXG4nK1xuICAgICAgICAgICAgICdodHRwOi8vYW5ndWxhcmpzLm9yZy8sXFxuJytcbiAgICAgICAgICAgICAnbWFpbHRvOnVzQHNvbWV3aGVyZS5vcmcsXFxuJytcbiAgICAgICAgICAgICAnYW5vdGhlckBzb21ld2hlcmUub3JnLFxcbicrXG4gICAgICAgICAgICAgJ2FuZCBvbmUgbW9yZTogZnRwOi8vMTI3LjAuMC4xLy4nO1xuICAgICAgICAgICAkc2NvcGUuc25pcHBldFdpdGhTaW5nbGVVUkwgPSAnaHR0cDovL2FuZ3VsYXJqcy5vcmcvJztcbiAgICAgICAgIH1dKTtcbiAgICAgPC9maWxlPlxuICAgICA8ZmlsZSBuYW1lPVwicHJvdHJhY3Rvci5qc1wiIHR5cGU9XCJwcm90cmFjdG9yXCI+XG4gICAgICAgaXQoJ3Nob3VsZCBsaW5raWZ5IHRoZSBzbmlwcGV0IHdpdGggdXJscycsIGZ1bmN0aW9uKCkge1xuICAgICAgICAgZXhwZWN0KGVsZW1lbnQoYnkuaWQoJ2xpbmt5LWZpbHRlcicpKS5lbGVtZW50KGJ5LmJpbmRpbmcoJ3NuaXBwZXQgfCBsaW5reScpKS5nZXRUZXh0KCkpLlxuICAgICAgICAgICAgIHRvQmUoJ1ByZXR0eSB0ZXh0IHdpdGggc29tZSBsaW5rczogaHR0cDovL2FuZ3VsYXJqcy5vcmcvLCB1c0Bzb21ld2hlcmUub3JnLCAnICtcbiAgICAgICAgICAgICAgICAgICdhbm90aGVyQHNvbWV3aGVyZS5vcmcsIGFuZCBvbmUgbW9yZTogZnRwOi8vMTI3LjAuMC4xLy4nKTtcbiAgICAgICAgIGV4cGVjdChlbGVtZW50LmFsbChieS5jc3MoJyNsaW5reS1maWx0ZXIgYScpKS5jb3VudCgpKS50b0VxdWFsKDQpO1xuICAgICAgIH0pO1xuXG4gICAgICAgaXQoJ3Nob3VsZCBub3QgbGlua2lmeSBzbmlwcGV0IHdpdGhvdXQgdGhlIGxpbmt5IGZpbHRlcicsIGZ1bmN0aW9uKCkge1xuICAgICAgICAgZXhwZWN0KGVsZW1lbnQoYnkuaWQoJ2VzY2FwZWQtaHRtbCcpKS5lbGVtZW50KGJ5LmJpbmRpbmcoJ3NuaXBwZXQnKSkuZ2V0VGV4dCgpKS5cbiAgICAgICAgICAgICB0b0JlKCdQcmV0dHkgdGV4dCB3aXRoIHNvbWUgbGlua3M6IGh0dHA6Ly9hbmd1bGFyanMub3JnLywgbWFpbHRvOnVzQHNvbWV3aGVyZS5vcmcsICcgK1xuICAgICAgICAgICAgICAgICAgJ2Fub3RoZXJAc29tZXdoZXJlLm9yZywgYW5kIG9uZSBtb3JlOiBmdHA6Ly8xMjcuMC4wLjEvLicpO1xuICAgICAgICAgZXhwZWN0KGVsZW1lbnQuYWxsKGJ5LmNzcygnI2VzY2FwZWQtaHRtbCBhJykpLmNvdW50KCkpLnRvRXF1YWwoMCk7XG4gICAgICAgfSk7XG5cbiAgICAgICBpdCgnc2hvdWxkIHVwZGF0ZScsIGZ1bmN0aW9uKCkge1xuICAgICAgICAgZWxlbWVudChieS5tb2RlbCgnc25pcHBldCcpKS5jbGVhcigpO1xuICAgICAgICAgZWxlbWVudChieS5tb2RlbCgnc25pcHBldCcpKS5zZW5kS2V5cygnbmV3IGh0dHA6Ly9saW5rLicpO1xuICAgICAgICAgZXhwZWN0KGVsZW1lbnQoYnkuaWQoJ2xpbmt5LWZpbHRlcicpKS5lbGVtZW50KGJ5LmJpbmRpbmcoJ3NuaXBwZXQgfCBsaW5reScpKS5nZXRUZXh0KCkpLlxuICAgICAgICAgICAgIHRvQmUoJ25ldyBodHRwOi8vbGluay4nKTtcbiAgICAgICAgIGV4cGVjdChlbGVtZW50LmFsbChieS5jc3MoJyNsaW5reS1maWx0ZXIgYScpKS5jb3VudCgpKS50b0VxdWFsKDEpO1xuICAgICAgICAgZXhwZWN0KGVsZW1lbnQoYnkuaWQoJ2VzY2FwZWQtaHRtbCcpKS5lbGVtZW50KGJ5LmJpbmRpbmcoJ3NuaXBwZXQnKSkuZ2V0VGV4dCgpKVxuICAgICAgICAgICAgIC50b0JlKCduZXcgaHR0cDovL2xpbmsuJyk7XG4gICAgICAgfSk7XG5cbiAgICAgICBpdCgnc2hvdWxkIHdvcmsgd2l0aCB0aGUgdGFyZ2V0IHByb3BlcnR5JywgZnVuY3Rpb24oKSB7XG4gICAgICAgIGV4cGVjdChlbGVtZW50KGJ5LmlkKCdsaW5reS10YXJnZXQnKSkuXG4gICAgICAgICAgICBlbGVtZW50KGJ5LmJpbmRpbmcoXCJzbmlwcGV0V2l0aFNpbmdsZVVSTCB8IGxpbmt5OidfYmxhbmsnXCIpKS5nZXRUZXh0KCkpLlxuICAgICAgICAgICAgdG9CZSgnaHR0cDovL2FuZ3VsYXJqcy5vcmcvJyk7XG4gICAgICAgIGV4cGVjdChlbGVtZW50KGJ5LmNzcygnI2xpbmt5LXRhcmdldCBhJykpLmdldEF0dHJpYnV0ZSgndGFyZ2V0JykpLnRvRXF1YWwoJ19ibGFuaycpO1xuICAgICAgIH0pO1xuXG4gICAgICAgaXQoJ3Nob3VsZCBvcHRpb25hbGx5IGFkZCBjdXN0b20gYXR0cmlidXRlcycsIGZ1bmN0aW9uKCkge1xuICAgICAgICBleHBlY3QoZWxlbWVudChieS5pZCgnbGlua3ktY3VzdG9tLWF0dHJpYnV0ZXMnKSkuXG4gICAgICAgICAgICBlbGVtZW50KGJ5LmJpbmRpbmcoXCJzbmlwcGV0V2l0aFNpbmdsZVVSTCB8IGxpbmt5Oidfc2VsZic6e3JlbDogJ25vZm9sbG93J31cIikpLmdldFRleHQoKSkuXG4gICAgICAgICAgICB0b0JlKCdodHRwOi8vYW5ndWxhcmpzLm9yZy8nKTtcbiAgICAgICAgZXhwZWN0KGVsZW1lbnQoYnkuY3NzKCcjbGlua3ktY3VzdG9tLWF0dHJpYnV0ZXMgYScpKS5nZXRBdHRyaWJ1dGUoJ3JlbCcpKS50b0VxdWFsKCdub2ZvbGxvdycpO1xuICAgICAgIH0pO1xuICAgICA8L2ZpbGU+XG4gICA8L2V4YW1wbGU+XG4gKi9cbmFuZ3VsYXIubW9kdWxlKCduZ1Nhbml0aXplJykuZmlsdGVyKCdsaW5reScsIFsnJHNhbml0aXplJywgZnVuY3Rpb24oJHNhbml0aXplKSB7XG4gIHZhciBMSU5LWV9VUkxfUkVHRVhQID1cbiAgICAgICAgLygoZnRwfGh0dHBzPyk6XFwvXFwvfCh3d3dcXC4pfChtYWlsdG86KT9bQS1aYS16MC05Ll8lKy1dK0ApXFxTKlteXFxzLjssKCl7fTw+XCJcXHUyMDFkXFx1MjAxOV0vaSxcbiAgICAgIE1BSUxUT19SRUdFWFAgPSAvXm1haWx0bzovaTtcblxuICB2YXIgbGlua3lNaW5FcnIgPSBhbmd1bGFyLiQkbWluRXJyKCdsaW5reScpO1xuICB2YXIgaXNTdHJpbmcgPSBhbmd1bGFyLmlzU3RyaW5nO1xuXG4gIHJldHVybiBmdW5jdGlvbih0ZXh0LCB0YXJnZXQsIGF0dHJpYnV0ZXMpIHtcbiAgICBpZiAodGV4dCA9PSBudWxsIHx8IHRleHQgPT09ICcnKSByZXR1cm4gdGV4dDtcbiAgICBpZiAoIWlzU3RyaW5nKHRleHQpKSB0aHJvdyBsaW5reU1pbkVycignbm90c3RyaW5nJywgJ0V4cGVjdGVkIHN0cmluZyBidXQgcmVjZWl2ZWQ6IHswfScsIHRleHQpO1xuXG4gICAgdmFyIG1hdGNoO1xuICAgIHZhciByYXcgPSB0ZXh0O1xuICAgIHZhciBodG1sID0gW107XG4gICAgdmFyIHVybDtcbiAgICB2YXIgaTtcbiAgICB3aGlsZSAoKG1hdGNoID0gcmF3Lm1hdGNoKExJTktZX1VSTF9SRUdFWFApKSkge1xuICAgICAgLy8gV2UgY2FuIG5vdCBlbmQgaW4gdGhlc2UgYXMgdGhleSBhcmUgc29tZXRpbWVzIGZvdW5kIGF0IHRoZSBlbmQgb2YgdGhlIHNlbnRlbmNlXG4gICAgICB1cmwgPSBtYXRjaFswXTtcbiAgICAgIC8vIGlmIHdlIGRpZCBub3QgbWF0Y2ggZnRwL2h0dHAvd3d3L21haWx0byB0aGVuIGFzc3VtZSBtYWlsdG9cbiAgICAgIGlmICghbWF0Y2hbMl0gJiYgIW1hdGNoWzRdKSB7XG4gICAgICAgIHVybCA9IChtYXRjaFszXSA/ICdodHRwOi8vJyA6ICdtYWlsdG86JykgKyB1cmw7XG4gICAgICB9XG4gICAgICBpID0gbWF0Y2guaW5kZXg7XG4gICAgICBhZGRUZXh0KHJhdy5zdWJzdHIoMCwgaSkpO1xuICAgICAgYWRkTGluayh1cmwsIG1hdGNoWzBdLnJlcGxhY2UoTUFJTFRPX1JFR0VYUCwgJycpKTtcbiAgICAgIHJhdyA9IHJhdy5zdWJzdHJpbmcoaSArIG1hdGNoWzBdLmxlbmd0aCk7XG4gICAgfVxuICAgIGFkZFRleHQocmF3KTtcbiAgICByZXR1cm4gJHNhbml0aXplKGh0bWwuam9pbignJykpO1xuXG4gICAgZnVuY3Rpb24gYWRkVGV4dCh0ZXh0KSB7XG4gICAgICBpZiAoIXRleHQpIHtcbiAgICAgICAgcmV0dXJuO1xuICAgICAgfVxuICAgICAgaHRtbC5wdXNoKHNhbml0aXplVGV4dCh0ZXh0KSk7XG4gICAgfVxuXG4gICAgZnVuY3Rpb24gYWRkTGluayh1cmwsIHRleHQpIHtcbiAgICAgIHZhciBrZXk7XG4gICAgICBodG1sLnB1c2goJzxhICcpO1xuICAgICAgaWYgKGFuZ3VsYXIuaXNGdW5jdGlvbihhdHRyaWJ1dGVzKSkge1xuICAgICAgICBhdHRyaWJ1dGVzID0gYXR0cmlidXRlcyh1cmwpO1xuICAgICAgfVxuICAgICAgaWYgKGFuZ3VsYXIuaXNPYmplY3QoYXR0cmlidXRlcykpIHtcbiAgICAgICAgZm9yIChrZXkgaW4gYXR0cmlidXRlcykge1xuICAgICAgICAgIGh0bWwucHVzaChrZXkgKyAnPVwiJyArIGF0dHJpYnV0ZXNba2V5XSArICdcIiAnKTtcbiAgICAgICAgfVxuICAgICAgfSBlbHNlIHtcbiAgICAgICAgYXR0cmlidXRlcyA9IHt9O1xuICAgICAgfVxuICAgICAgaWYgKGFuZ3VsYXIuaXNEZWZpbmVkKHRhcmdldCkgJiYgISgndGFyZ2V0JyBpbiBhdHRyaWJ1dGVzKSkge1xuICAgICAgICBodG1sLnB1c2goJ3RhcmdldD1cIicsXG4gICAgICAgICAgICAgICAgICB0YXJnZXQsXG4gICAgICAgICAgICAgICAgICAnXCIgJyk7XG4gICAgICB9XG4gICAgICBodG1sLnB1c2goJ2hyZWY9XCInLFxuICAgICAgICAgICAgICAgIHVybC5yZXBsYWNlKC9cIi9nLCAnJnF1b3Q7JyksXG4gICAgICAgICAgICAgICAgJ1wiPicpO1xuICAgICAgYWRkVGV4dCh0ZXh0KTtcbiAgICAgIGh0bWwucHVzaCgnPC9hPicpO1xuICAgIH1cbiAgfTtcbn1dKTtcblxuXG59KSh3aW5kb3csIHdpbmRvdy5hbmd1bGFyKTtcblxuXG5cbi8qKioqKioqKioqKioqKioqKlxuICoqIFdFQlBBQ0sgRk9PVEVSXG4gKiogL1VzZXJzL3h1Z2UvcHJvamVjdC9xdWFuc2hpLXVpL34vYW5ndWxhci1zYW5pdGl6ZS9hbmd1bGFyLXNhbml0aXplLmpzXG4gKiogbW9kdWxlIGlkID0gMTg4XG4gKiogbW9kdWxlIGNodW5rcyA9IDAgMSAyIDMgNSA2IDdcbiAqKi8iXSwic291cmNlUm9vdCI6IiJ9