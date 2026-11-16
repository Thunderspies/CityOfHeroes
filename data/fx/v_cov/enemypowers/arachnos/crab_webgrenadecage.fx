#########################################################
##	
##		WebGrenade Cage
##

#########################################################

FxInfo

LifeSpan	500


Condition
	On	Time
	Time	1
	
	Event
		EName	Sound
		Type	Start
		At	Chest

		bhvr	behaviors\FX_ArachnosCrab_CageDecay.bhvr
		ChildFX	Powers\Gadgets\WebCage.fx
		Lifespan 80
	End
End




End












		