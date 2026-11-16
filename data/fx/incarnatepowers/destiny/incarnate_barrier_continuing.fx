#########################################################
## IncarnatePower  - Barrier
#########################################################

FxInfo

########################################################

Input
	Inpname	Hips
End

########################################################



Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	Incarnate_Barrier_Continuing_Loop 100 100 .3
	End


	Event
		Type	PositOnly
		At	Hips
		Bhvr	:Incarnate_BarrierSphere.bhvr
		BhvrOverride


			FadeInLength		6
			Alpha			50
			FadeOutLength		50
			Spin			0 0 0
			Scale 			0.75 0.75 0.75
			PyrRotate 		0.0 0.0 0.0
			PyrRotateJitter 	0.0 0.0 0.0
			TintGeom 		1
		End
		Geom	Incarnate_Barrier_ShieldA_Small
     		Part2	:Incarnate_BarrierLightningRing_Small.part
		Part	:Incarnate_BarrierLightning_Perimeter.part
		CThresh 	0.00001
	End
End

#########################################################

End