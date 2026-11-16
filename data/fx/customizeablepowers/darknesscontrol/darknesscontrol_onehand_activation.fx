#############################################################
## ShadowyBinds_Attack.fx
#############################################################

FxInfo

	Flags DontSuppress


###########################################################

	Condition
		On Cycle
		Time 7

		Event
			At WepL
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\Shade_Scale_Small.bhvr
			BhvrOverride
				PYRRotate	0 0 0
				PYRRotateJitter	180 180 180
				PositionOffset	0.0 0.0 0.0
				StartJitter 	0.0 0.0 0.0
				EndScale	0.15 0.2 0.15
			End
			Geom Tintable_DarkGrasp
			LifeSpan 15
		End
	End

	Condition
		On Time
		Time 15

		Event
			At WepL
			Type Local
			Part	:Hand_Core_Solid.part
			Part	:Hand_Glow.part
			Part	:Hand_Glow_Faint.part
		End
	End

#############################################################

End
