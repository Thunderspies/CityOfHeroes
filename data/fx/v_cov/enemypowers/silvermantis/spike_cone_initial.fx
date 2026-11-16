#########################################################
##	template

FxInfo


Flags InheritAnimScale

LifeSpan 300

######## Sound  ###########################

Condition
	On	Time
	Time	36

	Event	
		Type	Local
		At	WepR
		Sound thornytoss2 70 70 .7
	End
End

########  projectile cone  #########################  

Condition
	On 	Time
	Time 	35

	Event
		EName 	Start
		Type	Start
		At	WepL
		Lookat	T_hips
	End

	
	Event
		EName 	prime
		Type	start
		At	Start
		Bhvr	:ProjectileSilverDart.bhvr
		Geom	FX_SilverSpike02
		Geom	FX_SilverSpike01
		Geom	FX_SilverSpike02
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail2.part
		Magnet	T_hips
		Lookat	T_hips


		Lifespan	17
	End

End

Condition
	On 	Time
	Time 	35
	Repeat	10
	RepeatJitter	2
		
	Event
		EName 	Spread
		Type	Local
		At	Start
		Bhvr	:SpikeConeSpread.bhvr
		Geom	FX_SilverSpike02
		Geom	FX_SilverSpike01
		Geom	FX_SilverSpike02
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
		
		Lifespan	15
	End
	
End

Condition
	On 	PrimeHit

	Event
		Ename 	All
		Type	Destroy
	End

End

End		