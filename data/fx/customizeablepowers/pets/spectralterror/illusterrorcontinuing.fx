#############################################################
## IllusTerrorContinuing.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

#########################################################
####################### AUDIO ###########################
#########################################################
Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound terror2 80 80 .7
	End

	Event
		Type	Local
		At 	Origin
		Sound terror2_loop 80 80 .4
	End
End




#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	30

	Event
		ENAME	ChestGlowMain
		Type	Local
		At	Neck
		part	:IllusTerrorChestGlow01.part
		part	:IllusTerrorRisingGlows01.part
		POther  Hips

	End

	Event
		ENAME	RightChestGlow
		Type	Local
		At	UarmR
		part	:IllusTerrorGlow01.part
		part	:IllusTerrorBodyGlow01.part
		part	:IllusTerrorRays01.part
		POther  Hips


	End

	Event
		ENAME	LeftChestGlow
		Type	Local
		At	UarmL
		part	:IllusTerrorGlow01.part
		part	:IllusTerrorBodyGlow01.part
		part	:IllusTerrorRays01.part
		POther  Hips


	End

	Event
		ENAME	Face
		Type	Local
		At	Eyes
		part	:IllusTerrorFace01.part
		part	:IllusTerrorRisingGlows01.part
		POther  Chest


	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:IllusTerrorGlow01.part
		part	:IllusTerrorRisingGlows01.part
		part	:IllusTerrorBodyGlow01.part
		POther  LarmR

	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:IllusTerrorGlow01.part
		part	:IllusTerrorRisingGlows01.part
		part	:IllusTerrorBodyGlow01.part
		POther  LarmL
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		part	:IllusTerrorGlow01.part
		part	:IllusTerrorRisingGlows01.part
		part	:IllusTerrorBodyGlow01.part
		POther  LarmR
	End


	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		part	:IllusTerrorGlow01.part
		part	:IllusTerrorRisingGlows01.part
		part	:IllusTerrorBodyGlow01.part
		POther  LarmL
	End

	Event
		ENAME	LeftHandGlow
		Type	Local
		At	HandL
		part	:IllusTerrorWristGlow01.part
		POther  RingL
	End

	Event
		ENAME	RightHandGlow
		Type	Local
		At	HandR
		part	:IllusTerrorWristGlow01.part
		POther  RingR
	End

############################## OLD HANDS ##############################
	Event
		ENAME	HandLeft
		Type	Local
		At	HandL
		part	:IllusTerrorHand02.part
	End

	Event
		ENAME	HandRight
		Type	Local
		At	HandR
		part	:IllusTerrorHand02.part
	End

End

############################## SUMMON ##############################
Condition
	On	Time
	Time	0

	Event
		ENAME	ChestMagnet
		Type	Start
		At	Chest
		part	:IllusTerrorSummonCloudIn.part
		part	:IllusTerrorSummonGlows.part
		Lifespan 35
	End
End

Condition
	On	Time
	Time	35

	Event
		ENAME	ChestMagnet
		Type	Start
		At	Head
		part	:IllusTerrorSummonGlowStatic.part
		part	:IllusTerrorSummonRays01.part
		Lifespan 15
	End
End

End
##################################

