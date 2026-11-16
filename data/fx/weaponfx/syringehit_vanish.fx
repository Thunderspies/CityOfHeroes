#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

LifeSpan 300

Condition
	On Time
	Time 5

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			FadeOutLength	10
		End
		Flags	AdoptParentEntity
		Part1	:Syringeflash.part
		Part2	:SyringeflashStar.part
		part	:SyringeSpark.part
		Sound syringe5 40 40 .65
		Lifespan 1
	End

End

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

End



End

