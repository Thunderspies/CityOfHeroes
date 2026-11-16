#############################################################
## BitterFreezeRay.fx
#############################################################

FxInfo
Lifespan 300

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPB_FrigidSlash_Hit_01 100 100 .8
	End
End
Condition
	On	Time
	Time	0

	Event
		Ename	IcicleCentralAnchor
		Type	Local
		At	Root
		Bhvr	behaviors\CameraShakeStomp.bhvr
		Lifespan 3
	End

	Event
		Ename	IcicleCentral
		Type	Local
		At	Root
		Bhvr	:FrigidGrasp_Crystal_Central.bhvr
		BhvrOverride
			StartColor	0 60 60
		End
		Geom	FX_FrigidGrasp
		Lifespan 180
	End

	Event
		Ename	IcicleCentral
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 1.0 0
		End
		Geom	FX_FrigidGrasp
		Part	:FrigidGrasp_DustPillar_Up.part
		Lifespan 180
	End
End

Condition
	On	Time
	Time	1

	Event
		Ename	IcicleCentral
		Type	Local
		At	Root
		Bhvr	:FrigidGrasp_Crystal_Large.bhvr
		BhvrOverride
			Alpha		255
			StartJitter	1 0 1
			StartColor	0 60 60
		End
		Geom	FX_FrigidGrasp
		Lifespan 180
	End
End

Condition
	On	Time
	Time	2
	Random	1

	Event
		Ename	IcicleCentral2
		Type	Local
		At	Root
		Bhvr	:FrigidGrasp_Crystal_Small.bhvr
		BhvrOverride
			StartColor	0 60 60
		End
		Geom	FX_FrigidGrasp_Small
		Lifespan 180
	End

	Event
		Ename	IcicleCentral3
		Type	Local
		At	Root
		Bhvr	:FrigidGrasp_Crystal_Small.bhvr
		BhvrOverride
			StartColor	10 80 100
		End
		Geom	FX_FrigidGrasp_Small2
		Lifespan 180
	End
End

Condition
	On	Time
	Time	3
	Random	1

	Event
		Ename	IcicleCentral4
		Type	Local
		At	Root
		Bhvr	:FrigidGrasp_Crystal_Small.bhvr
		BhvrOverride
			StartColor	10 80 100
		End
		Geom	FX_FrigidGrasp_Small
		Lifespan 180
	End

	Event
		Ename	IcicleCentral5
		Type	Local
		At	Root
		Bhvr	:FrigidGrasp_Crystal_Small.bhvr
		BhvrOverride
			StartColor	10 80 100
		End
		Geom	FX_FrigidGrasp_Small2
		Lifespan 180
	End
End

Condition
	On	Time
	Time	180

	Event
		Ename	IcicleCentral5
		Type	Local
		At	Root
		ChildFX	:Soulshatter_Shards.fx
		Sound BP_FrigidGrasp_Hit_01 100 100 .8
		Lifespan 90
	End
End

###########################################################

End