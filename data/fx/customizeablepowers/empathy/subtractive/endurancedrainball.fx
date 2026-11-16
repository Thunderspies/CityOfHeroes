#############################################################
## EnduranceDrainBall.fx
#############################################################

FxInfo
Lifespan 65
##############################################################


Condition
	On	Time
	Time	20


	Event
		Ename	Child01
		Type	Local
		At	Root
		ChildFx :IntelligenceOtherRings.fx
		#Magnet	target
		#LookAt  target
	End


	Event
		EName	1
		Type	Local
		At	Hips
		Part	:Endurance01.part
	End

	Event
		EName	2
		Type	Local
		At	FootR
		Part	:EnduranceBodyGlows.part
		#Part	:EnduranceBodyGlowsFlat.part
	End

#LegL


	Event
		EName	3
		Type	Local
		At	ULEGL
		Part	:EnduranceMotionGlows.part
		PMagnet Knee1
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		Part	:EnduranceMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		Part	:EnduranceBodyGlows.part
		#Part	:EnduranceBodyGlowsFlat.part
	End


	Event
		EName	6
		Type	Local
		At	ULEGR
		Part	:EnduranceMotionGlows.part
		PMagnet Kneer
	End

#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		Part	:EnduranceMotionGlows.part
		PMagnet FootR
	End
###############################################################################

	Event
		EName	9
		Type	Local
		At	Hair
		Part	:EnduranceHeadGlows.part
	End
##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		Part	:EnduranceArmMotionGlows.part
		Part	:EnduranceArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		Part	:EnduranceArmMotionGlows.part
		Part	:EnduranceArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		Part	:EnduranceArmGlows.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		Part	:EnduranceArmMotionGlows.part
		Part	:EnduranceArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		Part	:EnduranceArmMotionGlows.part
		Part	:EnduranceArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		Part	:EnduranceArmGlows.part
	End
End

End

