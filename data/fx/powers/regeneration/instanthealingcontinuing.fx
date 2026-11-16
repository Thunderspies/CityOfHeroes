#########################################################
##	Fireweapon
########################################################
FxInfo

########################################################

Condition
	On 	Time
	Time 	15
	
	Event
		Type	Local 
		At	chest
		Sound heal1_loop 60 60 .45
		bhvr	behaviors/soundFade1.bhvr
		Flags	PowerLoopingSound
		Lifespan 100
	End

End





Condition
	On 	Time
	Time 	35
	
	Event
		EName	dummy2
		Type	Local 
		At	chest
		Part2	:SparkliesTrail.part
#		Part1	:Ring01.part
		
	End

End

Condition
	On 	Time
	Time 	15
	
	Event
		EName	dummy
		Type	Local 
		At	chest
		
		Part1	:core4.part
		Part2	:Electronsfast.part
		Part3	:StarStreaks.part
		Part4	:StarStreaksDown.part
		Part5	:StarStreaksRightc.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	dummy2
		Type	Local 
		At	chest
		
		Part1	:Integration01Fast.part
		#Part2	:CoreRings.part
		Part3	:StarStreaksRightb.part
		Part4	:StarStreaksLeftb.part
		Part5	:StarStreaksLeftc.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	
End
#########################################################

Condition
	On	Time
	Time	0
	
	Event
		EName	Ring
		Type	Local
		At	Root
#		part1	Powers/Ability/StaminaRingUp.part
		Sound instantheal 70 70 .8
		LifeSpan	300
	End

End

#Condition
#	On	Time
#	Time	5
#	
#	Event
#		EName	Ring
#		Type	Local
#		At	Root
#		part1	Powers/Ability/StaminaRingUp.part
#		LifeSpan	60
#	End
#
#End

#Condition
#	On	Time
#	Time	20
#	
#	Event
#		EName	Ring
#		Type	Local
#		At	Root
#		part1	Powers/Ability/StaminaRingUp.part
#		LifeSpan	60
#	End
#
#End
#
Condition
	On	Time
	Time	3
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	:RegenBodyGlows.part
		
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:RegenMotionGlows.part
		PMagnet Knee1
		
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:RegenMotionGlows.part
		PMagnet FootL

	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:RegenBodyGlows.part
		
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:RegenMotionGlows.part
		PMagnet Kneer
		
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:RegenMotionGlows.part
		PMagnet FootR
		
	End


End

End
##################################

