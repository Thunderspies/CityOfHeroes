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
			PhysForcePower		1000
			PhysForcePowerJitter	200
		End

		Lifespan	1

	End

	Event
		EName   Cloud
		Type	Local
		At	Start
		ChildFX	:ChemCloud.fx
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
		EName   CraterGlow
		Type	Local
		At	Start
		Splat	Glow.tga
		Bhvr	:ChemGlowSplat.bhvr
		Lifespan	60
	End

	Event
		EName 	Primex
		Type	Local 
		At	Start
		
		Bhvr	:CameraShake_BioBomb.bhvr
		Part	:NovaTendrils.part
		Lifespan	20
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
		Part	:ChemFlash.part
		Lifespan	20
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
	Time 	0

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno.part
		Lifespan	20

	End

End

Condition

	On 	Time
	Time 	5

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno1.part
		Lifespan	25

	End

End

Condition

	On 	Time
	Time 	10

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno2.part
		Lifespan	30

	End

End

Condition

	On 	Time
	Time 	15

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno3.part
		Lifespan	35

	End

End

Condition

	On 	Time
	Time 	20

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:Inferno4.part
		Lifespan	40

	End

End


Condition

	On 	Time
	Time 	20


	Event
		EName   Splash
		Type	Local
		At	Start
		ChildFX	:ChemSplash.fx
		Lifespan	100
	End

	Event
		EName   Splash2
		Type	Local
		At	Start
		ChildFX	:ChemSplash.fx
		Lifespan	160
	End

	Event
		EName 	crime
		Type	Local
		At	Start
		
		Part2	:ChemMist.part
		Lifespan	240

	End

End

End
