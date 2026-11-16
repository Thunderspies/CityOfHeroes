#########################################################
##	Warburg Bomb - Nuke
##

FxInfo
	
lifespan	90

Condition

	On Time 
	Time 0
	
	Event
		Ename	Start
		Type	Start 
		Bhvr	:CloudRiser.bhvr
	End
	
	Event
		Ename	Trail
		Type	Local
		At	Start
		Part	:McloudSmoke3.part
		LifeSpan	30
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Column
		Type	Start
		bhvr    :CloudColumn.bhvr
		Part	:McloudSmoke2.part
		PMagnet	Start
		
		LifeSpan	30
	End		

	Event
		Type	Local
		At	Start
		Bhvr 	:McloudScaler.bhvr
		
		Geom	CosmicPeaceBringerShield
		LifeSpan	20
				
	End


End


Condition
	On 	Time
	Time 	6

	Event
		EName   RadiusFxScale
		Type	local
		At	Start
		Bhvr	:PeaceBringerTest.bhvr
	End

	Event
		EName 	Cloud
		Type	Local
		At	RadiusFxScale
		bhvr    :CloudColumn.bhvr
		Part	:McloudSmoke.part
		Pmagnet	Start
		Lifespan	45
	End		

	Event
		EName	ring
		Type	Start
		At	RadiusFxScale
		geom	MegaRing2
		bhvr	:BlastRingScale2b.bhvr
		LifeSpan		5
	End

	Event
		Type	Local
		At	Start
		Bhvr 	:McloudScaler.bhvr
		
		Geom	CosmicPeaceBringerShield
		LifeSpan	20
				
	End

	Event
		Type	Local
		At	Start
		Bhvr 	:McloudScaler.bhvr
		
		Geom	CosmicPeaceBringerShield
		LifeSpan	20
				
	End

End

Condition
	On 	Time
	Time 	16

	Event
		EName	ring
		Type	Start
		At	RadiusFxScale
		geom	MegaRing2
		bhvr	:BlastRingScale2b.bhvr
		LifeSpan		5
	End

	Event
		Type	Local
		At	Start
		Bhvr 	:McloudScaler.bhvr
		
		Geom	CosmicPeaceBringerShield
		LifeSpan	20
				
	End

	Event
		Ename	Top
		Type	Local 
		At	Start
		BhvrOverride
			PositionOffset 0 0 0
		End
		Part	:McloudSmoke4.part
		Lifespan 20
	End


End


Condition
	On 	Time
	Time 	36

	Event
		EName	ring
		Type	Start
		At	RadiusFxScale
		geom	MegaRing2
		bhvr	:BlastRingScale2b.bhvr
		LifeSpan		5
	End

End

Condition
	On 	Time
	Time	60

	Event
		EName 	Cloud
		Type	Destroy
	End

End	

End
