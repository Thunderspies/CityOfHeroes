#########################################################
##	jack
##

FxInfo


Flags    InheritAlpha


LifeSpan		200

Condition
	On 	Time
	Time 	20

	Event
		EName 	RockinHand
		Type	local
		At	mystic
		Bhvr	:LargeSnowball.bhvr
		Geom	snowball
		Sound smallrockpickup 40 40 .4
		LifeSpan	40
	End
End

Condition
	On 	Time
	Time 	55

	Event
		EName 	Prime
		Type	startPositOnly
		At	mystic
		Bhvr	:LargeSnowballProjectile.bhvr
		Geom	snowball
		Magnet	T_Chest
		Sound rockthrow 60 60 .28
		
		Part	:LargeIceBoltTail1.part
		Part	:IceBoltSnowtrail.part
		Part	:IceBoltMistTrail.part
		
	End
End

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
		
	End
End

End		