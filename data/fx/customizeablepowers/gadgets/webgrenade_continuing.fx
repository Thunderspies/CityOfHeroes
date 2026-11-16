#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	wepR
End

Input  
	Inpname	wepL
End

Input  
	Inpname	LlegR
End

Input  
	Inpname	LlegL
End
#################################################################################
Condition
	On	Time
	Time	0

	Event
		Type	local
		At	root
		Sound webgrexplo2 88 88 .95
	End
End	


Condition
	On 	Time
	Time 	0

	Event
		EName	webcage
		Type	posit
		At	root		
		Geom	netwebbingsmallCage
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	1
		Part	CustomizeablePowers\Gadgets\web.part
		POther	LlegR
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	1
		Part	CustomizeablePowers\Gadgets\web.part
		POther	LlegL
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	2
		Part	CustomizeablePowers\Gadgets\web.part
		POther	LlegR
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	10
		Part	CustomizeablePowers\Gadgets\web.part
		POther	LlegL
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	9
		Part	CustomizeablePowers\Gadgets\web.part
		POther	LlegR
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	3
		Part	CustomizeablePowers\Gadgets\web.part
		POther	LlegL
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	8
		Part	CustomizeablePowers\Gadgets\web.part
		POther	LlegR
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	4
		Part	CustomizeablePowers\Gadgets\web.part
		POther	LlegL
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	7
		Part	CustomizeablePowers\Gadgets\web.part
		POther	LlegR
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	5
		Part	CustomizeablePowers\Gadgets\web.part
		POther	LlegL
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	6
		Part	CustomizeablePowers\Gadgets\web.part
		POther	LlegR
				
	End

	
#######################################################################
	

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	1
		Part	CustomizeablePowers\Gadgets\web.part
		POther	WepL
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	1
		Part	CustomizeablePowers\Gadgets\web.part
		POther	WepR
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	2
		Part	CustomizeablePowers\Gadgets\web.part
		POther	WepL
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	3
		Part	CustomizeablePowers\Gadgets\web.part
		POther	WepL
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	4
		Part	CustomizeablePowers\Gadgets\web.part
		POther	WepL
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	5
		Part	CustomizeablePowers\Gadgets\web.part
		POther	WepL
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	6
		Part	CustomizeablePowers\Gadgets\web.part
		POther	WepR
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	7
		Part	CustomizeablePowers\Gadgets\web.part
		POther	WepR
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	8
		Part	CustomizeablePowers\Gadgets\web.part
		POther	WepR
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	9
		Part	CustomizeablePowers\Gadgets\web.part
		POther	WepR
				
	End

	Event
		EName	webcage
		Type	local
		At	WebCage
		Altpiv	10
		Part	CustomizeablePowers\Gadgets\web.part
		POther	WepR
				
	End



	Event
		EName	lazer
		Type	local
		At	root
			
			
		Geom	web1
		
		
	End

	Event
		EName	lazer
		Type	local
		At	root
			
			
		Geom	web2
		
		
	End

	Event
		EName	lazer
		Type	local
		At	root
			
			
		Geom	web3
		
		
	End

	Event
		EName	lazer
		Type	local
		At	root
			
			
		Geom	web4
		
		
	End
	
End


End