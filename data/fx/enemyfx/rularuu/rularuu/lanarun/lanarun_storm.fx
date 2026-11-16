#########################################################
##	template

FxInfo

Flags    InheritAlpha

#########################################################


Condition
	On	Time
	Time	25

	Event
		Type	Local	
		At	Origin
		Sound storm_loop 140 140 .38
	End
End



Condition

	On 	Time	
	Time	0

	Event	
		ENAME	Cloud
		Type	Local
		At	Head
		part	:Storm_Heart_01.part
		part	:Storm_Center_01.part
		#part	:Storm_Center_02.part	
	End

	Event	
		ENAME	Cloud
		Type	Local
		At	Hair
		part	:Storm_Heart_01.part
		#part	:Storm_Center_01.part
		#part	:Storm_Center_02.part	
	End

	Event	
		ENAME	Cloud
		Type	Local
		At	Col_R
		part	:Storm_Heart_01.part
		part	:Storm_Center_01.part
		part	:Storm_Center_02.part	
		Bhvr	:Offset_Point_Col_R.bhvr
	End

	Event	
		ENAME	Cloud
		Type	Local
		At	LArmR
		part	:Storm_Heart_01.part
		part	:Storm_Center_01.part
		part	:Storm_Center_02.part	
	End

	Event	
		ENAME	Cloud
		Type	Local
		At	HandR
		part	:Storm_Heart_01.part
		part	:Storm_Center_01.part
		part	:Storm_Center_02.part	
	End

	Event	
		ENAME	Cloud
		Type	Local
		At	Col_L
		part	:Storm_Heart_01.part
		part	:Storm_Center_01.part
		part	:Storm_Center_02.part	
		Bhvr	:Offset_Point_Col_L.bhvr
	End

	Event	
		ENAME	Cloud
		Type	Local
		At	LArmL
		part	:Storm_Heart_01.part
		part	:Storm_Center_01.part
		part	:Storm_Center_02.part	
	End

	Event	
		ENAME	Cloud
		Type	Local
		At	HandL
		part	:Storm_Heart_01.part
		part	:Storm_Center_01.part
		part	:Storm_Center_02.part	
	End

	Event	
		ENAME	Col_R_Offset
		Type	Local
		At	Col_R
		Bhvr	:Offset_Point_Col_R.bhvr
	End
	
	Event	
		ENAME	Col_L_Offset
		Type	Local
		At	Col_L
		Bhvr	:Offset_Point_Col_L.bhvr
	End

#####################  Eyes

	Event
		EName	1
		Type	Local
		At	Head
		part	:Glow_Eye.part
		bhvr	:Offset_Eye_R.bhvr
	End

	Event
		EName	1
		Type	Local
		At	Head
		part	:Glow_Eye.part
		bhvr	:Offset_Eye_L.bhvr
	End	
End

##################### LIGHTNING FLASHES

Condition
	On 	cycle
	Time	60
	Chance	0.5
	Event	
		ENAME	Lightning
		Type	Local
		At	Head
		part	:Storm_Lightning_01.part
		part	:Storm_Lightning_02.part
		part	:SSLSCloudLightning01a.part
		#Pother	Hair
		lifespan	15
	End

End

Condition
	On 	cycle
	Time	50
	Chance	0.5
	Event	
		ENAME	Lightning
		Type	Local
		At	Col_R_Offset
		part	:Storm_Lightning_01.part
		part	:Storm_Lightning_02.part
		part	:SSLSCloudLightning04.part
		POther	Neck
		lifespan	15
	End
End

Condition
	On 	cycle
	Time	40
	Chance	0.5
	Event	
		ENAME	Lightning
		Type	Local
		At	Col_L_Offset
		part	:Storm_Lightning_01.part
		part	:Storm_Lightning_02.part
		part	:SSLSCloudLightning02.part
		POther	Neck
		lifespan	15
	End
End

Condition
	On 	cycle
	Time	40
	Chance	0.5
	Event	
		ENAME	Lightning
		Type	Local
		At	HandR
		part	:SSLSCloudLightning02.part
		POther	LArmR
		lifespan	15
	End
End

Condition
	On 	cycle
	Time	65
	Chance	0.5
	Event	
		ENAME	Lightning
		Type	Local
		At	HandL
		part	:SSLSCloudLightning03.part
		POther	LArmL
		lifespan	15
	End
End

Condition
	On 	cycle
	Time	45
	Chance	0.5
	Event	
		ENAME	Lightning
		Type	Local
		At	LArmR
		part	:Storm_Lightning_01.part
		part	:Storm_Lightning_02.part
		part	:SSLSCloudLightning01.part
		POther	UArmR
		lifespan	15
	End
End

Condition
	On 	cycle
	Time	55
	Chance	0.5
	Event	
		ENAME	Lightning
		Type	Local
		At	LArmL
		part	:Storm_Lightning_01.part
		part	:Storm_Lightning_02.part
		part	:SSLSCloudLightning04.part
		POther	UArmL
		lifespan	15
	End
End
	