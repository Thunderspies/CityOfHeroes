#########################################################
##	template

FxInfo

LifeSpan	300
######  uprooted branch  #####################

Condition
	On 	Time
	Time 	32


	Event
		Type	Local  ##PositOnly ##(It needs to be local - steve)
		At	WepR
		bhvr	behaviors/Root_Grab.bhvr
		Geom    Root_uproot
		Lifespan	47

	End

End 


######  Dirt from ground #####################

Condition
	On 	Time
	Time 	30

	Event
		EName 	GroundBurst
		Type	Start 
		At	Mystic
		Part	:superStrengthDust1.part
		part	:superStrengthDust2.part
		Part	:HitRocks.part
		Part	:HitRocks.part
		Sound Groundburst 80 80 .96
		LifeSpan 1
	End


End


###### Projectile Root  #################################

Condition
	On 	Time
	Time 	79


	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Geom    Root_throw
		bhvr	behaviors/Root_toss.bhvr
		Magnet	Target
		LookAt	Target


	End


End

Condition
	On	PrimeHit


	Event
		Type	Start 
		At	T_Chest
		Part	:superStrengthDustHit1.part
		part	:superStrengthDust2Hit.part
		Part	:HitRocks.part
		Part	:HitRocks.part
		
		LifeSpan 20

	End

	Event
		Ename	Prime
		Type	Destroy
	End

End

End		