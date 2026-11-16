#########################################################
##	onfire
##
FxInfo  

#LifeSpan	150

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
		Bhvr	behaviors/vanishSlow2.bhvr
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
		
		BhvrOverride	
			startJitter		1 1 1
			PositionOffset		0 8 0
		End


		ChildFx	Explosions\StoneStatueExplosions\SmallStatueExplosion.Fx
		#LifeSpan	200

	End

	Event
		Type	Local 
		At	chest
		
		BhvrOverride	
			startJitter		1 1 1
			PositionOffset	0 4 0
		End


		ChildFx	Explosions\StoneStatueExplosions\SmallStatueExplosionLow.Fx
		#LifeSpan	200

	End
	
	Event
		Type	Local 
		At	chest
		
		BhvrOverride	
			startJitter		1 1 1
			PositionOffset	0 6 0
		End


		ChildFx	Explosions\StoneStatueExplosions\SmallStatueExplosionMiddle.Fx
		#LifeSpan	200

	End

End

End