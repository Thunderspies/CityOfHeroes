#########################################################
##	template

FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Root
End

Input  
	InpName	Hair
End

Input  
	InpName	Eyes
End

Input  
	InpName	Neck
End

Input  
	InpName	Chest
End

Input  
	InpName	SpadR
End

Input  
	InpName	SpadL
End

Input  
	InpName	UarmR
End

Input  
	InpName	UarmL
End

Input  
	InpName	LarmR
End

Input  
	InpName	LarmL
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

Input  
	InpName	Hips
End

Input  
	InpName	UlegR
End

Input  
	InpName	UlegL
End

Input  
	InpName	LlegR
End

Input  
	InpName	LlegL
End

Input  
	InpName	FootR
End

Input  
	InpName	FootL
End

Input  
	InpName	ToeL
End

Input  
	InpName	ToeR
End

Input
	InpName Origin
End

Lifespan 200



#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound gravityzap2 80 80 .75
	End
End

Condition
	On	Time
	Time	10
	
	Event	
		ENAME	NeckNode
		Type	Local
		At	Neck
		Sound gravityhit 80 80 .9
	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:GCBodyGlow.part
		Pmagnet NeckNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	ChestNode
		Type	Local
		At	Chest
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:GCBodyGlow.part
		Pmagnet ChestNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LarmRNode
		Type	Local
		At	LarmR
	
	End

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:GCBodyGlow.part
		Pmagnet LarmRNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:GCBodyGlow.part
		Pmagnet LarmRnode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LarmLNode
		Type	Local
		At	LarmL
	
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:GCBodyGlow.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:GCBodyGlow.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End

End

#########################################################
################## BODY GLOW MIST #######################
#########################################################


	
Condition
	On	Time
	Time	0
	
	Event	
		ENAME	NeckNode
		Type	Local
		At	Neck
	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:GCBodyGlowMist.part
		Pmagnet NeckNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	ChestNode
		Type	Local
		At	Chest
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:GCBodyGlowMist.part
		Pmagnet ChestNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LarmRNode
		Type	Local
		At	LarmR
	
	End

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:GCBodyGlowMist.part
		Pmagnet LarmRNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:GCBodyGlowMist.part
		Pmagnet LarmRnode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LarmLNode
		Type	Local
		At	LarmL
	
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:GCBodyGlowMist.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:GCBodyGlowMist.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End

End
	
Condition
	On	Time
	Time	0

	Event	
		ENAME	HandLOrbit
		Type	Local
		At	Chest
		part1	:GCHandOrbits.part
		Pmagnet Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 40
	
	End

End

#########################################################
###################### VORTEX ###########################
#########################################################


Condition
	On	Time
	Time	0

	Event	
		ENAME	Prime
		Type	Start
		At	Target
		Geom	RoottoChestAdjustment
	End

	Event	
		ENAME	VortexGlows
		Type	Local
		At	Prime
		AltPiv	1
		part	:GCVortexGlowsOut.part
		part	:GCVortexSteamOut.part
		Lifespan 140
	End
	
	Event
		Ename	ExpulsionForce
		Type	Local
		At	Prime
		AltPiv	1
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		35
			PhysForcePower		30
			PhysForcePowerJitter	10
		End

		Lifespan	100
	End
End

Condition
	On	Time
	Time	20

	Event	
		ENAME	Vortex
		Type	Local
		At	Prime
		AltPiv	1
		part1	:GCVortexOutFast.part
		Lifespan 100
	End

End

Condition
	On	Time
	Time	70
		
	Event	
		ENAME	VortexFlash
		Type	Start
		At	Prime
		AltPiv	1
		part1	:GCFlashLrg.part
		Lifespan 10
	End

End

End		