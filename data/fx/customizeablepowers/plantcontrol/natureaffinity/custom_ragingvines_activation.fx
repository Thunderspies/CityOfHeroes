#
FxInfo


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	NA_RagingTempest_01 100 100 .8
	End
End

####################################################################################

	Condition
		On Triggerbits
		Triggerbits MALE
		DoMany 1

		Event
			Ename MANCANDY
			At root
			Type Local
			Flags OneAtATime
			While MALE
			ChildFX :Custom_IvyBody_Male_Activation.fx
		End
	End

	Condition
		On Triggerbits
		Triggerbits FEMALE
		DoMany 1

		Event
			Ename FEMALECANDY
			At root
			Type Local
			Flags OneAtATime
			While FEMALE
			ChildFX :Custom_IvyBody_Female_Activation.fx
		End
	End

	Condition
		On Triggerbits
		Triggerbits HUGE
		DoMany 1

		Event
			Ename HUGECANDY
			At root
			Type Local
			Flags OneAtATime
			While HUGE
			ChildFX :Custom_IvyBody_Huge_Activation.fx
		End
	End

##################################################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName RibbonChestParts
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Raging_Ivy_Leaf01.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Raging_Ivy_Leaf02.part
			#LifeSpan 100
		End

	End

	Condition
		On Time
		Time 35

		Event
			At Root
			Type PositOnly
			EName Anchor
		End

		Event
			At Anchor
			Type Local
			EName Particles
			BhvrOverride
				PositionOffset 0 1 0
			End
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Raging_Ivy_Ring01.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Raging_Ivy_Ring02.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Raging_Ivy_Leaf03.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Raging_Ivy_Leaf04.part
		End

		Event
			At Anchor
			Type Local
			Bhvr behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				PositionOffset 0 2.5 0
				FadeInLength 45
				Alpha 255
				StartColor 80 180 75
				PrimaryTint 80
				SecondaryTint 20
				Scale 5 5 5
				Spin 0 -0.01 0
				TintGeom 1
			End
			Geom FX_Tintable_Ivy_Ring_Big
		End

		Event
			At Anchor
			Type Local
			Bhvr behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				PositionOffset 0 1.5 0
				FadeInLength 45
				Alpha 255
				StartColor 80 180 75
				PrimaryTint 80
				SecondaryTint 20
				PyrRotate 0 165 0
				Scale 5 5 5
				Spin 0 -0.01 0
				TintGeom 1
			End
			Geom FX_Tintable_Ivy_Ring_Big
		End

	End

End
