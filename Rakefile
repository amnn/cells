class CC
  class Logger
    def log(str)
      puts str
    end
  end

  class NullLogger
    def log(str)
    end
  end

  DEFAULTS = {
    'CC'       => 'g++-4.9 --std=c++11',
    'GLEW'     => `pkg-config --static --libs glew`.strip,
    'GLFW'     => `pkg-config --static --libs glfw3`.strip,
    'DEST'     => 'bin/cells',
    'SRC'      => 'src',
    'OBJ'      => 'bin/obj',
    'INCLUDES' => 'includes',
    'VERBOSE'  => false
  }

  def initialize(opts = {})
    @env    = DEFAULTS.merge(opts)
    @logger = env['VERBOSE'] ? Logger.new : NullLogger.new
  end

  def link(objects, target)
    logger.log "Linking #{target} ..."
    logger.log `#{compile_cmd} #{objects.join(' ')} -o #{target}`
  end

  def compile(source, target)
    logger.log "Compiling #{source} -> #{target}"
    logger.log `#{compile_cmd} -c #{source} -o #{target}`
  end

  def compile_cmd
    "#{env['CC']} #{env['GLEW']} #{env['GLFW']} -I#{env['INCLUDES']}"
  end

  def object_files
    FileList[ source('*.cpp') ]
      .map { |fname| object(fname) }
  end

  def source(file)
    File.join( env['SRC'], File.basename(file).ext('cpp') )
  end

  def object(file)
    File.join( env['OBJ'], File.basename(file).ext('o') )
  end

private
  attr_reader :env, :logger

end

@cc = CC.new(ENV)

task default: ['bin/cells']

directory 'bin/obj'

rule '.o' => [@cc.method(:source).to_proc, 'bin/obj'] do |t|
  @cc.compile(t.source, t.name)
end

file 'bin/cells' => @cc.object_files do |t|
  @cc.link(t.prerequisites, t.name)
end
