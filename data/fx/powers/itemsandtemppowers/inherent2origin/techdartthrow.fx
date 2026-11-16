#########################################################
##	
##

FxInfo

LifeSpan  60

##################################



Condition
	On 	Time
	Time 	15

	Event
		Type	Local	
		At	Origin
		Sound tazerdartthrow2 70 70 .79
	End

	Event
		EName 	Prime
		Type	start 
		At	bendMystic
		geom	FX_Tech_Dart
		bhvr	:TechDartProjectile.bhvr
		Magnet	T_Neck
		LookAt	T_Neck

	End
	
	Event
		EName 	Prime2
		Type	start 
		At	bendMystic
		geom	FX_Tech_Dart
		bhvr	:TechDartProjectile.bhvr
		Magnet	T_Neck
		LookAt	T_Neck

	End
	
	Event
		EName 	Glow
		Type	Local
		At	Prime
		bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0 -0.25
		End
		Part	:TechDartGlow.part
		Part	:TechDartSpark.part
		Part	:TechDartTrail.part
		Pmagnet	Prime
	End
	
	Event
		EName 	Glow2
		Type	Local
		At	Prime2
		bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0 -0.25
		End
		Part	:TechDartGlow.part
		Part	:TechDartSpark.part
		Part	:TechDartTrail.part
		Pmagnet	Prime2	
	End


End


Condition
	On 	Time
	Time	40

	
	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	Prime2
		Type	Destroy
	End


End

End							