# Custom exception used for when a failure flag is run
class FailureException(Exception):
	def __init__(self, value):
		self.parameter = value
	def __str__(self):
		return repr(self.parameter)
