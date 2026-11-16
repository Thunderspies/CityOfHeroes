#############################################################
## BitterFreezeRay.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

	Condition
		On	Time
		Time	0

		Event
			EName	RingGeo
			At 	Root
			Type 	Local
			Bhvr 	:Singularity_Ring_20ft_Grow.bhvr

			BhvrOverride
				PositionOffset	0 4 0
				Alpha 		255
				PYRRotateJitter 15 0 0
				StartColor 	50 0 90
			End
			Geom FX_Singularity_Ring
			Lifespan 50
		End
	End

	Condition
		On	Cycle
		Time	25

		Event
			EName	RingGeo
			At 	Root
			Type 	Local
			Bhvr 	:Singularity_Ring_20ft_Grow.bhvr

			BhvrOverride
				PositionOffset	0 4 0
				Alpha 		255
				PYRRotateJitter 15 0 0
				StartColor 	50 0 90
			End
			Geom FX_Singularity_Ring
			Lifespan 50
		End
	End

#############################################################

End