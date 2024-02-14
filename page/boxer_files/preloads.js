
    (function() {
      var baseURL = "https://cdn.shopify.com/shopifycloud/checkout-web/assets/";
      var scripts = ["https://cdn.shopify.com/shopifycloud/checkout-web/assets/runtime.latest.fr.dbbdac1810dbd87c055a.js","https://cdn.shopify.com/shopifycloud/checkout-web/assets/456.latest.fr.6ad11b7c37558109a81d.js","https://cdn.shopify.com/shopifycloud/checkout-web/assets/835.latest.fr.c60bd4bab29ab30e678f.js","https://cdn.shopify.com/shopifycloud/checkout-web/assets/40.latest.fr.48cd49fd7656219dadd9.js","https://cdn.shopify.com/shopifycloud/checkout-web/assets/app.latest.fr.af19e488892eba852628.js","https://cdn.shopify.com/shopifycloud/checkout-web/assets/240.latest.fr.d48b54ea867b809eedba.js","https://cdn.shopify.com/shopifycloud/checkout-web/assets/904.latest.fr.2004013e445b7353dc80.js","https://cdn.shopify.com/shopifycloud/checkout-web/assets/44.latest.fr.ed5da7e5a1dddfca0e79.js","https://cdn.shopify.com/shopifycloud/checkout-web/assets/OnePage.latest.fr.e7ca252888e98a162960.js"];
      var styles = ["https://cdn.shopify.com/shopifycloud/checkout-web/assets/456.latest.fr.800164302d4c7459140d.css","https://cdn.shopify.com/shopifycloud/checkout-web/assets/app.latest.fr.e5a7f63ca146c0549466.css","https://cdn.shopify.com/shopifycloud/checkout-web/assets/904.latest.fr.4d273af8acf76b1eb555.css","https://cdn.shopify.com/shopifycloud/checkout-web/assets/457.latest.fr.13041160ccc1912b17e1.css"];
      var fontPreconnectUrls = [];
      var fontPrefetchUrls = [];
      var imgPrefetchUrls = ["https://cdn.shopify.com/s/files/1/0852/2344/files/lp-logo_881f41c1-7712-4088-86ea-3bf26adce4fc_x320.png?v=1624460972"];

      function preconnect(url, callback) {
        var link = document.createElement('link');
        link.rel = 'dns-prefetch preconnect';
        link.href = url;
        link.crossOrigin = '';
        link.onload = link.onerror = callback;
        document.head.appendChild(link);
      }

      function preconnectAssets() {
        var resources = [baseURL].concat(fontPreconnectUrls);
        var index = 0;
        (function next() {
          var res = resources[index++];
          if (res) preconnect(res[0], next);
        })();
      }

      function prefetch(url, as, callback) {
        var link = document.createElement('link');
        if (link.relList.supports('prefetch')) {
          link.rel = 'prefetch';
          link.fetchPriority = 'low';
          link.as = as;
          if (as === 'font') link.type = 'font/woff2';
          link.href = url;
          link.crossOrigin = '';
          link.onload = link.onerror = callback;
          document.head.appendChild(link);
        } else {
          var xhr = new XMLHttpRequest();
          xhr.open('GET', url, true);
          xhr.onloadend = callback;
          xhr.send();
        }
      }

      function prefetchAssets() {
        var resources = [].concat(
          scripts.map(function(url) { return [url, 'script']; }),
          styles.map(function(url) { return [url, 'style']; }),
          fontPrefetchUrls.map(function(url) { return [url, 'font']; }),
          imgPrefetchUrls.map(function(url) { return [url, 'image']; })
        );
        var index = 0;
        (function next() {
          var res = resources[index++];
          if (res) prefetch(res[0], res[1], next);
        })();
      }

      function onLoaded() {
        preconnectAssets();
        prefetchAssets();
      }

      if (document.readyState === 'complete') {
        onLoaded();
      } else {
        addEventListener('load', onLoaded);
      }
    })();
  