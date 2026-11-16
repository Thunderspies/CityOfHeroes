#########################################################
##	template

FxInfo


Flags InheritAnimScale

LifeSpan 300

######## Sound  ###########################

Condition
	On	Time
	Time	13

	Event	
		Type	Local
		At	WepR
		Sound quills1 50 50 .8
	End
End


Condition
	On	Time
	Time	25

	Event	
		Type	Local
		At	WepR
		Sound thornbarrage2 80 80 .96
	End
End
###############################################################################


Condition
	On	Time
	Time	25
	

	Event
		EName	Prime
		Type	start
		At	WepR
		Geom	FX_SilverSpike02
		Geom	FX_SilverSpike01
		Geom	FX_SilverSpike02
		Bhvr	:ProjectileSilverDart.bhvr
		Magnet	Target
		LookAt	Target
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part
		Part	:SpikeTrailSmoke2.part
		Part	:silverSparkTrail2.part
		Part	:SpikeTrailSmoke2.part
		Part	:silverSparkTrail2.part
	End

End

Condition
	On	Time
	Time	25
	Repeat	10
	RepeatJitter	2

	Event
		EName	Secondary
		Type	Local
		At	Prime
		Geom	FX_SilverSpike02
		Geom	FX_SilverSpike01
		Geom	FX_SilverSpike02
		Bhvr	:SpikeBarrageSecondary.bhvr
		Part	:SpikeTrailSmoke.part
	End

End

Condition
	On	Time
	Time	32

	Event
		EName	Prime1
		Type	start
		At	WepL
		Geom	FX_SilverSpike02
		Geom	FX_SilverSpike01
		Geom	FX_SilverSpike02
		Bhvr	:ProjectileSilverDart.bhvr
		Magnet	Target
		LookAt	Target
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part
		Part	:SpikeTrailSmoke2.part
		Part	:silverSparkTrail2.part
		Part	:SpikeTrailSmoke2.part
		Part	:silverSparkTrail2.part
	End

End

Condition
	On	Time
	Time	32
	Repeat	10
	RepeatJitter	2

	Event
		EName	Secondary1
		Type	Local
		At	Prime1
		Geom	FX_SilverSpike02
		Geom	FX_SilverSpike01
		Geom	FX_SilverSpike02
		Bhvr	:SpikeBarrageSecondary.bhvr
		Part	:SpikeTrailSmoke.part	
	End

End

Condition
	On	Time
	Time	35
	Event
		EName	Prime2
				Type	start
		At	WepR
		Geom	FX_SilverSpike02
		Geom	FX_SilverSpike01
		Geom	FX_SilverSpike02
		Bhvr	:ProjectileSilverDart.bhvr
		Magnet	Target
		LookAt	Target
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part
		Part	:SpikeTrailSmoke2.part
		Part	:silverSparkTrail2.part
		Part	:SpikeTrailSmoke2.part
		Part	:silverSparkTrail2.part
	End

End

Condition
	On	Time
	Time	35
	Repeat	10
	RepeatJitter	2

	Event
		EName	Secondary2
		Type	Local
		At	Prime2
		Geom	FX_SilverSpike02
		Geom	FX_SilverSpike01
		Geom	FX_SilverSpike02
		Bhvr	:SpikeBarrageSecondary.bhvr
		Part	:SpikeTrailSmoke.part	
	End

End


Condition
	On	Time
	Time	42
	Event
		EName	Prime3
		Type	start
		At	WepL
		Geom	FX_SilverSpike02
		Geom	FX_SilverSpike01
		Geom	FX_SilverSpike02
		Bhvr	:ProjectileSilverDart.bhvr
		Magnet	Target
		LookAt	Target
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part
		Part	:SpikeTrailSmoke2.part
		Part	:silverSparkTrail2.part
		Part	:SpikeTrailSmoke2.part
		Part	:silverSparkTrail2.part
	End

End

Condition
	On	Time
	Time	42
	Repeat	10
	RepeatJitter	2

	Event
		EName	Secondary3
		Type	Local
		At	Prime3
		Geom	FX_SilverSpike02
		Geom	FX_SilverSpike01
		Geom	FX_SilverSpike02
		Bhvr	:SpikeBarrageSecondary.bhvr
		Part	:SpikeTrailSmoke.part	
	End

End


Condition
	On	Time
	Time	49

	Event
		EName	Prime4
		Type	start
		At	WepL
		Geom	FX_SilverSpike02
		Geom	FX_SilverSpike01
		Geom	FX_SilverSpike02
		Bhvr	:ProjectileSilverDart.bhvr
		Magnet	Target
		LookAt	Target
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part
		Part	:SpikeTrailSmoke2.part
		Part	:silverSparkTrail2.part
		Part	:SpikeTrailSmoke2.part
		Part	:silverSparkTrail2.part
	End

End

Condition
	On	Time
	Time	49
	Repeat	10
	RepeatJitter	2

	Event
		EName	Secondary4
		Type	Local
		At	Prime4
		Geom	FX_SilverSpike02
		Geom	FX_SilverSpike01
		Geom	FX_SilverSpike02
		Bhvr	:SpikeBarrageSecondary.bhvr
		Part	:SpikeTrailSmoke.part
	End

End


Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End

End

Condition
	On 	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy
		

	End

End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
		

	End

End

Condition
	On 	Prime3Hit

	Event
		EName 	Prime3
		Type	Destroy
		

	End

End

Condition
	On 	Prime4Hit

	Event
		EName 	Prime4
		Type	Destroy
		

	End

End

End