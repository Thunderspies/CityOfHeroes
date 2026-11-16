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
		Sound knifetwirly 70.0 70.0 .8
	End

	Event
		EName 	Prime
		Type	Start
		At	bendMystic
		geom	FX_Natural_Kunai
		bhvr	:NatKnifeProjectile.bhvr
		Magnet	Target

	End
	
	Event
		EName 	Streak1
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset 0 0 .15
		End
		Part	:NatKnifeTrail.part	
	End
	
	Event
		EName 	Streak2
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset 0 0 -.15
		End
		Part	:NatKnifeTrail2.part	
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