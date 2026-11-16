#########################################################
##	onfire
##
FxInfo  

#LifeSpan	120

Condition
	On 	triggerbits
	Triggerbits	death
	
################ If you want the guy to turn invisible right as he dies AND play particles, do this on triggerbits death
################ (you can't set TicksToLingerAfterDeath to less than about 15 if you want to have a 
################ deathFx because he'll be dead before the fx happens.)
	Event
		EName 	Prime
		Type	Local
		At	Origin 
		Bhvr	behaviors/vanish.bhvr
		Flags	AdoptParentEntity
	End

	Event
		EName 	Prime
		Type	Destroy
	End
#################### End do this

	
	Event
		Type	Local 
		At	chest
		
		ChildFx	Explosions\WoodExplosion\Large_WoodExplosion.Fx
		#LifeSpan	200

	End

End

End