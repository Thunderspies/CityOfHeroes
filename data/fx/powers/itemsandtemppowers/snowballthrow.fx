#########################################################
##	jack
##

FxInfo



Flags    InheritAlpha


LifeSpan		200

Condition
	On 	Time
	Time 	0

	Event
		EName 	RockinHand
		Type	local
		At	WepR
		Geom	snowball
		Sound snowballpickup 40 40 .46
		LifeSpan	60
	End
End

Condition
	On 	Time
	Time 	60

	Event
		EName 	Prime
		Type	start
		At	WepR
		Geom	snowball
		Bhvr	:rockprojectile.bhvr
		Magnet	T_Chest
		Sound smallrockthrow 60 60 .28
		#Part	:SnowballCore.part
		Part	:icebolttail1.part
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