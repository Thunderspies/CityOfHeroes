
#########################################################
##	
##
FxInfo

Input  
	InpName	Root
End

Input  
	InpName	Origin
End	
	

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
End

Condition
	On 	TriggerBits
	TriggerBits	Death

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

	Event
		Type	local
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:RockScatter01.part
		Part	:RockScatter02.part
		Part	:StalagtiteDustHit.part
		Part	:StalagtiteDustHit2.part
		LifeSpan	30
	End	
	
	Event
		Ename	Physics
		Type	Start
		At	chest

		childFx	Explosions\RockExplosion\Small_RockExplosion.Fx

	End

End


End
