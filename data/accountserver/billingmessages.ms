"OpieCall", <<
<?xml version='1.0' ?>
<env:Envelope xmlns:env="http://www.w3.org/2003/05/soap-envelope" >
 <env:Body>
  {funcall_body} 
 </env:Body>
</env:Envelope>
>>
"OpieInit", <<  
<m:init xmlns:m="uri:NC/API/op">
        <account>{authname}</account>
        <game>COH</game>
</m:init>        
>>
"OpiePay", <<
<m:pay xmlns:m="uri:NC/API/op">
        <handle>{handle}</handle>
      <order>
        <items>
          <item0>
            <product_item_id>{product_id}</product_item_id>
          </item0>
        </items>
        <client_tag>{guid}</client_tag>
      </order>
      <payment>
        <use_card_on_file>1</use_card_on_file>
      </payment>
</m:pay> 
>>
"OpieGetCatalog", << 
<m:getCatalog xmlns:m="uri:NC/API/op">
        <handle>{handle}</handle>
        <filter>
                <currency_id>{currency_id}</currency_id>
        </filter>
</m:getCatalog>
>>
"OpieGetBillingProfile", << 
<m:getBillingProfile xmlns:m="uri:NC/API/op">
        <handle>{handle}</handle>
</m:getBillingProfile>
>>



# Test functions

"OpieSleep", "    <m:sleep xmlns:m=\"uri:NC/API/op\">{time}</m:sleep> "
"OpieEnd", "    <m:end xmlns:m=\"uri:NC/API/op\">{handle}</m:end> "
