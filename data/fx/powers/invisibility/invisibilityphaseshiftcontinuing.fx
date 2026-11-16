#############################################################
## InvisibilityPhaseShiftContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	Root
End

#############################################################

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound phase_loop 80 80 .44
	End
End




#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	15

#	Event
#		ENAME	Alpha
#		Type	Local
#		At	Origin
#
#		BhvrOverride
#
#			FadeInLength		100
#			FadeOutLength		100
#			Alpha			180
#
#		End
#
#		Flags	AdoptParentEntity
#
#
#	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End


End


End