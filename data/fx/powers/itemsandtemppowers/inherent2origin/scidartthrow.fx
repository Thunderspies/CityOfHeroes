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
		Sound tranqdartthrow 60 60 .72
	End

	Event
		EName 	Prime
		Type	start 
		At	bendMystic
		geom	FX_Science_syringe
		bhvr	:SciDartProjectile.bhvr
		Magnet	T_Neck
		LookAt	T_Neck

	End
	
	Event
		EName 	Glow
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset 0 0 -.525
		End
		Part	:SciDartGlow.part
		Part	:SciDartTrail.part	
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