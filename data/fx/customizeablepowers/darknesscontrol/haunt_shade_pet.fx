#
FxInfo

	Flags DontSuppress


###########################################################


Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DC_Haunt_Spirit_Loop 100 100 .4
		bhvr	behaviors/Sound/SoundIn1.bhvr
		lifespan 10000
	End
End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			ChildFx :CHILD_Shade_Eyes.fx
			EName Eyes
			Flags OneAtATime
		End

		Event
			At Head
			Type Local
			Flags AdoptParentEntity
			Bhvr behaviors/FadeOut.bhvr
			LifeSpan 1
		End

		Event
			At Head
			Type Local
			EName HEAD
			BhvrOverride
				StartColor 1 1 1
			End
			Part :Head_Trail_Large.part
			Part CustomizeablePowers\DarknessControl\Shade_Core_Head.part
		End

		Event
			At Chest
			Type Local
			EName CHEST
			#POther Hips
			Bhvr behaviors/GenericParticleFade.bhvr
			Part CustomizeablePowers\DarknessControl\Shade_Highlights_Chest.part
			Part CustomizeablePowers\DarknessControl\Shade_Core_Chest.part
		End

		Event
			At UArmL
			Type Local
			EName UARM
			POther LArmL
			Part CustomizeablePowers\DarknessControl\Shade_Highlights_Arm.part
			Part CustomizeablePowers\DarknessControl\Shade_Core_Arm.part
		End

		Event
			At UArmR
			Type Local
			EName UARM
			POther LArmR
			Part CustomizeablePowers\DarknessControl\Shade_Highlights_Arm.part
			Part CustomizeablePowers\DarknessControl\Shade_Core_Arm.part
		End

		Event
			At LArmL
			Type Local
			EName ARM
			POther WepL
			Part CustomizeablePowers\DarknessControl\Shade_Highlights_Arm.part
			Part CustomizeablePowers\DarknessControl\Shade_Core_Arm.part
		End

		Event
			At LArmR
			Type Local
			EName ARM
			POther WepR
			Part CustomizeablePowers\DarknessControl\Shade_Highlights_Arm.part
			Part CustomizeablePowers\DarknessControl\Shade_Core_Arm.part
		End

		Event
			At Hips
			Type Local
			POther Chest
			Part CustomizeablePowers\DarknessControl\Shade_Highlights_Arm.part
			Part :Shade_Tail_Core.part
		End

		Event
			At Head
			Type Local
			EName HEAD
			POther Chest
			BhvrOverride
				StartColor 1 1 1
			End
			Part CustomizeablePowers\DarknessControl\Shade_Core_Neck.part
		End

	End

	Condition
		On Cycle
		Time 30
		Random 1
		Chance 0.250000

		#Event
			#At LArmL
			#Type Local
			#Bhvr CustomizeablePowers\DarknessControl\Shade_Scale_Small.bhvr
			#Geom Tintable_DarkGrasp
			#LifeSpan 15
		#End

		#Event
			#At LArmR
			#Type Local
			#Bhvr CustomizeablePowers\DarknessControl\Shade_Scale_Small.bhvr
			#BhvrOverride
				#PositionOffset 0.65 0 0
			#End
			#Geom Tintable_DarkGrasp
			#LifeSpan 15
		#End

		#Event
			#At UArmR
			#Type Local
			#Bhvr CustomizeablePowers\DarknessControl\Shade_Scale_Small.bhvr
			#BhvrOverride
				#PositionOffset 0.65 0 0
			#End
			#Geom Tintable_DarkGrasp
			#LifeSpan 15
		#End

		#Event
			#At UArmL
			#Type Local
			#Bhvr CustomizeablePowers\DarknessControl\Shade_Scale_Small.bhvr
			#Geom Tintable_DarkGrasp
			#LifeSpan 15
		#End

		Event
			At Chest
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\Shade_Scale.bhvr
			Geom Tintable_DarkGrasp
			LifeSpan 15
		End

		Event
			At Chest
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\Shade_Scale.bhvr
			Geom Tintable_DarkGrasp
			LifeSpan 15
		End

	End

End
