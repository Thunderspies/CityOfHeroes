#########################################################
##	FireRing
##
FxInfo
	
LifeSpan	100

Input  
	InpName	hips
End

Input  
	InpName	Root
End
#########################################################

Condition
	On 	Time
	Time 	0

	
	Event
		EName 	HipSpot
		Type	Local 
		At	hips
		geom	FireAura
						
	End

	Event
		EName 	base
		Type	Local 
		At	HipSpot
		altpiv	1
		geom	FX_RuneStreak
		bhvr	behaviors/Psionic_Spin.bhvr
		#Part1	:SurfaceScanGlow.part
						
	End

	Event
		EName 	baseExtention
		Type	Local 
		At	base
		altpiv	1
		
		Part1	:SurfaceScanGlow.part
						
	End
	
	Event
		EName 	OrbMist2
		Type	Local 
		At	root
		geom	FX_RuneStreak
		bhvr	behaviors/Psionic_Scale.bhvr
				
	End

	Event
		EName 	streaks
		Type	Local 
		At	OrbMist2
		altpiv  1
		part3	:GlowOrbTailWarm2.part
			
		
	End


	Event
		EName 	FloorMist2
		Type	Local 
		At	root
		Part1	:SurfaceScanGlow2.part
				
	End
	
End	

Condition
	On 	Time
	Time 	75

	Event
		EName 	streaks
		Type	Destroy 
				
	End

End

Condition
	On 	Time
	Time 	80

	Event
		EName 	all
		Type	Destroy 
				
	End

	Event
		EName 	Orb2
		Type	Destroy 
				
	End

End

End			