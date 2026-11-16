#
FxInfo

	LifeSpan 390


###########################################################

	#Condition
		#On Time
		#Time 0

		#Event
			#At Root
			#Type Local
			#Sound MoM_Suffer_Loop 100 100 0.5
		#End

	#End

	Condition
		On Time
		Time 0
	Event
		Type	Local
		At	Root
		Sound 	MoM_WorldOfAnguish_Short_01 100 100 .8
		LifeSpan 390
	End

		Event
			At Root
			Type Local
			EName RadiusFxScale
			Bhvr POWERS\FireControl\Inferno.bhvr
			BhvrOverride
				PositionOffset 0 2 0
			End
		End

		Event
			At RadiusFxScale
			Type Local
			EName splode
			Bhvr ENEMYFX\Praetorian\MoM\RingExpand_X3.bhvr
			BhvrOverride
				Scale 0.1 0.1 0.1
				EndScale .5 .5 .5
			End
			Part ENEMYFX\Praetorian\MoM\Flash_Small.part
			Part ENEMYFX\Praetorian\MoM\Blobby_Anguish_Burst.part
			LifeSpan 3
		End

		Event
			At RadiusFxScale
			Type StartPositOnly
			Bhvr ENEMYFX\Praetorian\MoM\RingExpand_X3.bhvr
			BhvrOverride
				Scale 0.5 0.5 0.5
				EndScale 1 0.5 1
				Spin 0 0.005 0
				StartColor 154 88 255
				Alpha 180
			End
			Part ENEMYFX\Praetorian\MoM\Suffer_Five_Ground02.part
			Part ENEMYFX\Praetorian\MoM\Suffer_Five_Ground.part
			Geom FX_SufferRing_10
			LifeSpan 120
		End

	End

	Condition
		On Time
		Time 120

		Event
			At RadiusFxScale
			Type Local
			EName splode
			Bhvr ENEMYFX\Praetorian\MoM\RingExpand_X3.bhvr
			BhvrOverride
				Scale 0.1 0.1 0.1
				EndScale .5 .5 .5
			End
			#Part ENEMYFX\Praetorian\MoM\Flash_Small.part
			Part ENEMYFX\Praetorian\MoM\Blobby_Anguish_Burst.part
			LifeSpan 3
		End

		Event
			At RadiusFxScale
			Type StartPositOnly
			Bhvr ENEMYFX\Praetorian\MoM\RingExpand_X3.bhvr
			BhvrOverride
				Scale 0.25 0.25 0.25
				EndScale 0.6 0.6 0.6
				Spin 0 0.005 0
				StartColor 218 117 255
				Alpha 180
			End
			Part ENEMYFX\Praetorian\MoM\Suffer_Five_Ground03.part
			Part ENEMYFX\Praetorian\MoM\Suffer_Five_Ground04.part
			Geom FX_SufferRing_30
			LifeSpan 120
		End

	End

	Condition
		On Time
		Time 240

		Event
			At RadiusFxScale
			Type Local
			EName splode
			Bhvr ENEMYFX\Praetorian\MoM\RingExpand_X3.bhvr
			BhvrOverride
				Scale 0.1 0.1 0.1
				EndScale .5 .5 .5
			End
			#Part ENEMYFX\Praetorian\MoM\Flash_Small.part
			Part ENEMYFX\Praetorian\MoM\Blobby_Anguish_Burst.part
			LifeSpan 3
		End

		Event
			At RadiusFxScale
			Type StartPositOnly
			Bhvr ENEMYFX\Praetorian\MoM\RingExpand_X3.bhvr
			BhvrOverride
				Scale 0.5 0.5 0.5
				EndScale 2 1 2
				Spin 0 0.005 0
				StartColor 154 88 255
				Alpha 180
			End
			Part ENEMYFX\Praetorian\MoM\Suffer_Five_Ground02.part
			Part ENEMYFX\Praetorian\MoM\Suffer_Five_Ground.part
			Geom FX_SufferRing_10
			LifeSpan 120
		End

	End

End
