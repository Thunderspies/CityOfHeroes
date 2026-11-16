#########################################################
##	
##

FxInfo

LifeSpan  90

##################################



Condition
	On 	Time
	Time 	15

	Event
		Type	Local	
		At	Origin
		Sound magicamuletthrow 60 60 .7
	End

	Event
		EName 	Prime
		Type	start 
		At	bendMystic
		geom	FX_Magic_Crystal
		bhvr	:MagicCharmProjectile.bhvr
		Magnet	Target

	End
	
	Event
		EName 	Glow
		Type	Local
		At	Prime
		Part	:MagicCharmGlow.part
		Part	:MagicCharmTrail.part	
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