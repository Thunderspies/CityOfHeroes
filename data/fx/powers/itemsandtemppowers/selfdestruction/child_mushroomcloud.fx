#############################################################
## Child_MushroomCloud.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	MushroomCloudOffset
		Type	Start
		Bhvr	:CloudRiser.bhvr
		#Geom	GEO_FX_TestSphere
	End

	Event
		Ename	Trail
		Type	Local
		At	MushroomCloudOffset
		#Part	:McloudSmoke3.part
		Part	:MushroomCloud_Shaft1.part
		Part	:MushroomCloud_Shaft2.part
		LifeSpan 25
	End

	Event
		Type	Local
		At	MushroomCloudOffset
		Bhvr 	:McloudScaler.bhvr
		Geom	CosmicPeaceBringerShield
		LifeSpan 20
	End
End

Condition
	On 	Time
	Time 	6

	Event
		EName   RadiusFxScale
		Type	local
		At	MushroomCloudOffset
		Bhvr	:PeaceBringerTest.bhvr
	End

	Event
		Type	Start
		At	RadiusFxScale
		Bhvr	:BlastRingScale2b.bhvr
		Geom	MegaRing2
		LifeSpan 5
	End
End

Condition
	On 	Time
	Time 	6

	Event
		Type	Local
		At	MushroomCloudOffset
		Part	:MushroomCloud_Cap1.part
		Part	:MushroomCloud_Cap2.part
		Pmagnet	MushroomCloudOffset
		Lifespan 30
	End

	Event
		Type	Local
		At	MushroomCloudOffset
		Bhvr 	:McloudScaler.bhvr
		Geom	CosmicPeaceBringerShield
		LifeSpan 20
	End
End

Condition
	On 	Time
	Time 	16

	Event
		EName	ring
		Type	Start
		At	RadiusFxScale
		bhvr	:BlastRingScale2b.bhvr
		geom	MegaRing2
		LifeSpan 5
	End

	Event
		Type	Local
		At	MushroomCloudOffset
		Bhvr 	:McloudScaler.bhvr
		Geom	CosmicPeaceBringerShield
		LifeSpan 20
	End
End

#############################################################

End