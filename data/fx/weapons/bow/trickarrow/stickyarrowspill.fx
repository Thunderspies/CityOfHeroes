#############################################################
## StickyArrowSpill.fx
#############################################################

FxInfo

LifeSpan 900

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Root
		Sound miasma_loop 80 80 0.22
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME	Flash
		Type	Local
		At	Root
		Bhvr    Behaviors\Fade.bhvr
		ChildFx	:GluePrimeHitChild.fx
		LifeSpan 810
	End
End

#############################################################

End