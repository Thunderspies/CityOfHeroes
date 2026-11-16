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
		EName	1
		Type	Local
		At	Head
		part	:Body_Glow_01.part
		POther	Hair
	End

	Event
		EName	1
		Type	Local
		At	Hair
		part	:Body_Glow_02.part
		Bhvr	:Offset_Glow_Hair.bhvr
	End

################  CHEST

	Event
		EName	1
		Type	Local
		At	Hips
		part	:Body_Glow_03.part
		Bhvr	:Offset_Glow_ChestR.bhvr
		POther	UArmR
		Sound ruladak1_loop 100 100 .37
	End

	Event
		EName	1
		Type	Local
		At	Hips
		part	:Body_Glow_03.part
		Bhvr	:Offset_Glow_ChestL.bhvr
		POther	UArmL
	End

################### UArmR

	Event
		EName	1
		Type	Local
		At	Col_R
		part	:Body_Glow_03.part
		Bhvr	:Offset_Glow_Col_R.bhvr
		POther	Neck
	End

	Event
		EName	1
		Type	Local
		At	Col_R
		part	:Body_Glow_03.part
		Bhvr	:Offset_Glow_Col_R.bhvr
		POther	LArmR
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
		part	:Body_Glow_03.part
		#Bhvr	:Offset_Glow_Col_R.bhvr
		POther	HandR
	End

####################  HANDR

	Event
		EName	1
		Type	Local
		At	RingR
		part	:Body_Glow_Hand.part
	End

	Event
		EName	1
		Type	Local
		At	F1_R
		part	:Body_Glow_Hand.part
	End

	Event
		EName	1
		Type	Local
		At	T1_R
		part	:Body_Glow_Hand.part
	End

################### UArmL

	Event
		EName	1
		Type	Local
		At	Col_L
		part	:Body_Glow_03.part
		Bhvr	:Offset_Glow_Col_L.bhvr
		POther	Neck
	End

	Event
		EName	1
		Type	Local
		At	Col_L
		part	:Body_Glow_03.part
		Bhvr	:Offset_Glow_Col_L.bhvr
		POther	LArmL
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
		part	:Body_Glow_03.part
		POther	HandL
	End

####################  HANDR

	Event
		EName	1
		Type	Local
		At	RingL
		part	:Body_Glow_Hand.part
	End

	Event
		EName	1
		Type	Local
		At	F1_L
		part	:Body_Glow_Hand.part
	End

	Event
		EName	1
		Type	Local
		At	T1_L
		part	:Body_Glow_Hand.part
	End

####################  Hips

	Event
		EName	1
		Type	Local
		At	Hips
		part	:Body_Glow_Hips.part
		bhvr	:Offset_Hips_01.bhvr
	End

	Event
		EName	1
		Type	Local
		At	Hips
		part	:Body_Glow_Hips2.part
		bhvr	:Offset_Hips_02.bhvr
	End

####################  Eyes

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