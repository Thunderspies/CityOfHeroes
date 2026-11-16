#########################################################
##	template

FxInfo

Flags    InheritAlpha

#########################################################

############## RIGHT ARM

Condition
	On 	Time
	Time 	0	

	Event
		Type	Local
		At	hips
		part	:Sparks.part
		
	End

	Event
		EName	1
		Type	Local
		At	Head
		part	:Body_Glow_01.part
		POther	Hair
	End

	Event
		EName	1
		Type	Local
		At	Col_R
		part	:Body_Glow_01.part
		POther	LArmR
	End

	Event
		EName	1
		Type	Local
		At	Col_L
		part	:Body_Glow_01.part
		POther	LArmL
	End

	Event
		EName	1
		Type	Local
		At	Col_L
		part	:Body_Glow_01.part
	End

	Event
		EName	1
		Type	Local
		At	Col_R
		part	:Body_Glow_01.part
	End

	Event
		EName	1
		Type	Local
		At	LArmR
		part	:Body_Glow_ArmJoint.part
	End

	Event
		EName	1
		Type	Local
		At	LArmR
		part	:Body_Glow_01.part
		POther	HandR
	End

	Event
		EName	1
		Type	Local
		At	LArmL
		part	:Body_Glow_ArmJoint.part
	End

	Event
		EName	1
		Type	Local
		At	LArmL
		part	:Body_Glow_01.part
		POther	HandL
	End

####################  HANDR

	Event
		EName	1
		Type	Local
		At	WepR
		part	:Body_Glow_01.part
	End

	Event
		EName	1
		Type	Local
		At	WepL
		part	:Body_Glow_01.part
	End

End
	
End
