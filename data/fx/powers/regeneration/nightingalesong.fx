#########################################################
##	Fireweapon
########################################################
FxInfo

Lifespan 85
########################################################

Condition
	On 	Time
	Time 	62
	
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
	Time 	62
	
	Event
		EName	dummy
		Type	Local 
		At	chest
		
		Part1	:MomentofGloryCollapse.part
		
	End

End

Condition
	On 	Time
	Time 	25
	
	Event
		EName	dummy
		Type	Local 
		At	chest
		
		Part1	:Healing01.part
		Sound ScreamPBAOE 88 88 .8
		
	End

	Event
		EName	dummy
		Type	Local
		At	chest
		
		Part1	:core.part
		Part2	:Electronsfast.part
		Part3	:StarStreaks1.part
		Part4	:StarStreaksDown1.part
		Part5	:StarStreaksRightc1.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	dummy2
		Type	Local 
		At	chest
		
		Part1	:Integration01Fast.part
		Part2	:CoreRings.part
		Part3	:StarStreaksRightb1.part
		Part4	:StarStreaksLeftb1.part
		Part5	:StarStreaksLeftc1.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	dummy3
		Type	Local 
		At	chest
		
		Part1	:RegenLightRays.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		
	End

	Event
		EName	dummy2
		Type	Local 
		At	chest
		
		
		Part2	:MomentOfGlorySparkliesTrail.part
		
	End

	Event
		EName	dummy2
		Type	Local 
		At	Root
		
		Part1	:MomentOfGlorySparkliesRising.part
		
	End

	
End

Condition
	On	Time
	Time	53
	
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

