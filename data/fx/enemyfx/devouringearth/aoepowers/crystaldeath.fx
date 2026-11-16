
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

Flags    InheritAlpha
	

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
		Part	:BeaconCrystalCharcoal.part
		Part	:BeaconCrystalGreen.part
		Part	:BeaconCrystalRed.part
		Part	:BeaconCrystalPurple.part
		LifeSpan	30
	End
	
	
End


End
