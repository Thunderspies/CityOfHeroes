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
		Sound mutagenthrow 70 70 .7
	End

	Event
		EName 	Prime
		Type	start 
		At	bendMystic
		geom	FX_Mutant_Pod
		bhvr	:SciDartProjectile.bhvr
		Magnet	Target

	End
	
	Event
		EName 	Glow
		Type	Local
		At	Prime
		Part	:MutantPodGlow.part
		Part	:MutantPodTrail.part	
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