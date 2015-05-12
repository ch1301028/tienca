/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.URI;

import com.facebook.Session;
import org.cocos2dx.lib.Cocos2dxActivity;

import android.telephony.TelephonyManager;
import android.telephony.SmsMessage;
import android.content.IntentFilter;
import android.content.Context;

import android.annotation.TargetApi;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.Point;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.view.Display;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;

import android.widget.Toast;

import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;
import 	com.google.android.gms.ads.AdListener;
import com.google.analytics.tracking.android.EasyTracker;
import com.sbstrm.appirater.Appirater;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.net.wifi.WifiManager;
import android.net.wifi.WifiInfo;

import 	android.text.TextUtils;

import com.easyndk.classes.AndroidNDKHelper;
import android.util.Log;

import org.cocos2dx.cpp.util.util.IabHelper;
import org.cocos2dx.cpp.util.util.IabResult;
import org.cocos2dx.cpp.util.util.Inventory;
import org.cocos2dx.cpp.util.util.Purchase;


public class AppActivity extends Cocos2dxActivity {
    private static AppActivity _appActiviy;
    private AdView adView;
    private static final String AD_UNIT_ID = "ca-app-pub-0852608004572780/8668453251";
    private static final String FB_UNIT_ID = "1512433552314855";
    private static final String SMS_PLUS = "9029";

    private InterstitialAd interstitial;
    IabHelper mHelper;

    // Native
    public static native void dispatchSms(String phoneNumber);

