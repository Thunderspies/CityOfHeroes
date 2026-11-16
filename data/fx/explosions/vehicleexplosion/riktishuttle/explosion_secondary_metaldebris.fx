#########################################################
## Rikti Shuttle - Explosion/Death FX
#########################################################

FxInfo  

Lifespan 300





## METAL DEBRIS #######################################################

Condition
	On	Time
	Time 	0
	Repeat	3

	Event
		EName 	metal
		Type	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	1 0.5 1
			Scale			0.0625 0.0625 0.0625
			Spin			0.0 0.0 0.0
			SpinJitter		0.125 0.125 0.125
			Drag			0.005
			StartColor		0 64 16
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		Lifespan	90
		LifespanJitter	30
	End
End

Condition
	On	Time
	Time 	5
	Repeat	3

	Event
		EName 	metal
		Type	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	2 1 2
			Scale			0.0625 0.0625 0.0625
			Spin			0.0 0.0 0.0
			SpinJitter		0.125 0.125 0.125
			Drag			0.005
			StartColor		0 64 16
		End
		Geom	Metal_Curl
		Geom	Metal_Flat
		Lifespan	90
		LifespanJitter	30
	End
End

#######################################################################

End			