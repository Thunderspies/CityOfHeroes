#########################################################
##	Warburg Bomb - Nuke
##

FxInfo
	
lifespan	500

Condition

	On Time 
	Time 0
		
	Event
		Ename	Start
		Type	Start 
		At	Origin
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Start
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		50
			PhysForcePower		1250
			PhysForcePowerJitter	250
		End

		Lifespan	1

	End

	Event
		EName   RadiusFxScale
		Type	Local
		At	Start
		ChildFX	:MushroomCloud.fx
	End

	Event
		EName   Crater
		Type	Local
		At	Start
		Splat	StoneCracks.tga
		Bhvr	:CraterSplat.bhvr
		Lifespan	300
	End

	Event
		EName   CraterDebris
		Type	Local
		At	Start
		Splat	Mist.tga
		Bhvr	:NukeDebrisSplat.bhvr
		Lifespan	240
	End

	Event
		EName   CraterGlow
		Type	Local
		At	Start
		Splat	Glow.tga
		Bhvr	:NukeGlowSplat.bhvr
		Lifespan	120
	End

	Event
		EName 	Primex
		Type	Local 
		At	Start
		
		Bhvr	:CameraShake_Nuke.bhvr
		Part	:NovaTendrils.part
		Part	:InfernorockBits1.part
		Lifespan	25
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Start
		Bhvr	:PeaceBringerTest.bhvr
	End


	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    :Infernobubble.bhvr
		Part	:NovaSmoke.part
		Part	:NovaSmoke01.part
		Part	:NovaSmoke02.part
		Part	:NukeFlash.part
	End	

	Event
		Type	Posit
		At	Start
		Bhvr 	:BlastScaler.bhvr
		
		Geom	CosmicPeaceBringerShield
		LifeSpan	10
				
	End

	Event
		EName 	Prime8
		Type	Local 
		At	Start
		bhvr	:BlastRingScale.bhvr
		Part1   :Infernoflash.part
		Part2   :Infernoflash2.part
	End

End

Condition
	On 	Time
	Time 	2

	Event
		Type	Posit
		At	Start
		Bhvr 	:BlastScaler.bhvr
		
		Geom	CosmicPeaceBringerShield
		LifeSpan	10
				
	End

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		geom	MegaRing2
		bhvr	:BlastRingScale2a.bhvr
		LifeSpan		2
	End

End

Condition

	On 	Time
	Time 	5

	Event
		Type	Posit
		At	Start
		Bhvr 	:BlastScaler.bhvr
		
		Geom	CosmicPeaceBringerShield
		LifeSpan	10
				
	End

End

Condition

	On 	Time
	Time 	8

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno.part
		Lifespan	15

	End

End

Condition

	On 	Time
	Time 	11

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno1.part
		Part	:InfernorockBits2.part
		Lifespan	20

	End

End

Condition

	On 	Time
	Time 	14

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno2.part
		Lifespan	25

	End

End

Condition

	On 	Time
	Time 	18

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno3.part
		Part	:InfernorockBits3.part
		Lifespan	30

	End

End

Condition

	On 	Time
	Time 	23

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno4.part
		Lifespan	40

	End

End

End
