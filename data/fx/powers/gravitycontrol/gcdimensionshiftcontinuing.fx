#########################################################
##	template

FxInfo



Condition
	On	Time
	Time	75
	Event
		Type	Local
		At 	origin
		Sound glow_loop 80 80 .45
	End

End

Condition
	On	Time
	Time	75
	Event
		Type	Local
		At 	origin
		Sound glow6_loop 80 80 .25
	End

End

Condition
	On	Time
	Time	0
	
#	Event	
#		ENAME	Alpha
#		Type	Local
#		At	Origin
#		
#		BhvrOverride
#
#			FadeInLength		100
#			FadeOutLength		100
#			Alpha			180
#		
#		End
#
#		Flags	AdoptParentEntity
#
#	
#	End
	
	Event	
		ENAME	Vortex
		Type	Local
		At	Hips
		part1	:GCVortexSteamSmall.part
		LifeSpan 80
	End

End

Condition
	On	Time
	Time	5
		
	Event	
		ENAME	Vortex
		Type	Local
		At	Hips
		part	:GCVortexSmall.part
		#Bhvr	Behaviors/GCVortexSpin.bhvr
		LifeSpan 60
	End

End

Condition
	On	Time
	Time	10
		
	Event	
		ENAME	Vortex
		Type	Local
		At	Hips
		part1	:GCVortexGlowsSmall.part
		Pmagnet Hips
		#Bhvr	Behaviors/GCVortexSpin.bhvr
		#Lookat	Camera
		LifeSpan 80
		Sound gravityhit 80 80 .8
		
	End

End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	30
	
	Event	
		ENAME	NeckNode
		Type	Local
		At	Neck
	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:GCBodyGlow.part
		Pmagnet NeckNode
		#Bhvr	Behaviors/GravityControlFade1.bhvr
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
		#Bhvr	Behaviors/GravityControlFade1.bhvr
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
		#Bhvr	Behaviors/GravityControlFade1.bhvr
		Lifespan 0
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:GCBodyGlow.part
		Pmagnet LarmRnode
		#Bhvr	Behaviors/GravityControlFade1.bhvr
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
		#Bhvr	Behaviors/GravityControlFade1.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:GCBodyGlow.part
		Pmagnet LarmLNode
		#Bhvr	Behaviors/GravityControlFade1.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LleglNode
		Type	Local
		At	Llegl
	
	End

	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part1	:GCBodyGlow.part
		Pmagnet LleglNode
		#Bhvr	Behaviors/GravityControlFade1.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:GCBodyGlow.part
		Pmagnet LleglNode
		#Bhvr	Behaviors/GravityControlFade1.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LlegRNode
		Type	Local
		At	LlegR
	
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part1	:GCBodyGlow.part
		Pmagnet LlegRNode
		#Bhvr	Behaviors/GravityControlFade1.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:GCBodyGlow.part
		Pmagnet LlegRNode
		#Bhvr	Behaviors/GravityControlFade1.bhvr
		Lifespan 0
		
	End


End

	
End	

	