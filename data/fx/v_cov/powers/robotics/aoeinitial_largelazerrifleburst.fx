#########################################################
##	
##

FxInfo

LifeSpan  300

##################################

Condition
	On 	Time
	Time 	42


	Event
		ENAME   p1
		Type	Local 
		At	WepR
		Geom	centerdummy
	End

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 1
		
		Part	:LargeVillianSniper_Tracer.part
		Part	:LargeVillianSniper_Tracer.part
		Part	:LargeVillianSniper_Tracer.part

		Part	:LargeVillianSniper_Head.part
		
		Part	:LargeEnergyBlast.part
		Part	:LargeEnergyBlastHead.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	Target
		Lookat	Target

	End
	
End

##################################


Condition
	On	PrimeHit
	
	Event
		EName 	Rings
		Type	local
		At	T_Root

		BHVR	Behaviors/spinrandom.bhvr
		Part	:HitRingflashthin.part
		Part	:HitRingflashthin2.part
		Part	:HitRingflash.part
		LifeSpan	3
	End

	Event
		EName 	Rings
		Type	local
		At	T_Root

		BHVR	Behaviors/spinrandom.bhvr
		Part	:HitRingflashthin.part
		Part	:HitRingflashthin2.part
		Part	:HitRingflash.part
		LifeSpan	3
	End
	
	Event
		EName 	Rings
		Type	local
		At	T_Root

		BHVR	Behaviors/spinrandom.bhvr
		Part	:HitRingflashthin.part
		Part	:HitRingflashthin2.part
		Part	:HitRingflash.part
		LifeSpan	3
	End

	Event
		Ename	Prime
		Type	Destroy
	End

End

End				