#########################################################
##	Fire_Ball
##
FxInfo

#LifeSpan	200

#####################################################################################
##Flens Flares
#####################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	FW_CapturedCarnivalOfLights_Loop 50 50 .8
	End
End


Condition
	On	Time
	Time	0

	Event
		Ename	Ring
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0 -4 0
		End
		Part1 :Captured_CarnivalofLight_Dots.part
	End

End
Condition
	On 	Time
	Time 	5

	Event
		EName 	Bondage
		Type	Start
		At	Waist
		BhvrOverride
			Pyrrotate	20 0 0
			PositionOffset	0 -4.5 0
		end
		Part1	:Captured_CarnivalofLight_Ring.part
		Lifespan	0
	End
End



#####################################################################################



Condition
	On	Time
	Time	0

	Event
		EName	3
		Type	Local
		At	Hips
		part2	:MotionGlows.part
		Part3	:InvincibilityLightRays.part
		bhvr	Behaviors/GenericParticleFade.bhvr

	End


	Event
		EName	3
		Type	Local
		At	Hips
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	UlegL

	End

	Event
		EName	3
		Type	Local
		At	Hips
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	UlegR
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	ULEGL

	End

	Event
		EName	6
		Type	Local
		At	LlegR
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	ULEGR


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	FootL

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet	FootR

	End




	Event
		EName	2
		Type	Local
		At	FootR
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr

	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	:MotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

#####################################################################################
###Original
#
######################################################################################

	Event
		EName	2
		Type	Local
		At	FootR
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LlegR
	End


	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet UlegL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet LlegL
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet UlegL
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		#PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		#PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		#PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		#PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:MotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

End



End


