#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	chest
End

#LifeSpan	40

########################################################

Condition
	On	Time
	Time	2

	Event
		Type	Local 
		At	chest
		Sound heal1_loop 60 60 .45
		bhvr	behaviors/soundFade1.bhvr
		Flags	PowerLoopingSound
		Lifespan 130

	End

End


Condition
	On 	Time
	Time 	2
	
	Event
		EName	dummy
		Type	Local 
		At	chest
		
		Part1	:core4.part
		Part2	:Electrons.part
		Part3	:StarStreaks1.part
		Part4	:StarStreaksDown1.part
		Part5	:StarStreaksRightc1.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		Sound regen4 60 60 1.0
	End

	Event
		EName	dummy2
		Type	Local 
		At	chest
		
		Part1	:tealmist.part
		Part2	:CoreRings.part
		Part3	:StarStreaksRightb1.part
		Part4	:StarStreaksLeftb1.part
		Part5	:StarStreaksLeftc1.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		Sound regen3 60 60 .8

	End

	Event
		EName	dummy2
		Type	Local 
		At	chest
		
		Part1	:SparkliesTrail.part
	End

End


End
##################################

