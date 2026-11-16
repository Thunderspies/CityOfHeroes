#############################################################
## DuplicatingGift_Receiving.fx
#############################################################
FxInfo
Lifespan 40

###########################################################

	Condition
		On Time
		Time 15

		Event
			At Mystic
			Type Local
			EName Gift_Bottom
			Bhvr Events\Winter\DupeGift.bhvr
			BhvrOverride
				Scale		1.4 1.4 1.4
				PyrRotate	90 0 0
			End
			Geom GEO_HolidayGift2_Bottom
		End

		Event
			At Mystic
			Type Local
			EName Gift_Top
			Bhvr Events\Winter\DupeGift.bhvr
			BhvrOverride
				PositionOffset 	0 0 -0.56
				Scale		1.4 1.4 1.4
				PyrRotate	90 0 0
			End
			Geom GEO_HolidayGift2_Top
		End

	End

###########################################################

End