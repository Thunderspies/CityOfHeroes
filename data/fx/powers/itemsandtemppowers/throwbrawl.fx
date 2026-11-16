#########################################################
##	jack
##

FxInfo



Flags    InheritAlpha


LifeSpan		200



Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	start
		At	BendMystic
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