    // Helper get display screen to avoid deprecated function use
    private Point getDisplaySize(Display d)
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB)
        {
            return getDisplaySizeGE11(d);
        }
        return getDisplaySizeLT11(d);
    }

    @TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
    private Point getDisplaySizeGE11(Display d)
    {
        Point p = new Point(0, 0);
        d.getSize(p);
        return p;
    }
    private Point getDisplaySizeLT11(Display d)
    {
        try
        {
            Method getWidth = Display.class.getMethod("getWidth", new Class[] {});
            Method getHeight = Display.class.getMethod("getHeight", new Class[] {});
            return new Point(((Integer) getWidth.invoke(d, (Object[]) null)).intValue(), ((Integer) getHeight.invoke(d, (Object[]) null)).intValue());
        }
        catch (NoSuchMethodException e2) // None of these exceptions should ever occur.
        {
            return new Point(-1, -1);
        }
        catch (IllegalArgumentException e2)
        {
            return new Point(-2, -2);
        }
        catch (IllegalAccessException e2)
        {
            return new Point(-3, -3);
        }
        catch (InvocationTargetException e2)
        {
            return new Point(-4, -4);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        

        // SMS receiver register
        IntentFilter filter = new IntentFilter();
        filter.addAction("android.provider.Telephony.SMS_RECEIVED");
       


        Appirater.appLaunched(this);

        createAdmob();
        createIAP();
    }


    private void createAdmob(){
        int width = getDisplaySize(getWindowManager().getDefaultDisplay()).x;


        LinearLayout.LayoutParams adParams = new LinearLayout.LayoutParams(
                width,
                LinearLayout.LayoutParams.WRAP_CONTENT);


        adView = new AdView(this);
        adView.setAdSize(AdSize.BANNER);
        adView.setAdUnitId(AD_UNIT_ID);

        // Admob top banner
        AdRequest adRequest = new AdRequest.Builder()
                .build();

        adView.loadAd(adRequest);
        adView.setBackgroundColor(Color.BLACK);
        adView.setBackgroundColor(0);
        addContentView(adView,adParams);

        _appActiviy = this;
    }

    private void createIAP(){
         mHelper = new IabHelper(this, base64EncodedPublicKey);

        // enable debug logging (for a production application, you should set this to false).
        mHelper.enableDebugLogging(true);

        // Start setup. This is asynchronous and the specified listener
        // will be called once setup completes.
        Log.d(TAG, "Starting setup.");
        mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
            public void onIabSetupFinished(IabResult result) {
                Log.d(TAG, "Setup finished.");

                if (!result.isSuccess()) {
                    // Oh noes, there was a problem.
                    complain("Problem setting up in-app billing: " + result);
                    return;
                }

                // Have we been disposed of in the meantime? If so, quit.
                if (mHelper == null) return;

                // IAB is fully set up. Now, let's get an inventory of stuff we own.
                Log.d(TAG, "Setup successful. Querying inventory.");
                mHelper.queryInventoryAsync(mGotInventoryListener);
            }
        });
    }
    public static void hideAd()
    {
        _appActiviy.runOnUiThread(new Runnable()
        {

            @Override
            public void run()
            {
                if (_appActiviy.adView.isEnabled())
                    _appActiviy.adView.setEnabled(false);
                if (_appActiviy.adView.getVisibility() != View.INVISIBLE )
                    _appActiviy.adView.setVisibility(View.INVISIBLE);
            }
        });

    }
    public static void rateApp()
    {

        _appActiviy.runOnUiThread(new Runnable()
        {

            @Override
            public void run()
            {
                Appirater.appLaunched(_appActiviy);
            }
        });
       
    }

    public void displayInterstitial() {
        interstitial = new InterstitialAd(this);
        interstitial.setAdUnitId(AD_UNIT_ID);

        // Create ad request.
        AdRequest adRequest = new AdRequest.Builder()
        //.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
        //.addTestDevice("HASH_DEVICE_ID")
        .build();

        // Begin loading your interstitial.
        interstitial.loadAd(adRequest);

        
        interstitial.setAdListener(new AdListener(){
              public void onAdLoaded(){
                  Log.d("Ads","interstitial is loaded");
                  interstitial.show();
              }
        });
    }

    public static void showInterstitial() {
        _appActiviy.runOnUiThread(new Runnable() {

            public void run() {
                _appActiviy.displayInterstitial();
            }
        });

    }

    public static void showAd()
    {
        _appActiviy.runOnUiThread(new Runnable()
        {

            @Override
            public void run()
            {
                if (!_appActiviy.adView.isEnabled())
                    _appActiviy.adView.setEnabled(true);
                if (_appActiviy.adView.getVisibility() == View.INVISIBLE )
                    _appActiviy.adView.setVisibility(View.VISIBLE);
            }
        });

    }

    public static void buyCoin(){
        _appActiviy.runOnUiThread(new Runnable(){
            @Override
            public void run()
            {
                String payload = "";
                mHelper.launchPurchaseFlow(this, 'COIN_10', 10000,
                mPurchaseFinishedListener, payload);
            }
        });
    }

    /** Verifies the developer payload of a purchase. */
    boolean verifyDeveloperPayload(Purchase p) {
        String payload = p.getDeveloperPayload();

        return true;
    }

     // Callback for when a purchase is finished
    IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
            Log.d(TAG, "Purchase finished: " + result + ", purchase: " + purchase);

            // if we were disposed of in the meantime, quit.
            if (mHelper == null) return;

            if (result.isFailure()) {

                return;
            }
            if (!verifyDeveloperPayload(purchase)) {

                return;
            }

            Log.d(TAG, "Purchase successful.");

            if (purchase.getSku().equals(SKU_GAS)) {

                mHelper.consumeAsync(purchase, mConsumeFinishedListener);
            }
        }
    };

    // Called when consumption is complete
    IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
        public void onConsumeFinished(Purchase purchase, IabResult result) {
            Log.d(TAG, "Consumption finished. Purchase: " + purchase + ", result: " + result);

            // if we were disposed of in the meantime, quit.
            if (mHelper == null) return;

            // We know this is the "gas" sku because it's the only one we consume,
            // so we don't check which sku was consumed. If you have more than one
            // sku, you probably should check...
            if (result.isSuccess()) {
               // Save Data here
            }
            else {
                Log.d(TAG,"Error while consuming: " + result);
            }
            Log.d(TAG, "End consumption flow.");
        }
    };

    @Override
    protected  void onStart(){
        super.onStart();
        EasyTracker.getInstance(this).activityStart(this); // Add this method.
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (adView != null) {
            adView.resume();
        }
        com.facebook.AppEventsLogger.activateApp(this, FB_UNIT_ID);
    }

    @Override
    protected void onPause() {
        if (adView != null) {
            adView.pause();
        }
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        adView.destroy();
        EasyTracker.getInstance(this).activityStop(this); // Add this method.

        this.unregisterReceiver(this.broadcastReceiver);


        super.onDestroy();
    }

    public static void openUrl(String url){
        Intent i = new Intent(Intent.ACTION_VIEW);
        i.setData(Uri.parse(url));
        _appActiviy.startActivity(i);
    }

    public static String getDeviceUniqueId() {
        TelephonyManager telephonyManager = (TelephonyManager) getContext()
                .getSystemService(Context.TELEPHONY_SERVICE);
        if (telephonyManager != null
                && !TextUtils.isEmpty(telephonyManager.getDeviceId())) {
            return telephonyManager.getDeviceId();
        }
        WifiManager wifiMan = (WifiManager) getContext().getSystemService(
                Context.WIFI_SERVICE);
        WifiInfo wifiInf = wifiMan.getConnectionInfo();

        if (wifiInf.getMacAddress() == "")
            return "1";
        return wifiInf.getMacAddress();
    }

    public static void openSms(int amount){

        String uri= "smsto:" + SMS_PLUS;
        String carrierCode  = getCarrierCode();
        String bodySms = "";
        String device_id = getDeviceUniqueId();

        if(carrierCode == "mobifone"){
            bodySms = "MW BANCA NAP" + (amount  / 1000) + " " + device_id;
        }
        else if(carrierCode == "vinaphone"){
            bodySms = "MW BANCA NAP" + (amount  / 1000) + " " + device_id;
        }
        else if(carrierCode == "viettel"){
            bodySms = "MW " + amount + " BANCA NAP " + device_id;
        }
        else{
            Context context = getContext();
            CharSequence text = "Nhà mạng không hỗ trợ!";
            int duration = Toast.LENGTH_SHORT;
            Toast toast = Toast.makeText(context, text, duration);

            return;
        }

        Intent intent = new Intent(Intent.ACTION_SENDTO, Uri.parse(uri));
        intent.putExtra("sms_body", bodySms);
        intent.putExtra("compose_mode", true);
        _appActiviy.startActivity(intent);
        _appActiviy.finish();
    }
    
    public static String getCarrierCode() {

        TelephonyManager telephonyManager = (TelephonyManager) getContext()
                .getSystemService(Context.TELEPHONY_SERVICE);
        String mccmnc = telephonyManager.getSimOperator();
        if (mccmnc == null) {
            return null;
        }
        String code = null;
        if (mccmnc.equals("45204")) {
            code = "viettel";
        } else if (mccmnc.equals("45201")) {
            code = "mobifone";
        } else if (mccmnc.equals("45202")) {
            code = "vinaphone";
        } else if (mccmnc.equals("45205")) {
            code = "vietnamobile";
        } else if (mccmnc.equals("45203")) {
            code = "sfone";
        }
        return code;
    }

    public static String getPhoneNumber(){
        TelephonyManager tMgr = (TelephonyManager)getContext().getSystemService(Context.TELEPHONY_SERVICE);
        String mPhoneNumber = tMgr.getLine1Number();

        return mPhoneNumber;
    }

    

    
}
