#
FxInfo

	LifeSpan 900


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
		Sound 	MoM_WorldOfAnguish_01 100 100 .8
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
			Part ENEMYFX\Praetorian\DD\Flash_Small.part
			Part ENEMYFX\Praetorian\DD\SentinelProc_AuraOfDecay_Burst.part
			LifeSpan 3
		End

		Event
			At RadiusFxScale
			Type StartPositOnly
			Bhvr ENEMYFX\Praetorian\MoM\RingExpand_X3.bhvr
			BhvrOverride
				Scale 0.5 0.5 0.5
				EndScale 3 1 3
				Spin 0 0.0200 0
				StartColor 200 212 255
				Alpha 180
			End
			Part ENEMYFX\Praetorian\DD\SentinelProc_AuraOfDecay_Ground01.part
			Part ENEMYFX\Praetorian\DD\SentinelProc_AuraOfDecay_Ground02.part
			Geom FX_SufferRing_10
			LifeSpan 900
		End

	End

End
