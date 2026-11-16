#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local
		At	chest
		Sound miasma_loop 55 55 .55
	End

	Event
		ENAME   Tendrills
		Type	Local
		At	chest
		Part	:ChillOfNightTendril.part
		Lifespan	30
	End

	Event
		ENAME   Tendrills
		Type	Local
		At	chest
		Part	:Blast_Additive.part
		Lifespan	7
	End


End

Condition
	On	Time
	Time	10

	Event
		ENAME   Tendrills
		Type	PositOnly
		At	chest
		Part	:ChillOfNightTendril.part
		Part	:ChillOfNightTendril_Additive.part
		Part	:ChillOfNightTendril_Small_Additive.part
		Part	:DarkBlastHitPuddle2.part
		Lifespan	30
	End

	Event
		Type	PositOnly
		At	Chest
		Part	:Rays.part
	End

	Event

		EName 	Hit
		Type	Local
		At	Root
		ChildFX	:ObsidionArmor.fx
	End
End

End


